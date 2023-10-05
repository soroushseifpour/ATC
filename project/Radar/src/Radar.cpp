#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <cstdlib>
#include <errno.h>
#include <string.h>
#include <sys/dispatch.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#define ONE_THOUSAND 1000
#define ONE_MILLION 1000000
/* offset and period are in microseconds. */
#define OFFSET 1000000
#define PERIOD 1000000
using namespace std;
using namespace std;
#define ATTACH_POINT_Radar_CompSYS "server_client_Radar_ComputerSYS"

#define ATTACH_POINT_FOR_SERVER_OF_COMMINUCATION_AND_RADAR "server_client_CominuSys_Radar"
typedef struct _pulse msg_header_t;
sigset_t sigst;
//////////////////////Running each 5 second///////////////////
//int **arr;
const int numberOfAirplanes=20;
int arr[numberOfAirplanes][numberOfAirplanes];
int arr3Min[numberOfAirplanes][numberOfAirplanes];
vector<string> positonsOfAirplanesEach30s;
static void wait_next_activation(void) {
    int dummy;
    /* suspend calling process until a signal is pending */
    sigwait(&sigst, &dummy);
}

int start_periodic_timer(uint64_t offset, int period) {
    struct itimerspec timer_spec;
    struct sigevent sigev;
    timer_t timer;
    const int signal = SIGALRM;
    int res;

    /* set timer parameters */
    timer_spec.it_value.tv_sec = offset / ONE_MILLION;
    timer_spec.it_value.tv_nsec = (offset % ONE_MILLION) * ONE_THOUSAND;
    timer_spec.it_interval.tv_sec = period / ONE_MILLION;
    timer_spec.it_interval.tv_nsec = (period % ONE_MILLION) * ONE_THOUSAND;

    sigemptyset(&sigst); // initialize a signal set
    sigaddset(&sigst, signal); // add SIGALRM to the signal set
    sigprocmask(SIG_BLOCK, &sigst, NULL); //block the signal

    /* set the signal event a timer expiration */
    memset(&sigev, 0, sizeof(struct sigevent));
    sigev.sigev_notify = SIGEV_SIGNAL;
    sigev.sigev_signo = signal;

    /* create timer */
    res = timer_create(CLOCK_MONOTONIC, &sigev, &timer);

    if (res < 0) {
        perror("Timer Create");
        exit(-1);
    }

    /* activate the timer */
    return timer_settime(timer, 0, &timer_spec, NULL);
}
//////////////////RUnning each second//////////////////////////

///////////////////////////////////////////////
typedef struct _my_data {
    msg_header_t hdr;
    int data;
    char mymessage[1024];
    int ID;
    float x;
    float y;
    float z;
    int vx;
    int vy;
    int vz;
    float deg;
    string info;
} my_data_t;

struct point{
	float x;
	float y;
	float z;
};
typedef struct comminucationMSG {
    msg_header_t hdr;
    int aireplaneID;
    char mymessage[4096];
    string option;
    int value;
    string index;
} comminucationMSG_t;
vector<comminucationMSG_t> recievedMsgRadar2Commi;
bool hasMsgRecievedRadar2Commi=false;
struct airplane{
	vector<point> vertices;
	point origin;
	string ID;

	int width=100000;
	int height=25000;
	int length=100000;

	int vx=10;
	int vy=10;
	int vz=0;

	float x;
	float y;

	float deg=45;

	int lineEquation;

		void setTheOriginOfAirplane(point origin){
			///...After generating we need to set the value of the origin center of the airplanes
			this->origin.x=origin.x;
			this->origin.y=origin.y;
			this->origin.z=origin.z;
		}
		void setID(string name){
			this->ID=name;
		}
		void printTheVerticiesOFAirPlane(){
			for(int i=0;i<this->vertices.size();i++){
				cout<<"set :"<< i<< " ";
				cout << this->vertices[i].x << " " <<this->vertices[i].y << " " << this->vertices[i].z  << " ";
		}


		}
		 point getAirplanePostion(){
			 point p_airplane;
			 p_airplane.x=this->x;
			 p_airplane.y=this->y;
			 p_airplane.z=this->origin.z;

			 return p_airplane;
		 }
};


point generatingRandomOriginCenterOfAirplane(){

	//...Make a point
	point origin;
	origin.x= rand() % 100001 - 50000;
	origin.y= rand() % 25001 - 12500;
	origin.z = rand() % 85001 + 15000;
	return origin;
}

struct canvas{
	point origin;
	int width=100000;
	int height=25000;
	int length=100000;

