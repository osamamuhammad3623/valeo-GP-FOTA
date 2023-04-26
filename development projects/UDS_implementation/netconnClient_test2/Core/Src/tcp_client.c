///*
// * tcp_client.c
// *
// *  Created on: Feb 24, 2023
// *      Author: Kyrillos Sawiris
// */
//
//#include "tcp_client.h"
//#include "string.h"
//
////==================ARRAYS to Hold DATA===================================
//char ReceivedMessage[100];
//char ToSendMessage[100];
//char* ProgramToSend = (char*)0x10000000;
////====================================================================
//struct target_confg target_1;
//struct target_confg target_2;
//
//
//
////==========================================================
//
//
//static void tcpinit_thread(void *arg)
//{
//
//	// Cast the argument to the correct type
//	if(arg == NULL)
//		return ;
//	struct target_confg *config = (struct target_confg*)arg;
//	struct netconn *conn;
//	struct netbuf *buf;
//	err_t err, connect_error;
//	ip_addr_t dest_addr;
//
//
//	// Extract the IP address and port number
//	char* ip_address = malloc(strlen(config->ip_add) + 1); // allocate memory for the string
//	strcpy(ip_address, config->ip_add); // copy the string into the new buffer
//	ip_address[strlen(config->ip_add)] = '\0'; // add null terminat
//	unsigned short dest_port = config->portNum;
//
//	/* Create a new connection identifier. */
//	conn = netconn_new(NETCONN_TCP);
//
//	if (conn!=NULL)
//	{
//		/* Bind connection to the port number bind to the same port as the server. */
//		err = netconn_bind(conn, IP_ADDR_ANY, dest_port);
//
//		if (err == ERR_OK)
//		{
//			/* The designation IP address of the computer */
//			err = ipaddr_aton(ip_address, &dest_addr);
//			//dest_port = 10;  // server port
//
//			/* Connect to the TCP Server */
//			connect_error = netconn_connect(conn, &dest_addr, dest_port);
//
//			// If the connection to the server is established, the following will continue, else delete the connection
//			if (connect_error == ERR_OK)
//			{
//				//send a "hi" message at first
//				int messageLength = sprintf(ToSendMessage , "hi");
//				tcp_SendMessage(conn,ToSendMessage, messageLength);
//
//				//start receiving
//				tcp_ReseveMessage(conn, buf);
//			}
//			else
//			{
//				/* Close connection and discard connection identifier. */
//				netconn_close(conn);
//				netconn_delete(conn);
//			}
//		}
//		else
//		{
//			// if the binding wasn't successful, delete the netconn connection
//			netconn_delete(conn);
//		}
//	}
//
//}
//
//void tcp_SendMessage (struct netconn *conn, char *Message , int messageLength)
//{
//	netconn_write(conn, Message, messageLength, NETCONN_COPY);
//}
//
//
//static void tcp_ReseveMessage (struct netconn *conn ,struct netbuf *buf )
//{
//
//	while (1)
//	{
//		/* wait until the data is sent by the server */
//		if (netconn_recv(conn, &buf) == ERR_OK)
//		{
//			/* If there is some data remaining to be sent, the following process will continue */
//			do
//			{
//				memset (ReceivedMessage, '\0', 100);
//				strncpy(ReceivedMessage,buf->p->payload, buf->p->len);
//				UDS_receive_response(conn,ReceivedMessage);
//			}
//			while (netbuf_next(buf) >0);
//
//			netbuf_delete(buf);
//		}
//	}
//}
//
//
//void UDS_receive_response(struct netconn *conn,char* message)
//{
//
//	memset (ToSendMessage, '\0', 100);
//
//	if(strcmp(message,"hi")==0)
//	{
//		int len = sprintf (ToSendMessage, "\n pleased to meet you ");
//		tcp_SendMessage(conn,ToSendMessage , len);
//	}
//	else if(strcmp(message,"delay")==0)
//	{
//		//for(int i=0;i<1000;i++);
//
//		int len = sprintf (ToSendMessage, "\n about to switch to another task");
//		tcp_SendMessage(conn,ToSendMessage , len);
//		sys_thread_new("tcpinit_thread2", tcpinit_thread, (void*)&target_2, DEFAULT_THREAD_STACKSIZE,osPriorityAboveNormal);
//
//	}
//	else if(strcmp(message,"send")==0)
//	{
//		int len = sprintf (ToSendMessage, "\n i will send first the program length");
//		tcp_SendMessage(conn,ToSendMessage , len);
//
//		len = sprintf (ToSendMessage, "\n 3000");
//		tcp_SendMessage(conn,ToSendMessage , len);
//
//		len = sprintf (ToSendMessage, "\n Be ready to receive the program /n/n");
//		tcp_SendMessage(conn,ToSendMessage , len);
//
//		tcp_SendMessage(conn,ProgramToSend , 10000);
//
//	}
//}
//
//
//
//void tcpclient_init (void)
//{
//	for(int i=0;i<10000;i++)
//	{
//		ProgramToSend[i] = 'a';
//	}
//
//	target_1.ip_add = "169.254.84.57";
//	target_1.portNum = 10;
//
//	target_2.ip_add = "169.254.84.57";
//	target_2.portNum = 7;
//
//
//	sys_thread_new("tcpinit_thread", tcpinit_thread, (void*)&target_1, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
//
//}


