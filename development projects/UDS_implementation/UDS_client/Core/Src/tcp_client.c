/*
 * tcp_client.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Sawiris
 */

#include "tcp_client.h"
#include "string.h"

//====================================================================
void (*uds_req_clbk) (TargetECU targetECU);
void (*uds_recv_resp_clbk) (TargetECU targetECU, uint8_t *responseFrame);

extern uint8_t target1_version_received;
osThreadId_t target1ThreadID;
osThreadId_t target2ThreadID;
extern osThreadId_t UartTaskHandle;
extern osThreadId_t InstallTaskHandle;
extern uint8_t uartTerminated;
//====================================================================
static struct netconn *conn1;
static struct netconn *conn2;

struct target_confg target_1;
struct target_confg target_2;

//====================================================================
void init_uds_request_callback(void (*p) (TargetECU targetECU)) {
	uds_req_clbk = p;
}

void init_uds_recv_resp_clbk(void (*p)(TargetECU targetECU, uint8_t *responseFrame)) {
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
	osThreadId_t targetThreadID;
	static int my_port_iterative = 0;
	TickType_t backoff;


	// Extract the IP address, port number and the target ECU
	char* ip_address = malloc(strlen(config->ip_add) + 1); // allocate memory for the string
	strcpy(ip_address, config->ip_add); // copy the string into the new buffer
	ip_address[strlen(config->ip_add)] = '\0'; // add null terminate
	unsigned short dest_port = config->portNum;

	TargetECU target_ECU = config->targetECU;

	/* Create a new connection identifier. */
	if (target_ECU == PS_TARGET) {
		conn1 = netconn_new(NETCONN_TCP);
		conn = conn1;
		targetThreadID = target1ThreadID;
	} else {
		conn2 = netconn_new(NETCONN_TCP);
		conn = conn2;
		targetThreadID = target2ThreadID;
	}

	ipaddr_aton(ip_address, &dest_addr);

	while(1)
	{
		//=====================First attemp to connect ================================

		backoff = 2000 ; //used as initial delay value
		/* Create a new connection identifier. */
		if (target_ECU == PS_TARGET) {
		conn1 = netconn_new(NETCONN_TCP);
		conn = conn1;
		targetThreadID = target1ThreadID;
		} else {
		conn2 = netconn_new(NETCONN_TCP);
		conn = conn2;
		targetThreadID = target2ThreadID;
		}

		if(conn != NULL)
		{
			/* Bind connection to the port number 10 (port of the Client). */
			err = netconn_bind(conn, IP_ADDR_ANY, dest_port+my_port_iterative);
			if (err == ERR_OK)
			{
				//suspend UART task to start connection
				if (osThreadGetState(UartTaskHandle) != osThreadTerminated) {
					osThreadSuspend(UartTaskHandle);
				}
				connect_error = netconn_connect(conn, &dest_addr, dest_port);/* Connect to the TCP Server */
			}
		}

		//==========================================================================================
		//if First Attempt to connection is Faild Try to connect again based on Back off algorithm
		while(connect_error != ERR_OK)
		{
			my_port_iterative++;
			osDelay(backoff);
			//delete conn
			if (conn != NULL) {
			    netconn_delete(conn);
			}
			conn = NULL;

			//make new tcp connection
			if (target_ECU == PS_TARGET) {
			conn1 = netconn_new(NETCONN_TCP);
			conn = conn1;
			} else {
			conn2 = netconn_new(NETCONN_TCP);
			conn = conn2;
			}

			if(conn != NULL)
			{
				/* Bind connection to the port number 10 (port of the Client). */
				err = netconn_bind(conn, IP_ADDR_ANY, dest_port+my_port_iterative);
				if (err == ERR_OK)
				{
					connect_error = netconn_connect(conn, &dest_addr, dest_port);/* Connect to the TCP Server */
				}
			}

			if(backoff < MAX_BACKOFF_MS) // double the delay value between attemptions to connect based on backoff algorithm
			backoff *=2;
		}
		//==================================================================
		//============= Connection is Successfully established
		/* Blink LED to indicate successful connection */
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);

		//make target task priority the same as that of UART task
		osThreadSetPriority(targetThreadID, osPriorityNormal1);

		// UDS_req callback
		uds_req_clbk(target_ECU);

		//start receiving
		tcp_ReceiveMessage(target_ECU, conn, buf);

		//================================================
		//== if there was error in the connection it was closed it will get to this section
		//===========================================================================

		//close the connection to retry to connect to it again

		/* Close connection and discard connection identifier. */
		netconn_close(conn);
		netconn_delete(conn);
		osDelay(5000);
		my_port_iterative++;
	}



//	if (conn!=NULL) // conn
//	{
//		/* Bind connection to the port number 10 (port of the Client). */
//		err = netconn_bind(conn, IP_ADDR_ANY, dest_port+my_port_iterative); //////////
//
//
//		while(connect_error != ERR_OK)
//		{
//			osDelay(backoff);
//			my_port_iterative++;
//
//			//delete conn
//			netconn_delete(conn);
//			conn = netconn_new(NETCONN_TCP);//make new tcp connection
//
//			if (conn!= NULL)
//			{
//				err = netconn_bind(conn, IP_ADDR_ANY, dest_port + my_port_iterative);/* Bind connection to the port number 10 (port of the Client). */
//				if (err == ERR_OK)
//				{
//					IP_ADDR4(&dest_addr, 192, 168, 1, 3);/* The designation IP address */
//					dest_port = 10;  // server port
//					connect_error = netconn_connect(conn, &dest_addr, dest_port);/* Connect to the TCP Server */
//				}
//			}
//			if(backoff < MAX_BACKOFF_MS)
//				backoff *=2;
//		}




//		if (err == ERR_OK)
//		{
//			/* The designation IP address of the computer */
//			err = ipaddr_aton(ip_address, &dest_addr);
//			//dest_port = 10;  // server port
//
//			//suspend UART task to start connection
//			osThreadSuspend(UartTaskHandle);
//
//			/* Connect to the TCP Server */
//			connect_error = netconn_connect(conn, &dest_addr, dest_port);
//
//			// If the connection to the server is established, the following will continue, else delete the connection
//			if (connect_error == ERR_OK)
//			{
//				/* Blink LED to indicate connection is successful */
//				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_SET);
//				HAL_Delay(500);
//				HAL_GPIO_WritePin(LD1_GPIO_Port, LD1_Pin, GPIO_PIN_RESET);
//
//				//make target task priority the same as that of UART task
//				osThreadSetPriority(targetThreadID, osPriorityNormal1);
//
//				// UDS_req callback
//				uds_req_clbk(target_ECU);
//
//				//start receiving
//				tcp_ReceiveMessage(target_ECU, conn, buf);
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

				if(target1_version_received) {
					target1_version_received = 0;
					osThreadTerminate(target1ThreadID); // will server know the connection has closed?
				}
			}
			while (netbuf_next(buf) >0);

			netbuf_delete(buf);
		} else {
			break;///////
		}
	}
}

void tcpclient_init (uint8_t* targetToConnectWith)
{
	if (targetToConnectWith[1]) {
		target_1.ip_add = "169.254.84.57";	//58
		target_1.portNum = 10;
		target_1.targetECU = PS_TARGET;

		target1ThreadID = sys_thread_new("tcpinit_thread", tcpinit_thread, (void*)&target_1, DEFAULT_THREAD_STACKSIZE,osPriorityNormal2);
	}
	if (targetToConnectWith[2]) {
		target_2.ip_add = "169.254.84.57";
		target_2.portNum = 7;
		target_2.targetECU = WIPERS_TARGET;

		target2ThreadID = sys_thread_new("tcpinit_thread", tcpinit_thread, (void*)&target_2, DEFAULT_THREAD_STACKSIZE,osPriorityNormal2);
	}
}