	vector<point> vertices;
	void draw(){
		int halfX=(this->height)/2;
		int halfY=(this->width)/2;
		int halfZ=(this->length)/2;
		///...first vertex
		point first;
		first.x=this->origin.x-halfY;
		first.y=this->origin.y-halfX;
		first.z=100000;
		///...second vertex
		point second;
		second.x=this->origin.x+halfY;
		second.y=this->origin.y-halfX;
		second.z=100000;
		///...third vertex
		point third;
		third.x=this->origin.x+halfY;
		third.y=this->origin.y+halfX;
		third.z=100000;
		///...fourth vertex
		point fourth;
		fourth.x=this->origin.x-halfY;
		fourth.y=this->origin.y+halfX;
		fourth.z=100000;
		///...fifth vertex
		point fifth;
		fifth.x=this->origin.x-halfY;
		fifth.y=this->origin.y-halfX;
		fifth.z=15000;
		///...sixth vertex
		point sixth;
		sixth.x=this->origin.x+halfY;
		sixth.y=this->origin.y-halfX;
		sixth.z=15000;
		///...seventh vertex
		point seventh;
		seventh.x=this->origin.x+halfY;
		seventh.y=this->origin.y+halfX;
		seventh.z=15000;
		///...eighth vertex
		point eighth;
		eighth.x=this->origin.x-halfY;
		eighth.y=this->origin.y+halfX;
		eighth.z=15000;
		///...setting them into the vector
		this->vertices.push_back(first);
		this->vertices.push_back(second);
		this->vertices.push_back(third);
		this->vertices.push_back(fourth);
		this->vertices.push_back(fifth);
		this->vertices.push_back(sixth);
		this->vertices.push_back(seventh);
		this->vertices.push_back(eighth);
	}
};
canvas cnv;

