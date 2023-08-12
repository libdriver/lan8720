/**
  ******************************************************************************
  * @file    ethernetif.h
  * @author  MCD Application Team
  * @brief   Ethernet interface header file.
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

#ifndef __ETHERNETIF_H__
#define __ETHERNETIF_H__


#include "lwip/err.h"
#include "lwip/netif.h"

/* Exported types ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
 * @brief     set phy address
 * @param[in] addr is the device address
 * @note      none
 */
void eth_set_address(uint8_t addr);

err_t ethernetif_init(struct netif *netif);

/**
  * @brief This function should be called when a packet is ready to be read
  * from the interface. It uses the function low_level_input() that
  * should handle the actual reception of bytes from the network
  * interface. Then the type of the received packet is determined and
  * the appropriate input function is called.
  *
  * @param netif the lwip network interface structure for this ethernetif
  */
void ethernetif_input(struct netif *netif);

/**
  * @brief
  * @retval None
  */
void ethernet_link_check_state(struct netif *netif);
#endif
