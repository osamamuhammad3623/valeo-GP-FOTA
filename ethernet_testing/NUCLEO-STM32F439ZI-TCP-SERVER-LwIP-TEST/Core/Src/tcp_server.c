
#include "tcp_server.h"
#include "lwip/tcp.h"

#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f4xx_hal.h"
#include "EthernetMacros.h"

extern char SendBuf[SendBufLength];

enum tcp_server_states
{
	ES_NONE = 0,
	ES_ACCEPTED,
	ES_RECEIVED,
	ES_CLOSING
};

struct tcp_server_struct
{
	u8_t state;
	u8_t retries;
	struct tcp_pcb* pcb;
	struct pbuf* p;
};



static err_t tcp_server_accept(void* arg, struct tcp_pcb* newpcb, err_t err);
static err_t tcp_server_recv(void* arg, struct tcp_pcb* newpcb, struct pbuf* p, err_t err);
static void tcp_server_error(void *arg, err_t err);
static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb);
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len);

static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es);
static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es);

static void tcp_server_handle(struct tcp_pcb* tpcb, struct tcp_server_struct* es);


void tcp_server_init(void)
{
	/* 1. create new tcp pcb */
	struct tcp_pcb *tpcb;

	tpcb = tcp_new();

	err_t err;

	/* 2. bind _pcb to port 7 ( protocol) */
	ip_addr_t myIPADDR;
	IP_ADDR4(&myIPADDR, 192, 168, 1, 7);
	err = tcp_bind(tpcb, &myIPADDR, 7);

	if (err == ERR_OK)
	{
		/* 3. start tcp listening for _pcb */
		tpcb = tcp_listen(tpcb);

		/* 4. initialize LwIP tcp_accept callback function */
		tcp_accept(tpcb, tcp_server_accept);
	}
	else
	{
		/* deallocate the pcb */
		memp_free(MEMP_TCP_PCB, tpcb);
	}
}

static err_t tcp_server_accept(void *arg, struct tcp_pcb *newpcb, err_t err)
{
  err_t ret_err;
  struct tcp_server_struct *es;

  LWIP_UNUSED_ARG(arg);
  LWIP_UNUSED_ARG(err);

  /* set priority for the newly accepted tcp connection newpcb */
  tcp_setprio(newpcb, TCP_PRIO_MIN);

  /* allocate structure es to maintain tcp connection information */
  es = (struct tcp_server_struct *)mem_malloc(sizeof(struct tcp_server_struct));
  if (es != NULL)
  {
    es->state = ES_ACCEPTED;
    es->pcb = newpcb;
    es->retries = 0;
    es->p = NULL;

    /* pass newly allocated es structure as argument to newpcb */
    tcp_arg(newpcb, es);

    /* initialize lwip tcp_recv callback function for newpcb  */
    tcp_recv(newpcb, tcp_server_recv);

    /* initialize lwip tcp_err callback function for newpcb  */
    tcp_err(newpcb, tcp_server_error);

    /* initialize lwip tcp_poll callback function for newpcb */
    tcp_poll(newpcb, tcp_server_poll, 0);

    ret_err = ERR_OK;
  }
  else
  {
    /*  close tcp connection */
    tcp_server_connection_close(newpcb, es);
    /* return memory error */
    ret_err = ERR_MEM;
  }
  return ret_err;
}

static err_t tcp_server_recv(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
  struct tcp_server_struct *es;
  err_t ret_err;

  LWIP_ASSERT("arg != NULL",arg != NULL);

  es = (struct tcp_server_struct *)arg;

  /* if we receive an empty tcp frame from client => close connection */
  if (p == NULL)
  {
    /* remote host closed connection */
    es->state = ES_CLOSING;
    if(es->p == NULL)
    {
       /* we're done sending, close connection */
       tcp_server_connection_close(tpcb, es);
    }
    else
    {
      /* we're not done yet */
      /* acknowledge received packet */
      tcp_sent(tpcb, tcp_server_sent);

      /* send remaining data*/
      tcp_server_send(tpcb, es);
    }
    ret_err = ERR_OK;
  }
  /* else : a non empty frame was received from client but for some reason err != ERR_OK */
  else if(err != ERR_OK)
  {
    /* free received pbuf*/
    if (p != NULL)
    {
      es->p = NULL;
      pbuf_free(p);
    }
    ret_err = err;
  }
  else if(es->state == ES_ACCEPTED)
  {
    /* first data chunk in p->payload */
    es->state = ES_RECEIVED;

    /* store reference to incoming pbuf (chain) */
    es->p = p;

    /* initialize LwIP tcp_sent callback function */
    tcp_sent(tpcb, tcp_server_sent);

    /* handle the received data */
    tcp_server_handle(tpcb, es);

    ret_err = ERR_OK;
  }
  else if (es->state == ES_RECEIVED)
  {
    /* more data received from client and previous data has been already sent*/
    if(es->p == NULL)
    {
      es->p = p;

      /* handle the received data */
      tcp_server_handle(tpcb, es);
    }
    else
    {
      struct pbuf *ptr;

      /* chain pbufs to the end of what we recv'ed previously  */
      ptr = es->p;
      pbuf_chain(ptr,p);
    }
    ret_err = ERR_OK;
  }
  else if(es->state == ES_CLOSING)
  {
    /* odd case, remote side closing twice, trash data */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  else
  {
    /* unknown es->state, trash data  */
    tcp_recved(tpcb, p->tot_len);
    es->p = NULL;
    pbuf_free(p);
    ret_err = ERR_OK;
  }
  return ret_err;
}

static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	struct tcp_server_struct *es;

	LWIP_UNUSED_ARG(len);

	es = (struct tcp_server_struct *)arg;
	es->retries = 0;

	if(es->p != NULL)
	{
//		tcp_sent(tpcb, tcp_client_sent);
		tcp_server_send(tpcb, es);
	}
	else
	{
		if(es->state ==ES_CLOSING)
		{
			tcp_server_connection_close(tpcb, es);
		}
	}

	return ERR_OK;
}

