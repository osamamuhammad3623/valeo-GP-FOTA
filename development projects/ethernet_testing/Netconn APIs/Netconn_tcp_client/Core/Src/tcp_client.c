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

void tcp_SendMessage (char *Message , int messageLength)
{
	err_t err;
	err = netconn_write(conn, Message, messageLength, NETCONN_COPY);
	printf("Receive Error -->ERR = %d",err);

}

static void tcp_ReseveMessage (void)
{
	err_t err;

	while (1)
	{
		/* wait until the data is sent by the server */
		err = netconn_recv(conn, &buf);
		printf("Receive Error -->ERR = %d",err);

		if(err == ERR_OK)
		{
			/* If there is some data remaining to be sent, the following process will continue */
			do
			{
				memset (ReceivedMessage, '\0', 100);
				strncpy(ReceivedMessage,buf->p->payload, buf->p->len);
				UDS_receive_response(ReceivedMessage);
			}
			while (netbuf_next(buf) >=0);

			netbuf_delete(buf);
		}
		else if(err == ERR_CLSD)
			break;
	}
}

//
//
//
//
//
//
//
void netconn_tcp_client_task(void *pvParameters)
{
	ip_addr_t server_ip;
	err_t err;
	const TickType_t xTicksToWait = pdMS_TO_TICKS(5000); // 1 second timeout

	// Set up the server IP address
	IP4_ADDR(&server_ip, 169, 254, 84, 57);

	while (1)
	{
		// Create a new Netconn connection object
		conn = netconn_new(NETCONN_TCP);

		/* Bind connection to the port number 10 (port of the Client). */
		netconn_bind(conn, IP_ADDR_ANY, 10);

		// Connect to the remote server
		err = netconn_connect(conn, &server_ip, 10);

		while (err != ERR_OK)
		{
			netconn_delete(conn);
			conn = NULL; // Reset the connection object
			if (conn == NULL) {
				// Create a new Netconn connection object
				conn = netconn_new(NETCONN_TCP);
				netconn_bind(conn, IP_ADDR_ANY, 10);
			}
			vTaskDelay(xTicksToWait); // Wait for 1 second before attempting to reconnect
			err = netconn_connect(conn, &server_ip, 10);
		}

		// send a "hi" message at first
		int messageLength = sprintf(ToSendMessage , "hi");
		tcp_SendMessage(ToSendMessage, messageLength);

		//start receiving
		tcp_ReseveMessage();

		//it will reach here when the connection is closed
		netconn_close(conn);
		netconn_delete(conn);
		conn = NULL; // Reset the connection object
		vTaskDelay(xTicksToWait); // Wait for 1 second before attempting to reconnect
	}
}


void tcpclient_init (void)
{
	for(int i=0;i<3000;i++)
	{
		ProgramToSend[i] = 'a';
	}

	sys_thread_new("tcpinit_thread", netconn_tcp_client_task, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);

}
