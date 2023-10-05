#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dispatch.h>

#include <iostream>
using namespace std;
#define ATTACH_POINT_FOR_SERVER_OF_COMMINUCATION_AND_RADAR "server_client_CominuSys_Radar"
#define ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_COMMUNICATION "server_client_ComSys_Communication"

typedef struct _pulse msg_header_t;

typedef struct comminucationMSG {
    msg_header_t hdr;
    int aireplaneID;
    char mymessage[4096];
    string option;
    int value;
    string index;
} comminucationMSG_t;
int client(string message,int airecraft,string opt,string index, int Vindex) {
	comminucationMSG_t msg;
    int server_coid; //server connection ID.

    if ((server_coid = name_open(ATTACH_POINT_FOR_SERVER_OF_COMMINUCATION_AND_RADAR, 0)) == -1) {
        return EXIT_FAILURE;
    }

    msg.hdr.type = 0x00;
    msg.hdr.subtype = 0x01;
    strcpy(msg.mymessage,message.c_str());
    msg.option=opt;
    msg.index=index;
    msg.value=Vindex;
    msg.aireplaneID=airecraft;
    MsgSend(server_coid, &msg, sizeof(msg), NULL, 0);
    name_close(server_coid);
    return 0;
}
int CommunicationSys2ComputerSysServer() {
   name_attach_t *attach;
   comminucationMSG_t msg;
   int rcvid;

   /* Create a local name (/dev/name/local/...) */
   if ((attach = name_attach(NULL, ATTACH_POINT_FOR_SERVER_OF_COMPUTER_AND_COMMUNICATION, 0)) == NULL) {
       return EXIT_FAILURE;
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
	    	  int ret;
	    	  ret = client(msg.mymessage,msg.aireplaneID,msg.option,msg.index,msg.value);
	      }
	   }

       MsgReply(rcvid, EOK, 0, 0);

   }

   /* Remove the name from the space */
   name_detach(attach, 0);

   return EXIT_SUCCESS;
}
int main(int argc, char **argv) {
	CommunicationSys2ComputerSysServer();
    return 0;
}
