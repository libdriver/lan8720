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
 * @file      driver_lan8720_basic.h
 * @brief     driver lan8720 basic header file
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

#ifndef DRIVER_LAN8720_BASIC_H
#define DRIVER_LAN8720_BASIC_H

#include "driver_lan8720_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup lan8720_example_driver lan8720 example driver function
 * @brief    lan8720 example driver modules
 * @ingroup  lan8720_driver
 * @{
 */

/**
 * @brief lan8720 basic example default definition
 */
#define LAN8720_BASIC_DEFAULT_SPEED              LAN8720_SPEED_100M        /**< 100Mbs */
#define LAN8720_BASIC_DEFAULT_DUPLEX_MODE        LAN8720_DUPLEX_FULL       /**< duplex full mode */

/**
 * @brief     basic example init
 * @param[in] addr is the device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t lan8720_basic_init(uint8_t addr);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t lan8720_basic_deinit(void);

/**
 * @brief      basic example get link status
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 link status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_basic_link_status(lan8720_link_t *status);

/**
 * @brief      basic example auto negotiation
 * @param[out] *speed points to a speed indication buffer
 * @return     status code
 *             - 0 success
 *             - 1 auto negotiation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_basic_auto_negotiation(lan8720_speed_indication_t *speed);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif