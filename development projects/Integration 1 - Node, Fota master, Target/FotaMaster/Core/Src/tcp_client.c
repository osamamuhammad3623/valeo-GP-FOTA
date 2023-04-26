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
//static unsigned short port, dest_port;
//=========================================================================

//==================ARRAYS to Hold DATA===================================
//char ReceivedMessage[100];
//char ToSendMessage[200];
//char ProgramToSend [10000];
extern uint8_t data_received[6516];
//====================================================================

////===================static functions used ===========================
//
//// Function to send the data to the server
//static void tcpsend (char *data);
//static void tcp_ReseveMessage (void *arg );
//static void tcp_SendMessage (void *arg);
//static void tcp_SendProgram (void );
//
//
////==========================================================
//
static void tcp_send_message(void *arg)
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
//
			/* Connect to the TCP Server */
			connect_error = netconn_connect(conn, &dest_addr, 10);

			// If the connection to the server is established, the following will continue, else delete the connection
			if (connect_error == ERR_OK)
			{
				err = netconn_write(conn, data_received, 6516, NETCONN_COPY);
				while (err != ERR_OK);
				int x =9;
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
//
//void tcpsend (char *data)
//{
//	netconn_write(conn, data, 6516, NETCONN_COPY);
//}
//
//
//static void tcp_SendProgram (void )
//{
//
//	tcpsend(data_received);
//}
//
//
//static void tcp_SendMessage (void *arg)
//{
//	sprintf (ToSendMessage, "\"%s\" was sent by the Server\n", ReceivedMessage);
//	tcpsend(ToSendMessage);
//	memset (ToSendMessage, '\0', 200);  // clear the buffer
//
//	osThreadId_t id;
//	id = osThreadGetId();
//	osThreadTerminate(id);
//}
//
//void tcp_ReseveMessage (void *arg )
//{
//
//	while (1)
//	{
//		/* wait until the data is sent by the server */
//		if (netconn_recv(conn, &buf) == ERR_OK)
//		{
//			/* Extract the address and port in case they are required */
//			addr = netbuf_fromaddr(buf);  // get the address of the client
//			port = netbuf_fromport(buf);  // get the Port of the client
//
//			/* If there is some data remaining to be sent, the following process will continue */
//			do
//			{
//
//				strncpy (ReceivedMessage, buf->p->payload, buf->p->len);   // get the message from the server
//
//				if(strcmp(ReceivedMessage , "SEND")==0)
//				{
//					sys_thread_new("tcp_SendProgram", tcp_SendProgram, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
//
//				}
//				else
//				{
//					// send the received data  using tcp_SendMessage
//					//sys_thread_new("tcp_SendMessage", tcp_SendMessage, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
//				}
//
//				//memset (ReceivedMessage, '\0', 100);  // clear the buffer
//
//			}
//			while (netbuf_next(buf) >0);
//
//			netbuf_delete(buf);
//		}
//	}
//}



//#define MESSAGE_SIZE 6516
//#define PACKET_SIZE 536
//
//static void tcp_send_message(void* arg)
//{
//	struct netconn *conn;
//	err_t err;
//	struct netbuf *buf;
//	uint8_t *send_buffer;
//	uint16_t send_len = 0;
//
//	// Create a new TCP connection
//	conn = netconn_new(NETCONN_TCP);
//	if (conn == NULL) {
//		// printf("Failed to create TCP connection\n");
//		return;
//	}
//	if (conn!=NULL)
//	{
//		/* Bind connection to the port number 7 (port of the Client). */
//		err = netconn_bind(conn, IP_ADDR_ANY, 7);
//	}
//
//	//    // Connect to the server
//	//   		IP_ADDR4(&dest_addr, 169, 254, 84, 57);
//	//
//	//    err = netconn_connect(conn, dest_addr, 10);
//	IP_ADDR4(&dest_addr, 169, 254, 84, 57);
//	//dest_port = 10;  // server port
//
//	/* Connect to the TCP Server */
//	err = netconn_connect(conn, &dest_addr, 10);
//	if (err != ERR_OK) {
//		// printf("Failed to connect to server: %d\n", err);
//		netconn_delete(conn);
//		return;
//	}
//
//	// Allocate a buffer to hold the send message
//	// send_buffer = (uint8_t *)malloc(MESSAGE_SIZE);
//	//    if (send_buffer == NULL) {
//	//        printf("Failed to allocate send buffer\n");
//	//        netconn_close(conn);
//	//        netconn_delete(conn);
//	//        return;
//	//    }
//
//	// Fill the send buffer with data
//	// For example, fill it with a string of 'A's
//	//    memset(send_buffer, 'A', MESSAGE_SIZE);
//
//	// Send packets until the entire message is sent
//	while (send_len < MESSAGE_SIZE) {
//		// Create a new packet buffer
//		buf = netbuf_new();
//		if (buf == NULL) {
//			//printf("Failed to create packet buffer\n");
//			break;
//		}
//
//		// Allocate space in the packet buffer for the data
//		uint16_t packet_len = (send_len + PACKET_SIZE <= MESSAGE_SIZE) ? PACKET_SIZE : (MESSAGE_SIZE - send_len);
//		uint8_t *packet_data = netbuf_alloc(buf, packet_len);
//		if (packet_data == NULL) {
//			//printf("Failed to allocate packet data\n");
//			netbuf_delete(buf);
//			break;
//		}
//
//		// Copy the data into the packet buffer
//		memcpy(packet_data, data_received + send_len, packet_len);
//
//		// Send the packet
//		err = netconn_send(conn, packet_data);
//		if (err != ERR_OK) {
//			//printf("Failed to send packet: %d\n", err);
//			netbuf_delete(buf);
//			break;
//		}
//
//		// Update the send length
//		send_len += packet_len;
//
//		// Free the packet buffer
//		netbuf_delete(buf);
//	}
//
//	// Wait for the server to acknowledge receipt of the message
//	uint8_t ack;
//	err = netconn_recv(conn, &buf);
//	if (err == ERR_OK) {
//		netbuf_data(buf, &ack, sizeof(ack));
//		netbuf_delete(buf);
//		if (ack != 1) {
//			// printf("Failed to receive message acknowledgment\n");
//		}
//	} else {
//		//printf("Failed to receive message acknowledgment: %d\n", err);
//	}
//
//	// Free the send buffer
//	//free(send_buffer);
//
//	// Close the TCP connection
//	netconn_close(conn);
//	netconn_delete(conn);
//}


//static void tcp_send_message(void* arg)
//{
//    struct netconn *conn;
//    struct ip4_addr server_ip;
//    err_t err;
//
//    // Create a new netconn object for the TCP connection
//    conn = netconn_new(NETCONN_TCP);
//    if (!conn)
//    {
//       // printf("Failed to create netconn\n");
//        return ERR_MEM;
//    }
//    	err = netconn_bind(conn, IP_ADDR_ANY, 7);
//
//    // Set up the server IP and port
//
//    	IP_ADDR4(&dest_addr, 169, 254, 84, 57);
//    	//dest_port = 10;  // server port
//
//    	/* Connect to the TCP Server */
//    	err = netconn_connect(conn, &dest_addr, 10);
//    if (err != ERR_OK)
//    {
//       // printf("Failed to connect to server: %d\n", err);
//        netconn_delete(conn);
//        return err;
//    }
//
//    // Send the message
//    err = netconn_write(conn, data_received, 6516, NETCONN_COPY);
//    while (err != ERR_OK);
//
//    // Receive response from the server
//    struct netbuf *recv_buf;
//    if (netconn_recv(conn, &recv_buf) == ERR_OK)
//    {
//        void *data;
//        uint16_t len;
//
//        netbuf_data(recv_buf, &data, &len);
//      //  printf("Received from server: %.*s\n", len, (char *)data);
//        netbuf_delete(recv_buf);
//    }
//
//    // Close the connection
//    netconn_close(conn);
//
//    // Clean up resources
//    netconn_delete(conn);
//
//}

void tcpclient_init (void)
{
	sys_thread_new("tcpinit_thread", tcp_send_message, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
}
