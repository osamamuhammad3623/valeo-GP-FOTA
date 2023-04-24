#include "lwip/opt.h"

#include "lwip/api.h"
#include "lwip/sys.h"

#include "tcp_server.h"
#include "string.h"
static struct netconn *conn, *newconn;
static struct netbuf *buf;


char RecivedProgram[7200];
#define MESSAGE_SIZE 7200
int recv_len = 0;
int Flag = 0;




/**** Send RESPONSE every time the client sends some data ******/
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
					while (recv_len < MESSAGE_SIZE) {

						/* receive the data from the client */
						if (netconn_recv(newconn, &buf) == ERR_OK)
						{
							/* If there is some data remaining to be sent, the following process will continue */
							do
							{
								memcpy((RecivedProgram +recv_len) ,buf->p->payload,buf->p->len );
								recv_len+=buf->p->len;
							}
							while (netbuf_next(buf) >=0);

							netbuf_delete(buf);
						}

					}
					Flag = 1;
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

