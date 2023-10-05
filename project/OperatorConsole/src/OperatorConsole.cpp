#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
//#include <sys/shm.h>
#include <sys/mman.h>
using namespace std;

#define ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_OPERATOR_CONSOLE "server_client_ComSys_Operator"

typedef struct _pulse msg_header_t;

typedef struct messageBtwComSysAndOperator {
    msg_header_t hdr;
    int aireplaneID;
    char mymessage[4096];
    string option;
    int value;
    string index;
} msgBtwComOpr;
int client(string message,string airecraft,string opt,string index, string Vindex) {
	msgBtwComOpr msg;
    int server_coid; //server connection ID.

    if ((server_coid = name_open(ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_OPERATOR_CONSOLE, 0)) == -1) {
        return EXIT_FAILURE;
    }

    msg.hdr.type = 0x00;
    msg.hdr.subtype = 0x01;
    strcpy(msg.mymessage,message.c_str());
    msg.option=opt;
    msg.index=index;
    msg.value=stoi(Vindex);
    msg.aireplaneID=stoi(airecraft);
    MsgSend(server_coid, &msg, sizeof(msg), NULL, 0);
    name_close(server_coid);
    return EXIT_SUCCESS;
}


int main(int argc, char **argv) {
	int ret;
	while(1){
	string message,airecraft,opt="";
	string index,Vindex,Pindex,VPindex;
	cerr<<"*********** Write Your Message ***********"<<"\n";
	    cerr<<"Your Input:"<<"\n";
	    getline(cin,message);
	cerr<<"***********What Kind of Information Do You Want to Change?***********"<<"\n";
	cerr<<"    1) Speed  2)Degree    " <<"\n";
	while(1){
	    cerr<<"Your Input:"<<"\n";
	    cin>>opt;
	    if(opt!="1" && opt!="2"){
	        cerr<<"Please Enter Valid Number.\n";
	    }
	    else{
	        if(opt=="1"){
	        	cerr<<"*********** Which Speed Do You Want To Change ?***********"<<"\n";
	        	cerr<<"    1) Vx  2)Vy    "<<"\n";
	        	cerr<<"Your Input:"<<"\n";
	        	cin >> index;
	        	if(index=="1") index="Vx";
	        	else index="Vy";
	        	cerr<<"*********** Enter New Value ***********"<<"\n";
	        	cerr<<"Your Input:"<<"\n";
	        	cin >> Vindex;
	        }
	        else{
	        	cerr<<"*********** Which Deg Do You Want ?***********"<<"\n";
	        	cerr<<"Your Input:"<<"\n";
	        	cin >> Vindex;
	        }
	        break;
	    }
	}
		cerr<<"***********Please Select The Airplane***********"<<"\n";
		cerr<<"Your Input:"<<"\n";
		cin>>airecraft;
//	printf("Running Client ... \n");
	ret = client(message,airecraft,opt,index,Vindex);
	}
}
