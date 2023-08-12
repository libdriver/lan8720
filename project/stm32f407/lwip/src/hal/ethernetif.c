/**
  ******************************************************************************
  * @file    LwIP/LwIP_HTTP_Server_Netconn_RTOS/Src/ethernetif.c
  * @author  MCD Application Team
  * @brief   This file implements Ethernet network interface drivers for lwIP
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "eth.h"
#define __CC_ARM
#include "lwip/timeouts.h"
#include "netif/ethernet.h"
#include "netif/etharp.h"
#include "lwip/stats.h"
#include "lwip/snmp.h"
#include "lwip/tcpip.h"
#include "ethernetif.h"
#undef __CC_ARM
#include "driver_lan8720_basic.h"
#include <string.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Define those to better describe your network interface. */
#define IFNAME0 's'
#define IFNAME1 't'

#define ETH_RX_BUFFER_SIZE            ETH_RX_BUF_SIZE
#define ETH_RX_BUFFER_CNT             10U

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
typedef enum
{
    RX_ALLOC_OK       = 0x00,
    RX_ALLOC_ERROR    = 0x01
} RxAllocStatusTypeDef;

typedef struct
{
    struct pbuf_custom pbuf_custom;
    uint8_t buff[(ETH_RX_BUFFER_SIZE + 31) & ~31] __ALIGNED(32);
} RxBuff_t;

/* Memory Pool Declaration */
LWIP_MEMPOOL_DECLARE(RX_POOL, ETH_RX_BUFFER_CNT, sizeof(RxBuff_t), "Zero-copy RX PBUF pool");
static uint8_t RxAllocStatus;

/* Private function prototypes -----------------------------------------------*/
void ethernet_link_check_state(struct netif *netif);
void pbuf_free_custom(struct pbuf *p);

static uint8_t gs_addr = 0x01;

/**
 * @brief     set phy address
 * @param[in] addr is the device address
 * @note      none
 */
void eth_set_address(uint8_t addr)
{
    gs_addr = addr;
}

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
                       LL Driver Interface ( LwIP stack --> ETH)
*******************************************************************************/
/**
  * @brief In this function, the hardware should be initialized.
  * Called from ethernetif_init().
  *
  * @param netif the already initialized lwip network interface structure
  *        for this ethernetif
  */
static void low_level_init(struct netif *netif)
{
    uint8_t mac[6] = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
    
    eth_init(mac);

    /* set MAC hardware address length */
    netif->hwaddr_len = ETH_HWADDR_LEN;

    /* set MAC hardware address */
    netif->hwaddr[0] =  MAC_ADDR0;
    netif->hwaddr[1] =  MAC_ADDR1;
    netif->hwaddr[2] =  MAC_ADDR2;
    netif->hwaddr[3] =  MAC_ADDR3;
    netif->hwaddr[4] =  MAC_ADDR4;
    netif->hwaddr[5] =  MAC_ADDR5;

    /* maximum transfer unit */
    netif->mtu = ETH_MAX_PAYLOAD;

    /* device capabilities */
    /* don't set NETIF_FLAG_ETHARP if this device is not an ethernet one */
    netif->flags |= NETIF_FLAG_BROADCAST | NETIF_FLAG_ETHARP;

    /* Initialize the RX POOL */
    LWIP_MEMPOOL_INIT(RX_POOL);
    
    (void)lan8720_basic_init(gs_addr);
    
    ethernet_link_check_state(netif);
}

/**
 * This function should do the actual transmission of the packet. The packet is
 * contained in the pbuf that is passed to the function. This pbuf
 * might be chained.
 *
 * @param netif the lwip network interface structure for this ethernetif
 * @param p the MAC packet to send (e.g. IP packet including MAC addresses and type)
 * @return ERR_OK if the packet was sent, or ERR_IF if the packet was unable to be sent
 *
 * @note ERR_OK means the packet was sent (but not necessarily transmit complete),
 * and ERR_IF means the packet has more chained buffers than what the interface supports.
 */