int Radar2CompSysClient(airplane airplane,string info=""){
	///////////////////////////
    my_data_t msg;
    int server_coid; //server connection ID.

    if ((server_coid = name_open(ATTACH_POINT_Radar_CompSYS, 0)) == -1) {
        return EXIT_FAILURE;
    }

    msg.hdr.type = 0x00;
    msg.hdr.subtype = 0x01;
    string myString="";
	/* input a message from keyboard */
	myString="ID: "+airplane.ID+" X:"+to_string(airplane.x)+" Y:"+to_string(airplane.y)+" Z:"+to_string(airplane.origin.z)+"\n";
   if(info != ""){
	   myString=myString+"\n"+info;
   }
   strcpy(msg.mymessage, myString.c_str());
   msg.ID=stoi(airplane.ID);
   msg.vx=airplane.vx;
   msg.vy=airplane.vy;
   msg.x=airplane.x;
   msg.y=airplane.y;
   msg.z=airplane.origin.z;
   msg.deg=airplane.deg;
   MsgSend(server_coid, &msg, sizeof(msg), NULL, 0);
    name_close(server_coid);
    return EXIT_SUCCESS;
}
void add_10_to_point(airplane& p) {
	p.origin.x +=10;
	p.origin.y+=10;
	p.origin.z+=10;
}
void moveAirplane(airplane& airplane, double current){
	///x=vt+x0
//	cerr<<current<<"\n";
	float v=sqrt(airplane.vx*airplane.vx+airplane.vy*airplane.vy);
	airplane.x=v*cos(airplane.deg)*current+airplane.origin.x;
	airplane.y=v*sin(airplane.deg)*current+airplane.origin.y;
//	cerr<<"airplane.x "<<airplane.x<<" cos(airplane.deg)" << cos(airplane.deg) << "  " << " V:" <<v <<" current:" << current<<  "\n";
}
int isTheAirplaneVisible(airplane& airplane){
    float minX = min_element(cnv.vertices.begin(), cnv.vertices.end(), [](point a, point b) { return a.x < b.x; })->x;
    float maxX = max_element(cnv.vertices.begin(), cnv.vertices.end(), [](point a, point b) { return a.x < b.x; })->x;
    float minY = min_element(cnv.vertices.begin(), cnv.vertices.end(), [](point a, point b) { return a.y < b.y; })->y;
    float maxY = max_element(cnv.vertices.begin(), cnv.vertices.end(), [](point a, point b) { return a.y < b.y; })->y;
    float minZ = min_element(cnv.vertices.begin(), cnv.vertices.end(), [](point a, point b) { return a.z < b.z; })->z;
    float maxZ = max_element(cnv.vertices.begin(), cnv.vertices.end(), [](point a, point b) { return a.z < b.z; })->z;
//    cerr<<minX<<" "<<maxX<<" " << minY <<" "<< maxY << minZ <<" "<< maxZ;
    if (airplane.origin.x >= minX && airplane.origin.x <= maxX && airplane.origin.y >= minY && airplane.origin.y <= maxY && airplane.origin.z >= minZ && airplane.origin.z <= maxZ) {
        return 1;
    } else {
        return 0;
    }
}
// calculates the squared distance between two points at a given time
double squaredDistanceAtTime(airplane& p1, airplane& p2, int time) {
    int dx = p2.x - p1.x + p2.vx * time - p1.vx * time;
    int dy = p2.y - p1.y + p2.vy * time - p1.vy * time;
    int dz = p2.origin.z - p1.origin.z + p2.vz * time - p1.vz * time;
    return dx * dx + dy * dy + dz * dz;
}
// checks if any pair of points will collide at the same time
int collisionTime;
string informationForTheFuture;
string infromationFor3Min;
const double EPSILON = 15000;
void willCollide(airplane* airplanes,int t) {
    for (int i = 0; i < numberOfAirplanes; i++) {
        for (int j = i + 1; j < numberOfAirplanes; j++) {
//            double distance = sqrt(pow(airplanes[i].x - airplanes[j].x - airplanes[j].vx*t + airplanes[i].vx*t, 2) + pow(airplanes[i].y - airplanes[j].y - airplanes[j].vy*t + airplanes[i].vy*t, 2));
//        	cerr<<distance<<"\n";
        	double distance =abs(airplanes[i].x - airplanes[j].x);
        	if(distance <= 1500){
        		arr[i][j]=1;
        		arr[j][i]=1;
        		informationForTheFuture="Two aircrafts with ID: "+ to_string(i) + " and " + to_string(j)+ " might collide soon.!!!!!!!" +"\n" + informationForTheFuture;
//        		cerr<<"i:"<<i << "j:" <<j<< "distane:" << distance<<"\n";
//        		cerr<< informationForTheFuture;
            }
        }
    }
}
void calculateIn3Min(airplane* airplanes,int t) {
	t=t+180;
    for (int i = 0; i < numberOfAirplanes; i++) {
        for (int j = i + 1; j < numberOfAirplanes; j++) {
            double distance = sqrt(pow(airplanes[i].x - airplanes[j].x - airplanes[j].vx*t + airplanes[i].vx*t, 2) + pow(airplanes[i].y - airplanes[j].y - airplanes[j].vy*t + airplanes[i].vy*t, 2));
//        	cerr<<airplanes[i].x <<"\n"<<airplanes[j].x<<"\n";
            if(distance <= 1500){
        		arr3Min[i][j]=1;
        		arr3Min[j][i]=1;
        	}
            else{
            	arr3Min[i][j]=-1;
            }
        }
    }
}
bool checkWhetherTheAirplanesWillcollide() {
    for (int i = 0; i < numberOfAirplanes; i++) {
        for (int j = i + 1; j < numberOfAirplanes; j++) {
        		if(arr3Min[i][j]==1){
        			///...they will certainly collide
        			infromationFor3Min="Two aircrafts with ID: "+ to_string(i) + " and " + to_string(j)+ "will collide to each other in the future.!!!!!!!"+"\n" +infromationFor3Min;
//        			cerr<<informationForTheFuture;
        			return true;
        		}
        		else if(arr3Min[i][j] != arr[i][j]){
        			arr[i][j]=-1;
        			arr[j][i]=-1;
        		}
        }
    }
    return false;
}
void update(comminucationMSG_t reMsg,airplane* &airplanes){
	///...find the airplane
	int index=reMsg.aireplaneID;
	airplane* selectedAriplane = &airplanes[index];
	cerr<<reMsg.mymessage;
	////...which option did the user choose
	if(reMsg.index=="Vx"){
		cerr<<"change"<<"\n";
		selectedAriplane->vx=reMsg.value;
		cerr<<selectedAriplane->vx;
	}
	else if(reMsg.index=="Vy"){
		selectedAriplane->vy=reMsg.value;
	}
	else{
		selectedAriplane->deg=reMsg.value;
	}
}
void saveAirplanePosition(point p,string id){
	string pos="ID:"+id+ " X:"+to_string(p.x)+" Y:"+ to_string(p.y)+ " Z:"+ to_string(p.z);
	positonsOfAirplanesEach30s.push_back(pos);
}
void writeTheLogs(string sample){
    string myString = sample;
//    cerr<<myString;
    string filePath = "C:\\Users\\sseif\\OneDrive\\Desktop\\log.txt";

    // Open file in append mode
    ofstream myFile(filePath, ios::app);

    // Check if file was successfully opened
    if (myFile.is_open()) {
        // Append string to file
        myFile << myString;
        // Flush data to file
        myFile.flush();
        // Close file
        myFile.close();
        cout << "File updated successfully." << endl;
    }
    else {
        // Create new file
        ofstream newFile(filePath);

        // Check if file was successfully created
        if (newFile.is_open()) {
            // Write string to file
            newFile << myString;

            // Close file
            newFile.close();
            cout << "File created successfully." << endl;
        }
        else {
            cout << "Error creating file." << endl;
        }
    }
}
void task(airplane* &airplanes, int num_points) {
    static int cycles = 0;
    static uint64_t start;
    uint64_t current;
    struct timespec tv;

    if (start == 0) {
        clock_gettime(CLOCK_MONOTONIC, &tv);
        start = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
    }

    clock_gettime(CLOCK_MONOTONIC, &tv);
    current = tv.tv_sec * ONE_THOUSAND + tv.tv_nsec / ONE_MILLION;
    int seconds = static_cast<int>(current) / 1000000.0;
    bool collide=false;
    if (cycles % 1 == 0) {
        for (int i = 0; i < num_points; i++) {
        		moveAirplane(airplanes[i],cycles);
        }
    	if(recievedMsgRadar2Commi.size()==1){
    		comminucationMSG_t commMsg;
    		commMsg = recievedMsgRadar2Commi.back();
    		recievedMsgRadar2Commi.pop_back();
    		update(commMsg,airplanes);
    	}
    	willCollide(airplanes,cycles);
    	calculateIn3Min(airplanes,cycles);
    	collide=checkWhetherTheAirplanesWillcollide();
    	if(cycles % 5 != 0){
			informationForTheFuture="";
			infromationFor3Min="";
    	}
//    	cerr<<collide;
    }
        if (cycles % 5 == 0) {
        	string m=informationForTheFuture+ infromationFor3Min;
        	///...check if it is in the canvas
            for (int i = 0; i < num_points; i++) {
    			int check=isTheAirplaneVisible(airplanes[i]);
//            	cerr<<"vx:"<<airplanes[i].vx;
    			if(check==1){
    				if(collide==true){
    						if(i==num_points-1)
        						Radar2CompSysClient(airplanes[i],m);
    						else
    							Radar2CompSysClient(airplanes[i]);
    				}
    				else{
    					Radar2CompSysClient(airplanes[i]);
    				}

    			}

            }
//    		if(positonsOfAirplanesEach30s.size()>0){
//    			positonsOfAirplanesEach30s.clear();
//    		}
//    		string s="";
//    		for(int i=0; i<numberOfAirplanes; i++){
//    				point pointOFAirPlanes= airplanes[i].getAirplanePostion();
//    				saveAirplanePosition(pointOFAirPlanes,airplanes[i].ID);
//    				///..log for the airplanes
//    				s=s+"The position of the airplane with the ID: "+airplanes[i].ID+" with the X: " +to_string(pointOFAirPlanes.x)+" with the Y: "+to_string(pointOFAirPlanes.y)+ " with the Z: "+to_string(pointOFAirPlanes.z)+"\n";
//    		}
//    				writeTheLogs(s);
        }

    if (cycles % 11 == 0) {
    		if(positonsOfAirplanesEach30s.size()>0){
    			positonsOfAirplanesEach30s.clear();
    		}
    		string s="";
    		for(int i=0; i<numberOfAirplanes; i++){
    				point pointOFAirPlanes= airplanes[i].getAirplanePostion();
    				saveAirplanePosition(pointOFAirPlanes,airplanes[i].ID);
    				///..log for the airplanes
    				s=s+"The position of the airplane with the ID: "+airplanes[i].ID+" with the X: " +to_string(pointOFAirPlanes.x)+" with the Y: "+to_string(pointOFAirPlanes.y)+ " with the Z: "+to_string(pointOFAirPlanes.z)+"\n";
    		}
    				writeTheLogs(s);
    }
	cycles++;

}


