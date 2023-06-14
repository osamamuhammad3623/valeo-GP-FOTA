/*
 * tcp_client.c
 *
 *  Created on: JUN 16, 2023
 *      Author: Kyrillos Sawiris
 */

#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "tcp_client.h"
#include "string.h"


#define MAX_BACKOFF_MS 50000
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
	static int i = 0;
	 TickType_t backoff  ; // its value used as a delay that its value is doubled after each attempt
	dest_port = 10;  // server port
	unsigned short my_port = 50;
	static unsigned short my_port_iterative = 0; // to iterate to change the my port after each connection attemp

	while(1)
	{
		//First Try to connect
		backoff = 2000 ;
		conn = NULL;
		conn = netconn_new(NETCONN_TCP);//make new tcp connection

		if (conn!=NULL)
		{
			err = netconn_bind(conn, IP_ADDR_ANY, my_port + my_port_iterative);/* Bind connection to the port number 10 (port of the Client). */
			if (err == ERR_OK)
			{
				IP_ADDR4(&dest_addr, 192, 168, 1, 3);/* The designation IP address */
				dest_port = 10;  // server port
				connect_error = netconn_connect(conn, &dest_addr, dest_port);/* Connect to the TCP Server */
			}
		}
//===========================================================================
//== execute this loop if the first attemp failed to establish a connection

		while(connect_error != ERR_OK)
		{
			osDelay(backoff);
			i++;

			//delete conn
			netconn_delete(conn);
			conn = netconn_new(NETCONN_TCP);//make new tcp connection

			if (conn!= NULL)
			{
				err = netconn_bind(conn, IP_ADDR_ANY, my_port + my_port_iterative);/* Bind connection to the port number 10 (port of the Client). */
				if (err == ERR_OK)
				{
					IP_ADDR4(&dest_addr, 192, 168, 1, 3);/* The designation IP address */
					dest_port = 10;  // server port
					connect_error = netconn_connect(conn, &dest_addr, dest_port);/* Connect to the TCP Server */
				}
			}
			if(backoff < MAX_BACKOFF_MS)
			backoff *=2;
		}
//==========================================================================
//====== this section is reached when the connection is established successfully

		HAL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin); // to indicate that the connection is established
		HAL_Delay(100);

		//send a "hi" message at first
		int messageLength = sprintf(ToSendMessage , "hi");
		err = tcp_SendMessage(ToSendMessage, messageLength);
		if(err == ERR_OK)
		{
			//start receiving
			tcp_ReseveMessage();
		}

//=================================================================
// this section reached after the connection is closed
//====================================================================

		/* Close connection and discard connection identifier. */
		netconn_close(conn);
		netconn_delete(conn);
		my_port_iterative ++;
		osDelay(5000); //Delay after the connection is closed
//======================================================================

	}

}



err_t tcp_SendMessage (char *Message , int messageLength)
{
	err_t err;
	err = netconn_write(conn, Message, messageLength, NETCONN_COPY);
	return err;
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

				//to indicate that the a message is received
				HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
				HAL_Delay(100);
			}
			while (netbuf_next(buf) >=0);
			netbuf_delete(buf);
		}
		else
		{
			// to indicate that the connection is closed
			HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
			HAL_Delay(100);
			break;
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

