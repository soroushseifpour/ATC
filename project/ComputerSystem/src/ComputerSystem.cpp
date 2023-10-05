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
#define ATTACH_POINT_Radar_CompSYS "server_client_Radar_ComputerSYS"
#define ATTACH_POINT_CompSys_DataDisplay "server_client_ComSys_DataDisplay"
#define ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_OPERATOR_CONSOLE "server_client_ComSys_Operator"
#define ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_COMMUNICATION "server_client_ComSys_Communication"
typedef struct _pulse msg_header_t;

using namespace std;

typedef struct _my_data {
    msg_header_t hdr;
    int data;
    char mymessage[1024];
    int ID;
    int x;
    int y;
    int z;
    int vx;
    int vy;
    int vz;
    float deg;
} my_data_t;
my_data_t msg;
vector<my_data_t> msg_messages;
//typedef struct messageBtwComSysAndOperator {
//    msg_header_t hdr;
//    int data;
//    char mymessage[1024];
//} msgBtwComOpr;
typedef struct messageBtwComSysAndOperator {
    msg_header_t hdr;
    int aireplaneID;
    char mymessage[4096];
    string option;
    int value;
    string index;
} msgBtwComOpr;
typedef struct comminucationMSG {
    msg_header_t hdr;
    int aireplaneID;
    char mymessage[4096];
    string option;
    int value;
    string index;
} comminucationMSG_t;
int findTheIndexOfChar(string sample,char s){
	for(int i=0;i<sample.size();i++){
		if(sample[i]==s){
			return i;
		}
	}
}
string selectTheInformationOfAirplane(int line, int opt){
	my_data_t airplane;
	for(int i=0;i<msg_messages.size();i++){
		if(msg_messages[i].ID == line){
			airplane=msg_messages[i];
			break;
		}
	}
	cerr<<line<<"-"<<opt;
	//...speed
	if(opt=1){
//		return "dd";
		return "************ The info your were looking for was V of the airplane "+to_string(line)+" : "+to_string(sqrt(pow(airplane.vx,2)+pow(airplane.vy,2)))+"**************************";
	}
	//...altitude
	else if(opt=2){
		return "************ The info your were looking for was DEGREE:"+to_string(airplane.deg)+"**************************";
	}
	else if(opt=3){
		return "************ The info your were looking for was POSTION of the airplane "+to_string(line)+" : "+to_string(sqrt(pow(airplane.x,2)+pow(airplane.y,2)))+"**************************";
	}
	else
		return "All info";
}
////Client for communication btw ComputerSys and DataDisplay
int CompSys2DataDisplayClient(char m[]) {
    my_data_t msg;
    int server_coid; //server connection ID.
    if ((server_coid = name_open(ATTACH_POINT_CompSys_DataDisplay, 0)) == -1) {
        return EXIT_FAILURE;
    }
    msg.hdr.type = 0x00;
    msg.hdr.subtype = 0x01;
    ///////////////////////////////
	strcpy(msg.mymessage, m);
//	printf("%s",msg.mymessage);
    MsgSend(server_coid, &msg, sizeof(msg), NULL, 0);
    name_close(server_coid);
    return EXIT_SUCCESS;
}
int CompSys2CommunicationClient(msgBtwComOpr msgBCO) {
	comminucationMSG_t msg;
    int server_coid; //server connection ID.
    if ((server_coid = name_open(ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_COMMUNICATION, 0)) == -1) {
        return EXIT_FAILURE;
    }
    msg.hdr.type = 0x00;
    msg.hdr.subtype = 0x01;
    ///////////////////////////////
	msg.aireplaneID=msgBCO.aireplaneID;
	msg.index=msgBCO.index;
	msg.option=msgBCO.option;
	msg.value=msgBCO.value;
//	printf("%s",msg.mymessage);
    MsgSend(server_coid, &msg, sizeof(msg), NULL, 0);
    name_close(server_coid);
    return EXIT_SUCCESS;
}
////server for communication btw ComputerSys and Radar
void* Radar2ComSysServer(void*) {
   name_attach_t *attach;
   int rcvid;

   if ((attach = name_attach(NULL, ATTACH_POINT_Radar_CompSYS, 0)) == NULL) {
//       return EXIT_FAILURE;
   }

   /* Do your MsgReceive's here now with the chid */
   while (1) {
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
       if (msg.hdr.type == _IO_CONNECT ) {
           MsgReply( rcvid, EOK, NULL, 0 );
           continue;
       }
       if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX ) {
           MsgError( rcvid, ENOSYS );
           continue;
       }
	   if (msg.hdr.type == 0x00) {
	      if (msg.hdr.subtype == 0x01) {
	    	  if(msg_messages.size()!=3){
	    		  msg_messages.push_back(msg);
	    	  }
	    	  else if(msg_messages.size()==3){
	    		  msg_messages.erase(msg_messages.begin());
	    		  msg_messages.push_back(msg);
	    	  }
	    	  CompSys2DataDisplayClient(msg.mymessage);
	      }
	   }
       MsgReply(rcvid, EOK, 0, 0);

   }
   name_detach(attach, 0);

//   return EXIT_SUCCESS;
}

/*** Server Side of the code ***/
void* Operator2ComSysServer(void*) {
   name_attach_t *attach;
   msgBtwComOpr msg_sample;

   int rcvid;

   /* Create a local name (/dev/name/local/...) */
   if ((attach = name_attach(NULL, ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_OPERATOR_CONSOLE, 0)) == NULL) {
//       return EXIT_FAILURE;
   }

   /* Do your MsgReceive's here now with the chid */
   while (1) {
       rcvid = MsgReceive(attach->chid, &msg_sample, sizeof(msg_sample), NULL);
       if (rcvid == -1) {
           break;
       }

       if (rcvid == 0) {
           switch (msg_sample.hdr.code) {
           case _PULSE_CODE_DISCONNECT:
               ConnectDetach(msg_sample.hdr.scoid);
               break;
           case _PULSE_CODE_UNBLOCK:
               break;
           default:
               break;
           }
           continue;
       }
       if (msg_sample.hdr.type == _IO_CONNECT ) {
           MsgReply( rcvid, EOK, NULL, 0 );
           continue;
       }
       if (msg_sample.hdr.type > _IO_BASE && msg_sample.hdr.type <= _IO_MAX ) {
           MsgError( rcvid, ENOSYS );
           continue;
       }
	   if (msg_sample.hdr.type == 0x00) {
	      if (msg_sample.hdr.subtype == 0x01) {
	    	  //..Sending Data to Data Display to alert the changes...
	    	  string airplaneID=to_string(msg_sample.aireplaneID);
	    	  string value=to_string(msg_sample.value);
	    	  string index=msg_sample.index;
	    	  string m="The airplane with ID :"+airplaneID+" has new "+ index + "with value: "+ value;
	    	  char msg_c[1024];
	    	  strcpy(msg_c,m.c_str());
	    	  CompSys2DataDisplayClient(msg_c);
	    	  ///...sending data to comminucationSystem
	    	  CompSys2CommunicationClient(msg_sample);
//	    	  string mystring(msg_sample.mymessage);
//	    	  int index=findTheIndexOfChar(mystring,'-');
//	    	  string option=mystring.substr(0, index);
//	    	  string aireplaneIndex=mystring.substr(index+1, -1);
//	    	  int convertedIndexOption=stoi(option);
//	    	  int convertedIndexAirplane=stoi(aireplaneIndex);
//	    	  string m=selectTheInformationOfAirplane(convertedIndexOption,convertedIndexAirplane);
//	    	  strcpy(msg_sample.mymessage,m.c_str());
//	    	  CompSys2DataDisplayClient(msg_sample.mymessage);
	      }
	   }

       MsgReply(rcvid, EOK, 0, 0);

   }

   /* Remove the name from the space */
   name_detach(attach, 0);

//   return EXIT_SUCCESS;
}
int main(int argc, char **argv) {
    int ret,retc;
	printf("Running Server ... \n");
	////server for communication btw ComputerSys and Radar
    // Create two threads, one for each server
    pthread_t thread1, thread2;
    pthread_create(&thread1, nullptr, &Radar2ComSysServer, nullptr);
    pthread_create(&thread2, nullptr, &Operator2ComSysServer, nullptr);

    // Wait for both threads to finish
    pthread_join(thread1, nullptr);
    pthread_join(thread2, nullptr);
	return 0;
}
