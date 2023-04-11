/*
 * tcp_client.c
 *
 *  Created on: Mar 6, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */


#include "lwip/opt.h"

#include "lwip/api.h"
#include "lwip/sys.h"

#include "tcp_client.h"
#include "string.h"

//=============================================================================

//============netconn parameters=============================================
static struct netconn *conn;
static struct netbuf *buf;
static ip_addr_t *addr, dest_addr;
static unsigned short port, dest_port;
//=========================================================================

//==================ARRAYS to Hold DATA===================================
char ReceivedMessage[100];
char ToSendMessage[200];
char ProgramToSend [10000];
extern uint8_t data_received[6516];
//====================================================================

//===================static functions used ===========================

// Function to send the data to the server
static void tcpsend (char *data);
static void tcp_ReseveMessage (void *arg );
static void tcp_SendMessage (void *arg);
static void tcp_SendProgram (void *arg);


//==========================================================

static void tcpinit_thread(void *arg)
{
	err_t err, connect_error;

	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	if (conn!=NULL)
	{
		/* Bind connection to the port number 7 (port of the Client). */
		err = netconn_bind(conn, IP_ADDR_ANY, 7);

		if (err == ERR_OK)
		{
			/* The desination IP adress of the computer */
			IP_ADDR4(&dest_addr, 169, 254, 84, 57);
			dest_port = 10;  // server port

			/* Connect to the TCP Server */
			connect_error = netconn_connect(conn, &dest_addr, dest_port);

			// If the connection to the server is established, the following will continue, else delete the connection
			if (connect_error == ERR_OK)
			{
				//sys_thread_new("tcp_ReseveMessage", tcp_ReseveMessage, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);

				sys_thread_new("tcp_SendProgram", tcp_SendProgram, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);

			}
			else
			{
				/* Close connection and discard connection identifier. */
				netconn_close(conn);
				netconn_delete(conn);
			}
		}
		else
		{
			// if the binding wasn't successful, delete the netconn connection
			netconn_delete(conn);
		}
	}

	osThreadId_t id;
	id = osThreadGetId();
	osThreadTerminate(id);
}

void tcpsend (char *data)
{
	netconn_write(conn, data, 6516, NETCONN_COPY);
}


static void tcp_SendProgram (void *arg)
{

	tcpsend(data_received);
	osThreadId_t id;
	id = osThreadGetId();
	osThreadTerminate(id);
}


static void tcp_SendMessage (void *arg)
{
	sprintf (ToSendMessage, "\"%s\" was sent by the Server\n", ReceivedMessage);
	tcpsend(ToSendMessage);
	memset (ToSendMessage, '\0', 200);  // clear the buffer

	osThreadId_t id;
	id = osThreadGetId();
	osThreadTerminate(id);
}

void tcp_ReseveMessage (void *arg )
{

	while (1)
	{
		/* wait until the data is sent by the server */
		if (netconn_recv(conn, &buf) == ERR_OK)
		{
			/* Extract the address and port in case they are required */
			addr = netbuf_fromaddr(buf);  // get the address of the client
			port = netbuf_fromport(buf);  // get the Port of the client

			/* If there is some data remaining to be sent, the following process will continue */
			do
			{

				strncpy (ReceivedMessage, buf->p->payload, buf->p->len);   // get the message from the server

				if(strcmp(ReceivedMessage , "SEND")==0)
				{
					sys_thread_new("tcp_SendProgram", tcp_SendProgram, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);

				}
				else
				{
					// send the received data  using tcp_SendMessage
					sys_thread_new("tcp_SendMessage", tcp_SendMessage, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
				}

				//memset (ReceivedMessage, '\0', 100);  // clear the buffer

			}
			while (netbuf_next(buf) >0);

			netbuf_delete(buf);
		}
	}
}


void tcpclient_init (void)
{
//	for(int i =0;i<10000;i++)
//	{
//		ProgramToSend[i]='k';
//	}
	sys_thread_new("tcpinit_thread", tcpinit_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);

	osThreadId_t id;
	id = osThreadGetId();
	osThreadTerminate(id);

}