static err_t low_level_output(struct netif *netif, struct pbuf *p)
{
    uint32_t i = 0U;
    struct pbuf *q = NULL;
    err_t errval = ERR_OK;
    ETH_BufferTypeDef Txbuffer[ETH_TX_DESC_CNT] = {0};

    memset(Txbuffer, 0 , ETH_TX_DESC_CNT * sizeof(ETH_BufferTypeDef));
    for(q = p; q != NULL; q = q->next)
    {
        if(i >= ETH_TX_DESC_CNT)
        {
            return ERR_IF;
        }
        Txbuffer[i].buffer = q->payload;
        Txbuffer[i].len = q->len;
        if (i > 0)
        {
            Txbuffer[i-1].next = &Txbuffer[i];
        }
        if (q->next == NULL)
        {
            Txbuffer[i].next = NULL;
        }

        i++;
    }
    pbuf_ref(p);
    if (eth_write(Txbuffer, p, p->tot_len) != 0)
    {
        errval = ERR_TIMEOUT;
    }
    
    return errval;
}

/**
  * @brief Should allocate a pbuf and transfer the bytes of the incoming
  * packet from the interface into the pbuf.
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return a pbuf filled with the received packet (including MAC header)
  *         NULL on memory error
  */
static struct pbuf *low_level_input(struct netif *netif)
{
    struct pbuf *p = NULL;

    if(RxAllocStatus == RX_ALLOC_OK)
    {
        HAL_ETH_ReadData(eth_get_handle(), (void **)&p);
    }
    
    return p;
}

/**
 * This task should be signaled when a receive packet is ready to be read
 * from the interface.
 *
 * @param argument the lwip network interface structure for this ethernetif
 */
void ethernetif_input(struct netif *netif)
{
  struct pbuf *p = NULL;

    do
    {
        p = low_level_input(netif);
        if (p != NULL)
        {
            if (netif->input(p, netif) != ERR_OK)
            {
                pbuf_free(p);
            }
        }
    } while(p!=NULL);
}

/**
  * @brief Should be called at the beginning of the program to set up the
  * network interface. It calls the function low_level_init() to do the
  * actual setup of the hardware.
  *
  * This function should be passed as a parameter to netif_add().
  *
  * @param netif the lwip network interface structure for this ethernetif
  * @return ERR_OK if the loopif is initialized
  *         ERR_MEM if private data couldn't be allocated
  *         any other err_t on error
  */
err_t ethernetif_init(struct netif *netif)
{
    LWIP_ASSERT("netif != NULL", (netif != NULL));

#if LWIP_NETIF_HOSTNAME
    /* Initialize interface hostname */
    netif->hostname = "lwip";
#endif /* LWIP_NETIF_HOSTNAME */

    netif->name[0] = IFNAME0;
    netif->name[1] = IFNAME1;
    /* We directly use etharp_output() here to save a function call.
     * You can instead declare your own function an call etharp_output()
     * from it if you have to do some checks before sending (e.g. if link
     * is available...) */
    netif->output = etharp_output;
    netif->linkoutput = low_level_output;

    /* initialize the hardware */
    low_level_init(netif);

    return ERR_OK;
}

/**
  * @brief  Custom Rx pbuf free callback
  * @param  pbuf: pbuf to be freed
  * @retval None
  */
void pbuf_free_custom(struct pbuf *p)
{
    struct pbuf_custom* custom_pbuf = (struct pbuf_custom*)p;
    LWIP_MEMPOOL_FREE(RX_POOL, custom_pbuf);
    /* If the Rx Buffer Pool was exhausted, signal the ethernetif_input task to
     * call HAL_ETH_GetRxDataBuffer to rebuild the Rx descriptors. */
    if (RxAllocStatus == RX_ALLOC_ERROR)
    {
        RxAllocStatus = RX_ALLOC_OK;
    }
}

/**
  * @brief  Returns the current time in milliseconds
  *         when LWIP_TIMERS == 1 and NO_SYS == 1
  * @param  None
  * @retval Current Time value
  */
u32_t sys_now(void)
{
    return HAL_GetTick();
}

