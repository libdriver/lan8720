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
 * @file      stm32f407_driver_lan8720_interface.c
 * @brief     stm32f407 driver lan8720 interface source file
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

#include "driver_lan8720_interface.h"
#include "delay.h"
#include "eth.h"
#include "wire.h"
#include "uart.h"
#include <stdarg.h>

/**
 * @brief  interface smi bus init
 * @return status code
 *         - 0 success
 *         - 1 smi init failed
 * @note   none
 */
uint8_t lan8720_interface_smi_init(void)
{
    uint8_t mac[6] = {MAC_ADDR0, MAC_ADDR1, MAC_ADDR2, MAC_ADDR3, MAC_ADDR4, MAC_ADDR5};
    
    return eth_init(mac);
}

/**
 * @brief  interface smi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 smi deinit failed
 * @note   none
 */
uint8_t lan8720_interface_smi_deinit(void)
{
    return eth_deinit();
}

/**
 * @brief      interface smi bus read
 * @param[in]  addr is the device address
 * @param[in]  reg is the register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t lan8720_interface_smi_read(uint8_t addr, uint8_t reg, uint16_t *data)
{
    return eth_read_phy(addr, reg, data);
}

/**
 * @brief     interface smi bus write
 * @param[in] addr is the device address
 * @param[in] reg is the register address
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t lan8720_interface_smi_write(uint8_t addr, uint8_t reg, uint16_t data)
{
    return eth_write_phy(addr, reg, data);
}

/**
 * @brief  interface reset gpio init
 * @return status code
 *         - 0 success
 *         - 1 reset gpio init failed
 * @note   none
 */
uint8_t lan8720_interface_reset_gpio_init(void)
{
    return wire_init();
}

/**
 * @brief  interface reset gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 reset gpio deinit failed
 * @note   none
 */
uint8_t lan8720_interface_reset_gpio_deinit(void)
{
    return wire_deinit();
}

/**
 * @brief     interface reset gpio write
 * @param[in] level is the set level
 * @return    status code
 *            - 0 success
 *            - 1 reset gpio write failed
 * @note      none
 */
uint8_t lan8720_interface_reset_gpio_write(uint8_t level)
{
    return wire_write(level);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void lan8720_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void lan8720_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
}
