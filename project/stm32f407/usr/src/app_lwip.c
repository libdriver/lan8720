/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      app_lwip.c
 * @brief     app lwip source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-08-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/08/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "app_lwip.h"

/*static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   (uint8_t) 192
#define IP_ADDR1   (uint8_t) 168
#define IP_ADDR2   (uint8_t) 1
#define IP_ADDR3   (uint8_t) 10

/* NETMASK */
#define NETMASK_ADDR0   (uint8_t) 255
#define NETMASK_ADDR1   (uint8_t) 255
#define NETMASK_ADDR2   (uint8_t) 255
#define NETMASK_ADDR3   (uint8_t) 0

/* gateway address*/
#define GW_ADDR0   (uint8_t) 192
#define GW_ADDR1   (uint8_t) 168
#define GW_ADDR2   (uint8_t) 1
#define GW_ADDR3   (uint8_t) 1 

/* dhcp process states */
#define DHCP_OFF                   (uint8_t) 0
#define DHCP_START                 (uint8_t) 1
#define DHCP_WAIT_ADDRESS          (uint8_t) 2
#define DHCP_ADDRESS_ASSIGNED      (uint8_t) 3
#define DHCP_TIMEOUT               (uint8_t) 4
#define DHCP_LINK_DOWN             (uint8_t) 5

#if LWIP_DHCP
  #define MAX_DHCP_TRIES  4
  uint32_t DHCPfineTimer = 0;
  uint8_t DHCP_state = DHCP_OFF;
#endif
struct netif g_netif;
uint32_t ethernet_link_timer;

/**
 * @brief netif get handle
 * @return points to a netif buffer
 */
struct netif *netif_get_handle(void)
{
    return &g_netif;
}

/**
 * @brief ethernet link status updated
 * @param *netif points to a netif struct
 */
static void ethernet_link_status_updated(struct netif *netif)
{
  if (netif_is_link_up(netif))
  {
#if LWIP_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_START;
#endif /* LWIP_DHCP */
  }
  else
  {
#if LWIP_DHCP
    /* Update DHCP state machine */
    DHCP_state = DHCP_LINK_DOWN;
#endif /* LWIP_DHCP */
  }
}

#if LWIP_NETIF_LINK_CALLBACK
/**
 * @brief  Ethernet Link periodic check
 * @param  netif
 * @retval None
 */
static void ethernet_link_periodic_handle(struct netif *netif)
{
  /* Ethernet Link every 100ms */
  if (HAL_GetTick() - ethernet_link_timer >= 100)
  {
    ethernet_link_timer = HAL_GetTick();
    ethernet_link_check_state(netif);
  }
}
#endif

#if LWIP_DHCP
/**
 * @brief  DHCP_Process_Handle
 * @param  None
 * @retval None
 */
static void dhcp_process(struct netif *netif)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
  struct dhcp *dhcp;
  uint8_t iptxt[20];
  
  switch (DHCP_state)
  {
    case DHCP_START:
    {
      lan8720_interface_debug_print("state: looking for dhcp server ...\n");
      ip_addr_set_zero_ip4(&netif->ip_addr);
      ip_addr_set_zero_ip4(&netif->netmask);
      ip_addr_set_zero_ip4(&netif->gw);
      dhcp_start(netif);
      DHCP_state = DHCP_WAIT_ADDRESS;
    }
    break;

  case DHCP_WAIT_ADDRESS:
    {
      if (dhcp_supplied_address(netif))
      {
        DHCP_state = DHCP_ADDRESS_ASSIGNED;
        sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
        lan8720_interface_debug_print("ip address assigned by a dhcp server: %s\n", iptxt);
      }
      else
      {
        dhcp = (struct dhcp *)netif_get_client_data(netif, LWIP_NETIF_CLIENT_DATA_INDEX_DHCP);
    
        /* DHCP timeout */
        if (dhcp->tries > MAX_DHCP_TRIES)
        {
          DHCP_state = DHCP_TIMEOUT;

          /* Static address used */
          IP_ADDR4(&ipaddr, IP_ADDR0 ,IP_ADDR1 , IP_ADDR2 , IP_ADDR3 );
          IP_ADDR4(&netmask, NETMASK_ADDR0, NETMASK_ADDR1, NETMASK_ADDR2, NETMASK_ADDR3);
          IP_ADDR4(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3);
          netif_set_addr(netif, &ipaddr, &netmask, &gw);
          sprintf((char *)iptxt, "%s", ip4addr_ntoa(netif_ip4_addr(netif)));
          lan8720_interface_debug_print("dhcp timeout !! \n");
          lan8720_interface_debug_print("static ip address: %s\n", iptxt);
        }
      }
    }
    break;
  case DHCP_LINK_DOWN:
    {
      DHCP_state = DHCP_OFF;
      lan8720_interface_debug_print("the network cable is not connected \n");
    }
    break;
  default: break;
  }
}

/**
 * @brief  DHCP periodic check
 * @param  netif
 * @retval None
 */
static void dhcp_periodic_handle(struct netif *netif)
{
  /* Fine DHCP periodic process every 500ms */
  if (HAL_GetTick() - DHCPfineTimer >= DHCP_FINE_TIMER_MSECS)
  {
    DHCPfineTimer =  HAL_GetTick();
    /* process DHCP state machine */
    dhcp_process(netif);
  }
}
#endif

/**
 * @brief netif config
 */
void netif_config(void)
{
    ip_addr_t ipaddr;
    ip_addr_t netmask;
    ip_addr_t gw;
  
#if LWIP_DHCP 
    ip_addr_set_zero_ip4(&ipaddr);
    ip_addr_set_zero_ip4(&netmask);
    ip_addr_set_zero_ip4(&gw);
#else
    /* ip address default setting */
    IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
    IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
    IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3); 
#endif
    /* add the network interface */    
    netif_add(&g_netif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  
    /*  registers the default network interface */
    netif_set_default(&g_netif);
  
    ethernet_link_status_updated(&g_netif);
  
#if LWIP_NETIF_LINK_CALLBACK
    netif_set_link_callback(&g_netif, ethernet_link_status_updated);
#endif
}

/**
 * @brief lwip server
 */
void lwip_server(void)
{
    /* Handle timeouts */
    sys_check_timeouts();

#if LWIP_NETIF_LINK_CALLBACK
    ethernet_link_periodic_handle(&g_netif);
#endif

#if LWIP_DHCP
    dhcp_periodic_handle(&g_netif);
#endif
}