/**
  * @brief
  * @retval None
  */
void ethernet_link_check_state(struct netif *netif)
{
    ETH_MACConfigTypeDef MACConf = {0};
    lan8720_speed_indication_t speed_indication;
    uint32_t linkchanged = 0U, speed = 0U, duplex =0U;
    
    /* check auto negotiation */
    if (lan8720_basic_auto_negotiation(&speed_indication) != 0)
    {
        return;
    }
    
    if(!netif_is_link_up(netif))
    {
        switch (speed_indication)
        {
            case LAN8720_SPEED_INDICATION_100BASE_TX_FULL_DUPLEX:
              duplex = ETH_FULLDUPLEX_MODE;
              speed = ETH_SPEED_100M;
              linkchanged = 1;
              break;
            case LAN8720_SPEED_INDICATION_100BASE_TX_HALF_DUPLEX:
              duplex = ETH_HALFDUPLEX_MODE;
              speed = ETH_SPEED_100M;
              linkchanged = 1;
              break;
            case LAN8720_SPEED_INDICATION_10BASE_T_FULL_DUPLEX:
              duplex = ETH_FULLDUPLEX_MODE;
              speed = ETH_SPEED_10M;
              linkchanged = 1;
              break;
            case LAN8720_SPEED_INDICATION_10BASE_T_HALF_DUPLEX:
              duplex = ETH_HALFDUPLEX_MODE;
              speed = ETH_SPEED_10M;
              linkchanged = 1;
              break;
            default:
              break;
        }
        if (linkchanged)
        {
            /* Get MAC Config MAC */
            HAL_ETH_GetMACConfig(eth_get_handle(), &MACConf);
            MACConf.DuplexMode = duplex;
            MACConf.Speed = speed;
            HAL_ETH_SetMACConfig(eth_get_handle(), &MACConf);
            HAL_ETH_Start_IT(eth_get_handle());
            netif_set_up(netif);
            netif_set_link_up(netif);
        }
    }
}

void HAL_ETH_RxAllocateCallback(uint8_t **buff)
{
    struct pbuf_custom *p = LWIP_MEMPOOL_ALLOC(RX_POOL);
    if (p)
    {
        /* Get the buff from the struct pbuf address. */
        *buff = (uint8_t *)p + offsetof(RxBuff_t, buff);
        p->custom_free_function = pbuf_free_custom;
        /* Initialize the struct pbuf.
        * This must be performed whenever a buffer's allocated because it may be
        * changed by lwIP or the app, e.g., pbuf_free decrements ref. */
        pbuf_alloced_custom(PBUF_RAW, 0, PBUF_REF, p, *buff, ETH_RX_BUFFER_SIZE);
    }
    else
    {
        RxAllocStatus = RX_ALLOC_ERROR;
        *buff = NULL;
    }
}

void HAL_ETH_RxLinkCallback(void **pStart, void **pEnd, uint8_t *buff, uint16_t Length)
{
    struct pbuf **ppStart = (struct pbuf **)pStart;
    struct pbuf **ppEnd = (struct pbuf **)pEnd;
    struct pbuf *p = NULL;

    /* Get the struct pbuf from the buff address. */
    p = (struct pbuf *)(buff - offsetof(RxBuff_t, buff));
    p->next = NULL;
    p->tot_len = 0;
    p->len = Length;

    /* Chain the buffer. */
    if (!*ppStart)
    {
        /* The first buffer of the packet. */
        *ppStart = p;
    }
    else
    {
        /* Chain the buffer to the end of the packet. */
        (*ppEnd)->next = p;
    }
    *ppEnd = p;

    /* Update the total length of all the buffers of the chain. Each pbuf in the chain should have its tot_len
    * set to its own length, plus the length of all the following pbufs in the chain. */
    for (p = *ppStart; p != NULL; p = p->next)
    {
        p->tot_len += Length;
    }
}

void HAL_ETH_TxFreeCallback(uint32_t * buff)
{
    pbuf_free((struct pbuf *)buff);
}
