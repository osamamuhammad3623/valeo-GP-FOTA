/*
 * tcp_server.c
 *
 *  Created on: Feb 24, 2023
 *      Author: phelp
 */


#include "lwip/opt.h"

#include "lwip/api.h"
#include "lwip/sys.h"

#include "tcp_server.h"
#include "string.h"
static struct netconn *conn, *newconn;
static struct netbuf *buf;
static ip_addr_t *addr;
static unsigned short port;
char msg[100];
char * data = (char*)0x10000000;
char smsg[200];


/**** Send RESPONSE every time the client sends some data ******/
static void tcp_thread(void *arg)
{
	err_t err, accept_err, recv_error;

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
					while (netconn_recv(newconn, &buf) == ERR_OK)
					{
						/* Extrct the address and port in case they are required */
						addr = netbuf_fromaddr(buf);  // get the address of the client
						port = netbuf_fromport(buf);  // get the Port of the client

						/* If there is some data remaining to be sent, the following process will continue */
						do
						{

							//To store data

							static int i=0;
							static int c=0;
							strncpy((data+i*c)  ,buf->p->payload, buf->p->len );
							i = buf->p->len;
							c++;

							// Or modify the message received, so that we can send it back to the client
							//int len = sprintf (smsg, "\"%s\" was sent by the Server\n", msg);
							int len = sprintf (smsg, " message was sent by the Server\n");

							netconn_write(newconn, smsg, len, NETCONN_COPY);  // send the message back to the client
							memset (msg, '\0', 100);  // clear the buffer
						}
						while (netbuf_next(buf) >0);

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