void *periodic_task(void *arg) {
    airplane* airplanes = (airplane*)arg;
    int res;

    //set and activate a timer
    res = start_periodic_timer(OFFSET, PERIOD);
    if (res < 0 ){
        perror("Start periodic timer");
        exit(-1);
    }
    while (1) {
        wait_next_activation(); //wait for timer expiration
        task(airplanes, numberOfAirplanes); //executes the task for 2 points
    }
    return NULL;
}

/*** Server Side of the Radar2ComminucationSys ***/
void* Radar2ComminucationSys(void*) {
   name_attach_t *attach;
   comminucationMSG_t msg;
   int rcvid;
   if ((attach = name_attach(NULL, ATTACH_POINT_FOR_SERVER_OF_COMMINUCATION_AND_RADAR, 0)) == NULL) {
//       return EXIT_FAILURE;
   }

   /* Do your MsgReceive's here now with the chid */
   while (1) {
//	   hasMsgRecievedRadar2Commi=false;
       rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);
       if (rcvid == -1) {/* Error condition, exit */
           break;
       }

       if (rcvid == 0) {/* Pulse received */
           switch (msg.hdr.code) {
           case _PULSE_CODE_DISCONNECT:
               ConnectDetach(msg.hdr.scoid);
               break;
           case _PULSE_CODE_UNBLOCK:
               break;
           default:
               break;
           }
           continue;
       }

       /* name_open() sends a connect message, must EOK this */
       if (msg.hdr.type == _IO_CONNECT ) {
           MsgReply( rcvid, EOK, NULL, 0 );
           continue;
       }

       /* Some other QNX IO message was received; reject it */
       if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
           MsgError( rcvid, ENOSYS );
           continue;
       }
	   if (msg.hdr.type == 0x00) {
	      if (msg.hdr.subtype == 0x01) {
              /* A message (presumable ours) received, handle */
//              printf("Server receive %s \n", msg.mymessage);
	    	  cerr<<msg.mymessage;
	    	  cerr<<msg.aireplaneID;
	    	  cerr<<msg.index;
	    	  cerr<<msg.option;
	    	  cerr<<msg.value;
	    	  recievedMsgRadar2Commi.push_back(msg);
	    	  hasMsgRecievedRadar2Commi=true;

	      }
	   }

       MsgReply(rcvid, EOK, 0, 0);

   }

   /* Remove the name from the space */
   name_detach(attach, 0);

