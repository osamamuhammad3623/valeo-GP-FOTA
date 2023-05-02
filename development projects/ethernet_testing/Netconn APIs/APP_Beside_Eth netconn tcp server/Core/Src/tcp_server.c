/*
 * tcp_server.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

//=======================================================
//================ Includes
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"

#include "tcp_server.h"
#include "string.h"
//===============================================================
//======== netconn global parameters
static struct netconn *conn, *newconn;
static struct netbuf *buf;
static struct netbuf *buf2;

//============================================================

//============ data structures used in send and receive
char msg[100];
char * data = (char*)0x10000000;
char smsg[200];
//==============================================================================

/**** Send RESPONSE every time the client sends some data ******/

void UDS_execute_response(char* message)
{
	if(strcmp(message,"hi")==0)
	{
		int len = sprintf (smsg, "\nhi from the server ");
		tcp_SendResponse(smsg , len);
	}
	else if(strcmp(message,"delay")==0)
	{
		for(int i=0;i<1000;i++);

		int len = sprintf (smsg, "\nProcess done");
		tcp_SendResponse(smsg , len);
	}
	else if(strcmp(message,"store")==0)
	{
		int len = sprintf (smsg, "\nsend program length");
		tcp_SendResponse(smsg , len);
		if(netconn_recv(newconn, &buf) == ERR_OK)
		{
			int programLength = atoi(buf->p->payload);
			netbuf_delete(buf);
			tcp_RecievePrograme(programLength);
		}
	}
	else
	{
	}
}
void tcpserver_init (void )
{
	err_t err, accept_err;

	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	if (conn!=NULL)
	{
		/* Bind connection to the port number 10. */
		err = netconn_bind(conn, IP_ADDR_ANY, 10);

		if (err == ERR_OK)
		{
			/* Tell connection to go into listening mode. */
			netconn_listen(conn);

			//			while (1)
			//			{
			//				/* Grab new connection. */
			//				//while(netconn_accept(conn, &newconn) !=ERR_OK);
			//				accept_err = netconn_accept(conn, &newconn);
			//
			//				/* Process the new connection. */
			//				if (accept_err == ERR_OK)
			//				{
			//
			//					/* receive the data from the client */
			//					if (netconn_recv(newconn, &buf) == ERR_OK)
			//					{
			//						/* If there is some data remaining to be sent, the following process will continue */
			//						do
			//						{
			//							strncpy(msg,buf->p->payload,buf->p->len);
			//							UDS_execute_response(msg);
			//						}
			//						while (netbuf_next(buf) >=0);
			//
			//						netbuf_delete(buf);
			//					}
			//
			//					/* Close connection and discard connection identifier. */
			//					netconn_close(newconn);
			//					netconn_delete(newconn);
			//				}
			//				osDelay(100);
			//
			//			}

			while(1)
			{
				// Accept a new connection
				err = netconn_accept(conn, &newconn);
				if (err == ERR_OK) {
					// Send a message to the client
					while (netconn_recv(newconn, &buf) == ERR_OK)
					{
						/* If there is some data remaining to be sent, the following process will continue */
						do
						{
							strncpy(msg,buf->p->payload,buf->p->len);
							UDS_execute_response(msg);
						}
						while (netbuf_next(buf) >=0);
						memset(msg, 0,100);
						netbuf_delete(buf);
					//	vTaskDelay(100);
					}

					// Close the connection
					netconn_close(newconn);
					netconn_delete(newconn);
				}
			}
		}
		else
		{
			netconn_delete(conn);
		}
	}
}

void tcp_RecievePrograme(int length)
{
	int len = sprintf (smsg, "/nstart Sending\n");
	tcp_SendResponse(smsg,len);
	int i=0;
//	while (i <length)
//	{
		if(netconn_recv(newconn, &buf2) == ERR_OK)
		{
			/* If there is some data remaining to be sent, the following process will continue */
			do
			{
				//To store data
				strncpy((data+i)  ,buf2->p->payload, buf->p->len );
				i += buf2->p->len;
			}
			while (netbuf_next(buf2) >=0);
			netbuf_delete(buf2);
		//}

	}
	len = sprintf (smsg, "Done, Length = %d\n",length);
	tcp_SendResponse(smsg,len);
}

void tcp_SendResponse(char *data , int len)
{
	netconn_write(newconn, data, len, NETCONN_COPY);
}


//void tcpserver_init(void)
//{
//	sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
//}