static void tcp_server_error(void *arg, err_t err)
{

}


static void tcp_server_send(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	struct pbuf *ptr;
	err_t wr_err = ERR_OK;

	while((wr_err == ERR_OK) &&(es->p != NULL) &&(es->p->len <= tcp_sndbuf(tpcb)))
	{
		ptr = es->p;

		wr_err = tcp_write(tpcb, ptr->payload, ptr->len, 1);

		if(wr_err == ERR_OK)
		{
			u16_t plen;
			u8_t freed;

			plen = ptr->len;

			es->p = ptr->next;

			if(es->p != NULL)
			{
				pbuf_ref(es->p);
			}
			do
			{
				freed = pbuf_free(ptr);
			}
			while(freed==0);

			tcp_recved(tpcb, plen);
		}
		else if(wr_err ==ERR_MEM)
		{
			es->p = ptr;
		}
		else
		{

		}
	}
//	tcp_output(tpcb);
}

static void tcp_server_connection_close(struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, NULL);
	tcp_recv(tpcb, NULL);
	tcp_err(tpcb, NULL);
	tcp_accept(tpcb, NULL);
	tcp_poll(tpcb, NULL,0);

	if(es !=NULL)
	{
		mem_free(es);
	}

	tcp_close(tpcb);

}

static err_t tcp_server_poll(void *arg, struct tcp_pcb *tpcb)
{
	err_t ret_err;
	struct tcp_server_struct *es;

	es = (struct tcp_server_struct*)arg;

	if(es != NULL)
	{
		if(es->p != NULL)
		{

		}
		else
		{
			if(es->state == ES_CLOSING)
			{
				tcp_server_connection_close(tpcb,es);
			}
		}
		ret_err = ERR_OK;
	}
	else
	{
		tcp_abort(tpcb);
		ret_err = ERR_ABRT;
	}
	return ret_err;
}

static void tcp_server_handle (struct tcp_pcb *tpcb, struct tcp_server_struct *es)
{
	char *recv;

	/*save recieved string into recv */
	recv= (char*) es->p->payload;
	pbuf_free(es->p);

	char buf1[100];
	int len1 = sprintf(buf1, " Hello from server\n");

	/* allocate pbuf from RAM*/
	struct pbuf *txBuf1 = pbuf_alloc(PBUF_TRANSPORT, len1, PBUF_RAM);

	/* copy the data into the buffer  */
	pbuf_take(txBuf1, buf1, len1);

	/* copy the Tx buffer into the buffer  */
	es->p=txBuf1;

	/* send the TX buffer  */
	tcp_server_send(tpcb,es);

	/* delete the TX buffer  */
	pbuf_free(txBuf1);
	pbuf_free(es->p);

///////////////////////* start of my test area *///////////////////////
	if( !strcmp(recv,"red") )
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
	}
	if(  !strcmp(recv,"blue") )
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
	}
	if(  !strcmp(recv,"yellow") )
	{
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);
	}
	if(  !strcmp(recv,"data") )
	{

			/* allocate pbuf from RAM*/
			struct pbuf *txBuf = pbuf_alloc(PBUF_TRANSPORT, SendBufLength, PBUF_RAM);

			/* copy the data into the buffer  */
			pbuf_take(txBuf, SendBuf, SendBufLength);

			/* copy the Tx buffer into the buffer  */
			es->p=txBuf;

			/* send the TX buffer  */
			tcp_server_send(tpcb,es);

			/* free the TX buffer  */
			pbuf_free(txBuf);

			/*free pcb pbuf */
			pbuf_free(es->p);
	}
//	if(  !strcmp(recv,"halfdata") )
//	{
//
//			/* allocate pbuf from RAM*/
//			struct pbuf *txBuf2 = pbuf_alloc(PBUF_TRANSPORT, HalfSendBufLength, PBUF_RAM);
//
//			/* copy the data into the buffer  */
//			pbuf_take(txBuf2, SendBuf, HalfSendBufLength);
//
//			/* copy the Tx buffer into the buffer  */
//			es->p=txBuf2;
//
//			/* send the TX buffer  */
//			tcp_server_send(tpcb,es);
//
//			/* free the TX buffer  */
//			pbuf_free(txBuf2);
//
//			/*free pcb pbuf */
//			pbuf_free(es->p);
//	}

}