//   return EXIT_SUCCESS;
}
int main()
{
	srand(time(nullptr));

	////...making the environment...///
//	point c=generatingRandomOriginCenterOfAirplane();
	cnv.origin.x=0;
	cnv.origin.y=0;
	cnv.origin.z=0;
	cnv.draw();
    ///...x-coordinates (-50000 to 50000)
    ///...y-coordinates (-12500 to 12500)
	///...z-coordinates (15000 to 100000)
//	airplane airplane1;
//    point airplanePoints1=generatingRandomOriginCenterOfAirplane();
//    airplane1.setTheOriginOfAirplane(airplanePoints1);
//    airplane1.origin.x=0;
//    airplane1.origin.y=0;
//    airplane1.origin.z=20000;
//    airplane1.setID("1");
    ////...make the line equation

//    airplane airplane2;
//    point airplanePoints2=generatingRandomOriginCenterOfAirplane();
//    airplane2.setTheOriginOfAirplane(airplanePoints2);
//    airplane2.origin.x=500;
//    airplane2.origin.y=120;
//    airplane2.origin.z=20000;
//    airplane2.setID("2");

//	airplane airplane3;
//    point airplanePoints3=generatingRandomOriginCenterOfAirplane();
//    airplane3.setTheOriginOfAirplane(airplanePoints3);
//    airplane3.origin.x=500;
//    airplane3.origin.y=120;
//    airplane3.origin.z=80000;
//    airplane3.setID("3");

    airplane airplanes[numberOfAirplanes];
//    airplanes[0] = airplane1;
//    airplanes[1] = airplane2;
//    airplanes[2] = airplane3;

    for (int i = 0; i < numberOfAirplanes; i++) {
        for (int j = 0; j < numberOfAirplanes; j++) {
        	if(j!=i)
            arr[i][j] = -1;
        	else
            arr[i][j] = 0;
        }
    }

    for(int i=0;i<numberOfAirplanes;i++){
    	airplane airplane;
    	point airplanePoints=generatingRandomOriginCenterOfAirplane();
    	airplane.setTheOriginOfAirplane(airplanePoints);
    	airplane.setID(to_string(i));
    	airplanes[i]=airplane;
    }
    pthread_t thread1,thread2;
    // create a thread to run periodic_task()
    int res = pthread_create(&thread1, NULL, periodic_task, (void *)airplanes);
    pthread_create(&thread2, nullptr, &Radar2ComminucationSys, nullptr);
//    int ret = pthread_create(&tmove, NULL, updating_matrix, (void *)airplanes);
    if (res != 0) {
        perror("Thread create");
    }

//	 wait for the threads to complete
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
    return 0;
}

