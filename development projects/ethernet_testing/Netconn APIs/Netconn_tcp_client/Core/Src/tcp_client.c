/*
 * tcp_client.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Sawiris
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
static ip_addr_t  dest_addr;
static unsigned short  dest_port;
//=========================================================================

//==================ARRAYS to Hold DATA===================================
char ReceivedMessage[100];
char ToSendMessage[100];
char* ProgramToSend = (char*)0x10000000;
//====================================================================

//===================static functions used ===========================

// Function to send the data to the server
static void tcp_ReseveMessage (void );
void tcp_SendMessage (char *Message , int messageLength);

//==========================================================

void UDS_receive_response(char* message)
{

	memset (ToSendMessage, '\0', 100);

	if(strcmp(message,"hi")==0)
	{
		int len = sprintf (ToSendMessage, "\n pleased to meet you ");
		tcp_SendMessage(ToSendMessage , len);
	}
	else if(strcmp(message,"delay")==0)
	{
		for(int i=0;i<1000;i++);

		int len = sprintf (ToSendMessage, "\n Process done");
		tcp_SendMessage(ToSendMessage , len);
	}
	else if(strcmp(message,"send")==0)
	{
		int len = sprintf (ToSendMessage, "\n i will send first the program length");
		tcp_SendMessage(ToSendMessage , len);

		 len = sprintf (ToSendMessage, "\n 3000");
		 tcp_SendMessage(ToSendMessage , len);

		 len = sprintf (ToSendMessage, "\n Be ready to receive the program /n/n");
		 tcp_SendMessage(ToSendMessage , len);

		 tcp_SendMessage(ProgramToSend , 3000);

	}
}

static void tcpinit_thread(void *arg)
{
	err_t err, connect_error;

	/* Create a new connection identifier. */
	conn = netconn_new(NETCONN_TCP);

	if (conn!=NULL)
	{
		/* Bind connection to the port number 10 (port of the Client). */
		err = netconn_bind(conn, IP_ADDR_ANY, 10);

		if (err == ERR_OK)
		{
			/* The designation IP address of the computer */
			IP_ADDR4(&dest_addr, 169, 254, 84, 57);
			dest_port = 10;  // server port

			/* Connect to the TCP Server */
			connect_error = netconn_connect(conn, &dest_addr, dest_port);

			// If the connection to the server is established, the following will continue, else delete the connection
			if (connect_error == ERR_OK)
			{
				//send a "hi" message at first
				int messageLength = sprintf(ToSendMessage , "hi");
				tcp_SendMessage(ToSendMessage, messageLength);

				//start receiving
				tcp_ReseveMessage();
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

}

 void tcp_SendMessage (char *Message , int messageLength)
{
	netconn_write(conn, Message, messageLength, NETCONN_COPY);
}


 static void tcp_ReseveMessage (void)
{

	while (1)
	{
		/* wait until the data is sent by the server */
		if (netconn_recv(conn, &buf) == ERR_OK)
		{
			/* If there is some data remaining to be sent, the following process will continue */
			do
			{
				memset (ReceivedMessage, '\0', 100);
				strncpy(ReceivedMessage,buf->p->payload, buf->p->len);
				UDS_receive_response(ReceivedMessage);
			}
			while (netbuf_next(buf) >0);

			netbuf_delete(buf);
		}
	}
}


void tcpclient_init (void)
{
	for(int i=0;i<3000;i++)
	{
		ProgramToSend[i] = 'a';
	}

	sys_thread_new("tcpinit_thread", tcpinit_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);

}
