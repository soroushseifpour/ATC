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
#define ATTACH_POINT_CompSys_DataDisplay "server_client_ComSys_DataDisplay"
typedef struct _pulse msg_header_t;

using namespace std;

typedef struct _pulse msg_header_t;



/* Our real data comes after the header */
typedef struct _my_data {
    msg_header_t hdr;
    int data;
    char mymessage[1024];
} my_data_t;

/*** Server Side of the code ***/
int server() {
   name_attach_t *attach;
   my_data_t msg;
   int rcvid;

   if ((attach = name_attach(NULL, ATTACH_POINT_CompSys_DataDisplay, 0)) == NULL) {
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
	    	  printf("Server receive %s \n", msg.mymessage);

	      }
	   }
       MsgReply(rcvid, EOK, 0, 0);

   }
   name_detach(attach, 0);

   return EXIT_SUCCESS;
}
int main(int argc, char **argv) {
    int ret;
	printf("Running Server ... \n");
	ret = server();   /* see name_attach() for this code */
    return ret;
}
