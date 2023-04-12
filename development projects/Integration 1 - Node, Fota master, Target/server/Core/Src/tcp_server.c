/*
 * tcp_server.c
 *
 *  Created on: Mar 6, 2023
 *      Author: kyrillos Phelopos Sawiris
 */


#include "lwip/opt.h"

#include "lwip/api.h"
#include "lwip/sys.h"

#include "tcp_server.h"
#include "string.h"
static struct netconn *conn, *newconn;
static struct netbuf *buf;


char RecivedProgram[6516];
#define MESSAGE_SIZE 6516


/**** Send RESPONSE every time the client sends some data ******/
static void tcp_thread(void *arg)
{
	err_t err, accept_err;
	int recv_len =0;

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

					/* receive the data from the client */
					while (recv_len < MESSAGE_SIZE) {
						// Receive a packet
						err = netconn_recv(newconn, &buf);
						if (err != ERR_OK) {
							break;
						}

						memcpy((RecivedProgram +recv_len) ,(char *)buf->p->payload,buf->p->len );
						recv_len+=buf->p->len;



						// Free the packet buffer
						netbuf_delete(buf);
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


void tcpserver_init(void)
{
	sys_thread_new("tcp_thread", tcp_thread, NULL, DEFAULT_THREAD_STACKSIZE,osPriorityNormal);
}