/*
 * tcp_client.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Sawiris
 */

#include "tcp_client.h"
#include "string.h"

//==================ARRAYS to Hold DATA===================================
char ToSendMessage[100];
char* ProgramToSend = (char*)0x10000000;

void (*uds_req_clbk) (TargetECU targetECU);
void (*uds_recv_resp_clbk) (TargetECU targetECU, void * arg);
//====================================================================
static struct netconn *conn1;
static struct netconn *conn2;

struct target_confg target_1;
struct target_confg target_2;
//==========================================================

void init_uds_request_callback(void (*p) (TargetECU targetECU)) {
	uds_req_clbk = p;
}

void init_uds_recv_resp_clbk(void (*p)(TargetECU targetECU, void *arg)) {
	uds_recv_resp_clbk = p;
}

static void tcpinit_thread(void *arg)
{
	// Cast the argument to the correct type
	if(arg == NULL)
		return;
	struct target_confg *config = (struct target_confg*)arg;
	struct netconn *conn;
	struct netbuf *buf;
	err_t err, connect_error;
	ip_addr_t dest_addr;


	// Extract the IP address, port number and the target ECU
	char* ip_address = malloc(strlen(config->ip_add) + 1); // allocate memory for the string
	strcpy(ip_address, config->ip_add); // copy the string into the new buffer
	ip_address[strlen(config->ip_add)] = '\0'; // add null terminat
	unsigned short dest_port = config->portNum;

	TargetECU target_ECU = config->targetECU;

	/* Create a new connection identifier. */
	if (target_ECU == PS_TARGET) {
		conn1 = netconn_new(NETCONN_TCP);
		conn = conn1;
	} else {
		conn2 = netconn_new(NETCONN_TCP);
		conn = conn2;
	}

	if (conn!=NULL) // conn
	{
		/* Bind connection to the port number 10 (port of the Client). */
		err = netconn_bind(conn, IP_ADDR_ANY, dest_port);

		if (err == ERR_OK)
		{
			/* The designation IP address of the computer */
			err = ipaddr_aton(ip_address, &dest_addr);
			//dest_port = 10;  // server port

			/* Connect to the TCP Server */
			connect_error = netconn_connect(conn, &dest_addr, dest_port);

			// If the connection to the server is established, the following will continue, else delete the connection
			if (connect_error == ERR_OK)
			{
				//send a "hi" message at first
				//int messageLength = sprintf(ToSendMessage , "hi");
				//tcp_SendMessage(target_ECU,(uint8_t *)ToSendMessage, messageLength);

				// UDS_req callback
				uds_req_clbk(target_ECU);

				//start receiving
				tcp_ReceiveMessage(target_ECU, conn, buf);
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

void tcp_SendMessage (TargetECU targetECU, uint8_t *Message , int messageLength)
{
	switch (targetECU) {
	case PS_TARGET:
		netconn_write(conn1, Message, messageLength, NETCONN_COPY);
		break;
	case WIPERS_TARGET:
		netconn_write(conn2, Message, messageLength, NETCONN_COPY);
		break;
	default: break;
	}
}


static void tcp_ReceiveMessage (TargetECU targetECU, struct netconn *conn ,struct netbuf *buf )
{

	while (1)
	{
		/* wait until the data is sent by the server */
		if (netconn_recv(conn, &buf) == ERR_OK)
		{
			/* If there is some data remaining to be sent, the following process will continue */
			do
			{
				uint8_t ReceivedMessage[100];
				memset ((char *)ReceivedMessage, '\0', 100);
				strncpy((char *)ReceivedMessage,buf->p->payload, buf->p->len);

				// uds receive response callback
				uds_recv_resp_clbk(targetECU, ReceivedMessage);
			}
			while (netbuf_next(buf) >0);

			netbuf_delete(buf);
		}
	}
}


void tcpclient_init (uint8_t targetToConnectWith)
{
	if (targetToConnectWith&0x02) {
		target_1.ip_add = "169.254.84.57"; // 169.254.84.58
		target_1.portNum = 10;
		target_1.targetECU = PS_TARGET;

		sys_thread_new("tcpinit_thread", tcpinit_thread, (void*)&target_1, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
	}
	if (targetToConnectWith&0x04) {
		target_2.ip_add = "169.254.84.57";
		target_2.portNum = 7;
		target_2.targetECU = WIPERS_TARGET;

		sys_thread_new("tcpinit_thread", tcpinit_thread, (void*)&target_2, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
	}
}