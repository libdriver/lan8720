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
 * @file      driver_lan8720_basic.c
 * @brief     driver lan8720 basic source file
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
 
#include "driver_lan8720_basic.h"

static lan8720_handle_t gs_handle;        /**< lan8720 handle */

/**
 * @brief     basic example init
 * @param[in] addr is the device address
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t lan8720_basic_init(uint8_t addr)
{
    uint8_t res;
    
    /* link interface function */
    DRIVER_LAN8720_LINK_INIT(&gs_handle, lan8720_handle_t); 
    DRIVER_LAN8720_LINK_SMI_INIT(&gs_handle, lan8720_interface_smi_init);
    DRIVER_LAN8720_LINK_SMI_DEINIT(&gs_handle, lan8720_interface_smi_deinit);
    DRIVER_LAN8720_LINK_SMI_READ(&gs_handle, lan8720_interface_smi_read);
    DRIVER_LAN8720_LINK_SMI_WRITE(&gs_handle, lan8720_interface_smi_write);
    DRIVER_LAN8720_LINK_RESET_GPIO_INIT(&gs_handle, lan8720_interface_reset_gpio_init);
    DRIVER_LAN8720_LINK_RESET_GPIO_DEINIT(&gs_handle, lan8720_interface_reset_gpio_deinit);
    DRIVER_LAN8720_LINK_RESET_GPIO_WRITE(&gs_handle, lan8720_interface_reset_gpio_write);
    DRIVER_LAN8720_LINK_DELAY_MS(&gs_handle, lan8720_interface_delay_ms);
    DRIVER_LAN8720_LINK_DEBUG_PRINT(&gs_handle, lan8720_interface_debug_print);
    
    /* set address */
    res = lan8720_set_address(&gs_handle, addr);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set address failed.\n");
        
        return 1;
    }
    
    /* init */
    res = lan8720_init(&gs_handle);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: init failed.\n");
        
        return 1;
    }
    
    /* disable loop back */
    res = lan8720_set_loop_back(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default speed */
    res = lan8720_set_speed_select(&gs_handle, LAN8720_BASIC_DEFAULT_SPEED);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set speed select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable auto negotiation */
    res = lan8720_set_auto_negotiation(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable power down */
    res = lan8720_set_power_down(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set power down failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable electrical isolation */
    res = lan8720_set_electrical_isolation(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set electrical isolation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* disable restart auto negotiate */
    res = lan8720_set_restart_auto_negotiate(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set restart auto negotiate failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set default duplex mode */
    res = lan8720_set_duplex_mode(&gs_handle, LAN8720_BASIC_DEFAULT_DUPLEX_MODE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set duplex mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    /* set phy address */
    res = lan8720_set_phy_address(&gs_handle, addr);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set phy address failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    
    return 0;
}

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t lan8720_basic_deinit(void)
{
    uint8_t res;
    
    res = lan8720_deinit(&gs_handle);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

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
uint8_t lan8720_basic_link_status(lan8720_link_t *status)
{
    uint8_t res;
    
    /* get link status */
    res = lan8720_get_link_status(&gs_handle, status);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}

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
uint8_t lan8720_basic_auto_negotiation(lan8720_speed_indication_t *speed)
{
    uint8_t res;
    uint16_t timeout = 1000;
    lan8720_bool_t enable;
    
    /* enable auto negotiation */
    res = lan8720_set_auto_negotiation(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        return 1;
    }
    
    /* loop for timeout */
    while (timeout != 0)
    {
        /* get auto negotiation done */
        res = lan8720_get_auto_negotiation_done(&gs_handle, &enable);
        if (res != 0)
        {
            return 1;
        }
        
        /* check break */
        if (enable == LAN8720_BOOL_TRUE)
        {
            break;
        }
        
        /* timeout-- */
        timeout--;
        
        /* delay 10ms */
        lan8720_interface_delay_ms(10);
    }
    
    /* get speed indication */
    res = lan8720_get_speed_indication(&gs_handle, speed);
    if (res != 0)
    {
        return 1;
    }
    
    return 0;
}
