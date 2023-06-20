/*
 * tcp_server.c
 *
 *  Created on: Feb 24, 2023
 *      Author: Kyrillos Phelopos Sawiris
 */

//=======================================================
//================ Includes
#include "cmsis_os.h"
#include "tcp_server.h"
#include "string.h"
//===============================================================
//======== netconn global parameters
struct netconn *conn, *newconn;
static struct netbuf *buf;
void (*UDS_init_conn_p) (void);
void (*UDS_exec_req_p) (void *argument);

//==============================================================================
//============ data structures used in send and receive ========================
uint8_t msg[100];
uint8_t chunk[CHUNK_SIZE+1];
uint32_t recv_len = 0;
extern uint8_t isDataFrame;
extern uint32_t downloadSize;

//==============================================================================
void init_uds_callback(void (*p)(void)) {
	UDS_init_conn_p = p;
}

void init_execute_request_callback(void (*p)(void *argument)) {
	UDS_exec_req_p = p;
}

//=========== UDS_execute_request task data ======================
osThreadId_t tcpThreadTaskHandle;

const osThreadAttr_t tcpThreadTask_attributes = {
  .name = "tcp_thread",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = DEFAULT_THREAD_STACKSIZE
};

static void tcp_thread(void *arg)
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

			while (1)
			{
				/* Grab new connection. */
				accept_err = netconn_accept(conn, &newconn);

				/* Process the new connection. */
				if (accept_err == ERR_OK)
				{
					/* Initialize UDS state (session, security access) */
					UDS_init_conn_p();

					/* Receive the data from the client */
					while (netconn_recv(newconn, &buf) == ERR_OK)
					{
						if (isDataFrame) {
							if (downloadSize >= CHUNK_SIZE) {
								tcp_receiveChunk(CHUNK_SIZE);
								downloadSize -= CHUNK_SIZE;
							} else {
								tcp_receiveChunk(downloadSize);
							}
						}
						else {
							/* If there is some data remaining to be sent, the following process will continue */
							do
							{
								memcpy((msg + recv_len),buf->p->payload,buf->p->len);
								recv_len+=buf->p->len;

							}
							while (netbuf_next(buf) >=0);	//remove loop!

							UDS_exec_req_p((void *)msg);
							recv_len = 0;

							netbuf_delete(buf);
						}
					}

					/* Close connection and discard connection identifier. */
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

void tcp_receiveChunk(uint16_t chunkSize) {
	memcpy((chunk +recv_len) ,buf->p->payload,buf->p->len );
	recv_len+=buf->p->len;

	netbuf_delete(buf);
	while (recv_len < chunkSize) {

		/* receive the data from the client */
		if (netconn_recv(newconn, &buf) == ERR_OK)
		{
			/* If there is some data remaining to be sent, the following process will continue */
			do
			{
				memcpy((chunk +recv_len) ,buf->p->payload,buf->p->len );
				recv_len+=buf->p->len;
			}
			while (netbuf_next(buf) >=0);

			netbuf_delete(buf);
		}
	}
	UDS_exec_req_p((void *)chunk);
	recv_len = 0;
}

void tcp_SendResponse(uint8_t *data , int len)
{
	netconn_write(newconn, data, len, NETCONN_COPY);
}

void tcpserver_init(void)
{
	tcpThreadTaskHandle = sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
	//tcpThreadTaskHandle = osThreadNew(tcp_thread, NULL, &tcpThreadTask_attributes);
}
