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
 * @file      driver_lan8720_register_test.c
 * @brief     driver lan8720 register test source file
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
 
#include "driver_lan8720_register_test.h"
#include <stdlib.h>

static lan8720_handle_t gs_handle;        /**< lan8720 handle */

/**
 * @brief     register test
 * @param[in] addr is the device address
 * @return    status code
 *            - 0 success
 *            - 1 test failed
 * @note      none
 */
uint8_t lan8720_register_test(uint8_t addr)
{
    uint8_t res;
    uint8_t reg;
    uint8_t reg_check;
    uint8_t selector;
    uint8_t selector_check;
    uint32_t phy_id; 
    uint8_t model_number;
    uint8_t revision_number;
    uint32_t phy_id_check; 
    uint8_t model_number_check;
    uint8_t revision_number_check;
    uint16_t cnt;
    lan8720_speed_t speed;
    lan8720_info_t info;
    lan8720_bool_t enable;
    lan8720_duplex_t mode;
    lan8720_manual_channel_select_t select;
    lan8720_link_t status;
    lan8720_pause_t pause;
    lan8720_mode_t mode_check;
    lan8720_polarity_t polarity;
    lan8720_speed_indication_t speed_indication;

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
    
    /* get information */
    res = lan8720_info(&info);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get info failed.\n");
        
        return 1;
    }
    else
    {
        /* print chip info */
        lan8720_interface_debug_print("lan8720: chip is %s.\n", info.chip_name);
        lan8720_interface_debug_print("lan8720: manufacturer is %s.\n", info.manufacturer_name);
        lan8720_interface_debug_print("lan8720: interface is %s.\n", info.interface);
        lan8720_interface_debug_print("lan8720: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        lan8720_interface_debug_print("lan8720: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        lan8720_interface_debug_print("lan8720: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        lan8720_interface_debug_print("lan8720: max current is %0.2fmA.\n", info.max_current_ma);
        lan8720_interface_debug_print("lan8720: max temperature is %0.1fC.\n", info.temperature_max);
        lan8720_interface_debug_print("lan8720: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    lan8720_interface_debug_print("lan8720: start register test.\n");
    
    /* lan8720_set_address/lan8720_get_address test */
    lan8720_interface_debug_print("lan8720: lan8720_set_address/lan8720_get_address test.\n");
    
    /* set address */
    reg = rand() % 0x20;
    res = lan8720_set_address(&gs_handle, reg);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set address failed.\n");
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set address 0x%02X.\n", reg);
    res = lan8720_get_address(&gs_handle, &reg_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get address failed.\n");
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check address %s.\n", reg == reg_check ? "ok" : "error");
    
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
    
    /* lan8720_set_loop_back/lan8720_get_loop_back test */
    lan8720_interface_debug_print("lan8720: lan8720_set_loop_back/lan8720_get_loop_back test.\n");
    
    /* enable loop back */
    res = lan8720_set_loop_back(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable loop back.\n");
    res = lan8720_get_loop_back(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check loop back %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable loop back */
    res = lan8720_set_loop_back(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable loop back.\n");
    res = lan8720_get_loop_back(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check loop back %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_speed_select/lan8720_get_speed_select test */
    lan8720_interface_debug_print("lan8720: lan8720_set_speed_select/lan8720_get_speed_select test.\n");
    
    /* 10M */
    res = lan8720_set_speed_select(&gs_handle, LAN8720_SPEED_10M);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set speed select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set speed select 10mbps.\n");
    res = lan8720_get_speed_select(&gs_handle, &speed);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get speed select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check speed select %s.\n", speed == LAN8720_SPEED_10M ? "ok" : "error");
    
    /* 100M */
    res = lan8720_set_speed_select(&gs_handle, LAN8720_SPEED_100M);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set speed select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set speed select 100mbps.\n");
    res = lan8720_get_speed_select(&gs_handle, &speed);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get speed select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check speed select %s.\n", speed == LAN8720_SPEED_100M ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation/lan8720_get_auto_negotiation test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation/lan8720_get_auto_negotiation test.\n");
    
    /* enable auto negotiation */
    res = lan8720_set_auto_negotiation(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation failed.\n");
    res = lan8720_get_auto_negotiation(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable auto negotiation */
    res = lan8720_set_auto_negotiation(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation failed.\n");
    res = lan8720_get_auto_negotiation(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_power_down/lan8720_get_power_down test */
    lan8720_interface_debug_print("lan8720: lan8720_set_power_down/lan8720_get_power_down test.\n");
    
    /* enable power down */
    res = lan8720_set_power_down(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set power down failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable power down.\n");
    res = lan8720_get_power_down(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get power down failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check power down %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable power down */
    res = lan8720_set_power_down(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set power down failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable power down.\n");
    res = lan8720_get_power_down(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get power down failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check power down %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_electrical_isolation/lan8720_get_electrical_isolation test */
    lan8720_interface_debug_print("lan8720: lan8720_set_electrical_isolation/lan8720_get_electrical_isolation test.\n");
    
    /* enable electrical isolation */
    res = lan8720_set_electrical_isolation(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set electrical isolation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable electrical isolation.\n");
    res = lan8720_get_electrical_isolation(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get electrical isolation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check electrical isolation %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable electrical isolation */
    res = lan8720_set_electrical_isolation(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set electrical isolation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable electrical isolation.\n");
    res = lan8720_get_electrical_isolation(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get electrical isolation failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check electrical isolation %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_restart_auto_negotiate/lan8720_get_restart_auto_negotiate test */
    lan8720_interface_debug_print("lan8720: lan8720_set_restart_auto_negotiate/lan8720_get_restart_auto_negotiate test.\n");
    
    /* enable restart auto negotiate */
    res = lan8720_set_restart_auto_negotiate(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set restart auto negotiate failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable restart auto negotiate.\n");
    res = lan8720_get_restart_auto_negotiate(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get restart auto negotiate failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check restart auto negotiate %s.\n", res == 0 ? "ok" : "error");
    
    /* disable restart auto negotiate */
    res = lan8720_set_restart_auto_negotiate(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set restart auto negotiate failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable restart auto negotiate.\n");
    res = lan8720_get_restart_auto_negotiate(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get restart auto negotiate failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check restart auto negotiate %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_duplex_mode/lan8720_get_duplex_mode test */
    lan8720_interface_debug_print("lan8720: lan8720_set_duplex_mode/lan8720_get_duplex_mode test.\n");
    
    /* half duplex */
    res = lan8720_set_duplex_mode(&gs_handle, LAN8720_DUPLEX_HALF);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set duplex mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set half duplex mode.\n");
    res = lan8720_get_duplex_mode(&gs_handle, &mode);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get duplex mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check duplex mode %s.\n", mode == LAN8720_DUPLEX_HALF ? "ok" : "error");
    
    /* full duplex */
    res = lan8720_set_duplex_mode(&gs_handle, LAN8720_DUPLEX_FULL);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set duplex mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set full duplex mode.\n");
    res = lan8720_get_duplex_mode(&gs_handle, &mode);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get duplex mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check duplex mode %s.\n", mode == LAN8720_DUPLEX_FULL ? "ok" : "error");
    
    /* lan8720_get_100base_t4 test */
    lan8720_interface_debug_print("lan8720: lan8720_get_100base_t4 test.\n");
    
    /* get 100base t4 */
    res = lan8720_get_100base_t4(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 100base t4 failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 100base t4 is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_100base_tx_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_100base_tx_full_duplex test.\n");
    
    /* get 100base tx full duplex */
    res = lan8720_get_100base_tx_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 100base tx full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 100base tx full duplex is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_100base_tx_half_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_100base_tx_half_duplex test.\n");
    
    /* get 100base tx half duplex */
    res = lan8720_get_100base_tx_half_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 100base tx half duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 100base tx half duplex is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_10base_t_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_10base_t_full_duplex test.\n");
    
    /* get 10base t full duplex */
    res = lan8720_get_10base_t_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 10base t full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 10base t full duplex is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_10base_t_half_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_10base_t_half_duplex test.\n");
    
    /* get 10base t half duplex */
    res = lan8720_get_10base_t_half_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 10base t half duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 10base t half duplex is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_100base_t2_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_100base_t2_full_duplex test.\n");
    
    /* get 100base t2 full duplex */
    res = lan8720_get_100base_t2_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 100base t2 full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 100base t2 full duplex is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_100base_t2_half_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_100base_t2_half_duplex test.\n");
    
    /* get 100base t2 half duplex */
    res = lan8720_get_100base_t2_half_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get 100base t2 half duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: 100base t2 half duplex is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_extended_status_information test */
    lan8720_interface_debug_print("lan8720: lan8720_get_extended_status_information test.\n");
    
    /* get extended status information */
    res = lan8720_get_extended_status_information(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get extended status information failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: extended status information is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_auto_negotiate_complete test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiate_complete test.\n");
    
    /* get auto negotiate complete */
    res = lan8720_get_auto_negotiate_complete(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiate complete failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: auto negotiate complete is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_remote_fault test */
    lan8720_interface_debug_print("lan8720: lan8720_get_remote_fault test.\n");
    
    /* get remote fault */
    res = lan8720_get_remote_fault(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get remote fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: remote fault is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_auto_negotiate_ability test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiate_ability test.\n");
    
    /* get auto negotiate ability */
    res = lan8720_get_auto_negotiate_ability(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiate ability failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: auto negotiate ability is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_link_status test */
    lan8720_interface_debug_print("lan8720: lan8720_get_link_status test.\n");
    
    /* get link status */
    res = lan8720_get_link_status(&gs_handle, &status);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get link status failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: link status is %s.\n", status == LAN8720_LINK_DOWN ? "down" : "up");
    
    /* lan8720_get_jabber_detect test */
    lan8720_interface_debug_print("lan8720: lan8720_get_jabber_detect test.\n");
    
    /* get jabber detect */
    res = lan8720_get_jabber_detect(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get jabber detect failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: jabber detect is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_get_extended_capabilities test */
    lan8720_interface_debug_print("lan8720: lan8720_get_extended_capabilities test.\n");
    
    /* get extended capabilities */
    res = lan8720_get_extended_capabilities(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get extended capabilities failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: extended capabilities is %s.\n", enable == LAN8720_BOOL_TRUE ? "yes" : "no");
    
    /* lan8720_set_identifier/lan8720_get_identifier test */
    lan8720_interface_debug_print("lan8720: lan8720_set_identifier/lan8720_get_identifier test.\n");
    
    /* set identifier */
    phy_id = rand() % 0x3FFFFF;
    model_number  = rand() % 0x3F;
    revision_number = rand() % 0xF;
    res = lan8720_set_identifier(&gs_handle, phy_id, model_number, revision_number);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set identifier failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set phy_id 0x%06X.\n", phy_id);
    lan8720_interface_debug_print("lan8720: set model_number 0x%02X.\n", model_number);
    lan8720_interface_debug_print("lan8720: set revision_number 0x%02X.\n", revision_number);
    res = lan8720_get_identifier(&gs_handle, &phy_id_check, &model_number_check, &revision_number_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get identifier failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check phy_id %s.\n", phy_id == phy_id_check ? "ok" : "error");
    lan8720_interface_debug_print("lan8720: check model_number %s.\n", model_number == model_number_check ? "ok" : "error");
    lan8720_interface_debug_print("lan8720: check revision_number %s.\n", revision_number == revision_number_check ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_remote_fault/lan8720_get_auto_negotiation_advertisement_remote_fault test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_remote_fault/lan8720_get_auto_negotiation_advertisement_remote_fault test.\n");
    
    /* enable */
    res = lan8720_set_auto_negotiation_advertisement_remote_fault(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement remote fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation advertisement remote fault.\n");
    res = lan8720_get_auto_negotiation_advertisement_remote_fault(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement remote fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement remote fault %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_auto_negotiation_advertisement_remote_fault(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement remote fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation advertisement remote fault.\n");
    res = lan8720_get_auto_negotiation_advertisement_remote_fault(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement remote fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement remote fault %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_pause/lan8720_get_auto_negotiation_advertisement_pause test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_pause/lan8720_get_auto_negotiation_advertisement_pause test.\n");
    
    /* no pause */
    res = lan8720_set_auto_negotiation_advertisement_pause(&gs_handle, LAN8720_PAUSE_NO);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set no pause.\n");
    res = lan8720_get_auto_negotiation_advertisement_pause(&gs_handle, &pause);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement pause %s.\n", pause == LAN8720_PAUSE_NO ? "ok" : "error");
    
    /* symmetric pause */
    res = lan8720_set_auto_negotiation_advertisement_pause(&gs_handle, LAN8720_PAUSE_SYMMETRIC);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set symmetric pause.\n");
    res = lan8720_get_auto_negotiation_advertisement_pause(&gs_handle, &pause);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement pause %s.\n", pause == LAN8720_PAUSE_SYMMETRIC ? "ok" : "error");
    
    /* asymmetric pause */
    res = lan8720_set_auto_negotiation_advertisement_pause(&gs_handle, LAN8720_PAUSE_ASYMMETRIC);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set asymmetric pause.\n");
    res = lan8720_get_auto_negotiation_advertisement_pause(&gs_handle, &pause);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement pause %s.\n", pause == LAN8720_PAUSE_ASYMMETRIC ? "ok" : "error");
    
    /* both pause */
    res = lan8720_set_auto_negotiation_advertisement_pause(&gs_handle, LAN8720_PAUSE_BOTH);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set both pause.\n");
    res = lan8720_get_auto_negotiation_advertisement_pause(&gs_handle, &pause);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement pause %s.\n", pause == LAN8720_PAUSE_BOTH ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex/lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex/lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex test.\n");
    
    /* enable */
    res = lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 100base tx full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation advertisement 100base tx full duplex.\n");
    res = lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 100base tx full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 100base tx full duplex %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 100base tx full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation advertisement 100base tx full duplex.\n");
    res = lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 100base tx full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 100base tx full duplex %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_100base_tx/lan8720_get_auto_negotiation_advertisement_100base_tx test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_100base_tx/lan8720_get_auto_negotiation_advertisement_100base_tx test.\n");
    
    /* enable */
    res = lan8720_set_auto_negotiation_advertisement_100base_tx(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 100base tx failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation advertisement 100base tx.\n");
    res = lan8720_get_auto_negotiation_advertisement_100base_tx(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 100base tx failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 100base tx %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_auto_negotiation_advertisement_100base_tx(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 100base tx failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation advertisement 100base tx.\n");
    res = lan8720_get_auto_negotiation_advertisement_100base_tx(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 100base tx failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 100base tx %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex/lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex/lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex test.\n");
    
    /* enable */
    res = lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 10base t full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation advertisement 10base t full duplex failed.\n");
    res = lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 10base t full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 10base t full duplex %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 10base t full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation advertisement 10base t full duplex failed.\n");
    res = lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 10base t full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 10base t full duplex %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_10base_t/lan8720_get_auto_negotiation_advertisement_10base_t test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_10base_t/lan8720_get_auto_negotiation_advertisement_10base_t test.\n");
    
    /* enable */
    res = lan8720_set_auto_negotiation_advertisement_10base_t(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 10base t failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation advertisement 10base t.\n");
    res = lan8720_get_auto_negotiation_advertisement_10base_t(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 10base t failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 10base t %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_auto_negotiation_advertisement_10base_t(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement 10base t failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation advertisement 10base t.\n");
    res = lan8720_get_auto_negotiation_advertisement_10base_t(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement 10base t failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement 10base t %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_auto_negotiation_advertisement_selector_field/lan8720_get_auto_negotiation_advertisement_selector_field test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_negotiation_advertisement_selector_field/lan8720_get_auto_negotiation_advertisement_selector_field test.\n");
    
    selector = rand() % 0x1F;
    res = lan8720_set_auto_negotiation_advertisement_selector_field(&gs_handle, selector);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto negotiation advertisement selector field failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set auto negotiation advertisement selector field 0x%02X.\n", selector);
    res = lan8720_get_auto_negotiation_advertisement_selector_field(&gs_handle, &selector_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation advertisement selector field failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation advertisement selector field %s.\n", selector_check == selector ? "ok" : "error");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_next_page test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_next_page test.\n");
    
    /* get auto negotiation link partner ability next page */
    res = lan8720_get_auto_negotiation_link_partner_ability_next_page(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability next page failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability next page %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_acknowledge test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_acknowledge test.\n");
    
    /* get auto negotiation link partner ability acknowledge */
    res = lan8720_get_auto_negotiation_link_partner_ability_acknowledge(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability acknowledge failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability acknowledge %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_remote_fault test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_remote_fault test.\n");
    
    /* get auto negotiation link partner ability remote fault */
    res = lan8720_get_auto_negotiation_link_partner_ability_remote_fault(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability remote fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability remote fault %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_pause test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_pause test.\n");
    
    /* get auto negotiation link partner ability pause */
    res = lan8720_get_auto_negotiation_link_partner_ability_pause(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability pause failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability pause %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_100base_t4 test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_100base_t4 test.\n");
    
    /* get auto negotiation link partner ability 100base t4 */
    res = lan8720_get_auto_negotiation_link_partner_ability_100base_t4(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability 100base t4 failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability 100base t4 %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_100base_tx_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_100base_tx_full_duplex test.\n");
    
    /* get auto negotiation link partner ability 100base tx full duplex */
    res = lan8720_get_auto_negotiation_link_partner_ability_100base_tx_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability 100base tx full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability 100base tx full duplex %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_100base_tx test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_100base_tx test.\n");
    
    /* get auto negotiation link partner ability 100base tx */
    res = lan8720_get_auto_negotiation_link_partner_ability_100base_tx(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability 100base tx failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability 100base tx %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_10base_t_full_duplex test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_10base_t_full_duplex test.\n");
    
    /* get auto negotiation link partner ability 10base t full duplex */
    res = lan8720_get_auto_negotiation_link_partner_ability_10base_t_full_duplex(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability 10base t full duplex failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability 10base t full duplex %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_10base_t test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_10base_t test.\n");
    
    /* get auto negotiation link partner ability 10base t */
    res = lan8720_get_auto_negotiation_link_partner_ability_10base_t(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability 10base t failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation link partner ability 10base t %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_link_partner_ability_selector_field test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_link_partner_ability_selector_field test.\n");
    
    /* get auto negotiation link partner ability selector field */
    res = lan8720_get_auto_negotiation_link_partner_ability_selector_field(&gs_handle, &selector);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation link partner ability selector field failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: auto negotiation link partner ability selector field is 0x%02X.\n", selector);
    
    /* lan8720_get_auto_negotiation_expansion_parallel_detection_fault test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_expansion_parallel_detection_fault test.\n");
    
    /* get auto negotiation expansion parallel detection fault */
    res = lan8720_get_auto_negotiation_expansion_parallel_detection_fault(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation expansion parallel detection fault failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation expansion parallel detection fault %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_expansion_link_partner_next_page_able test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_expansion_link_partner_next_page_able test.\n");
    
    /* get auto negotiation expansion link partner next page able */
    res = lan8720_get_auto_negotiation_expansion_link_partner_next_page_able(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation expansion link partner next page able failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation expansion link partner next page able %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_expansion_next_page_able test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_expansion_next_page_able test.\n");
    
    /* get auto negotiation expansion next page able */
    res = lan8720_get_auto_negotiation_expansion_next_page_able(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation expansion next page able failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation expansion next page able %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_expansion_page_received test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_expansion_page_received test.\n");
    
    /* get auto negotiation expansion page received */
    res = lan8720_get_auto_negotiation_expansion_page_received(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation expansion page received failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation expansion page received %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_auto_negotiation_expansion_link_partner_auto_negotiation_able test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_expansion_link_partner_auto_negotiation_able test.\n");
    
    /* get auto negotiation expansion link partner auto negotiation able */
    res = lan8720_get_auto_negotiation_expansion_link_partner_auto_negotiation_able(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation expansion link partner auto negotiation able failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation expansion link partner auto negotiation able %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_set_energy_detect_power_down_mode/lan8720_get_energy_detect_power_down_mode test */
    lan8720_interface_debug_print("lan8720: lan8720_set_energy_detect_power_down_mode/lan8720_get_energy_detect_power_down_mode test.\n");
    
    /* enable */
    res = lan8720_set_energy_detect_power_down_mode(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set energy detect power down mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable energy detect power down mode.\n");
    res = lan8720_get_energy_detect_power_down_mode(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get energy detect power down mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check energy detect power down mode %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_energy_detect_power_down_mode(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set energy detect power down mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable energy detect power down mode.\n");
    res = lan8720_get_energy_detect_power_down_mode(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get energy detect power down mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check energy detect power down mode %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_far_loop_back/lan8720_get_far_loop_back test */
    lan8720_interface_debug_print("lan8720: lan8720_set_far_loop_back/lan8720_get_far_loop_back test.\n");
    
    /* enable */
    res = lan8720_set_far_loop_back(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set far loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable far loop back.\n");
    res = lan8720_get_far_loop_back(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get far loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check far loop back %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_far_loop_back(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set far loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable far loop back.\n");
    res = lan8720_get_far_loop_back(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get far loop back failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check far loop back %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_alternate_interrupt_mode/lan8720_get_alternate_interrupt_mode test */
    lan8720_interface_debug_print("lan8720: lan8720_set_alternate_interrupt_mode/lan8720_get_alternate_interrupt_mode test.\n");
    
    /* enable */
    res = lan8720_set_alternate_interrupt_mode(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set alternate interrupt mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable alternate interrupt mode.\n");
    res = lan8720_get_alternate_interrupt_mode(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get alternate interrupt mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check alternate interrupt mode %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_alternate_interrupt_mode(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set alternate interrupt mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable alternate interrupt mode.\n");
    res = lan8720_get_alternate_interrupt_mode(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get alternate interrupt mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check alternate interrupt mode %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_get_energy_detected test */
    lan8720_interface_debug_print("lan8720: lan8720_get_energy_detected test.\n");
    
    /* get energy detected */
    res = lan8720_get_energy_detected(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get energy detected failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check energy detected %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_mode/lan8720_get_mode test */
    lan8720_interface_debug_print("lan8720: lan8720_set_mode/lan8720_get_mode test.\n");
    
    /* 10base-t half duplex */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_10BASE_T_HALF_DUPLEX);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set 10base-t half duplex.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_10BASE_T_HALF_DUPLEX ? "ok" : "error");
    
    /* 10base-t full duplex */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_10BASE_T_FULL_DUPLEX);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set 10base-t full duplex.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_10BASE_T_FULL_DUPLEX ? "ok" : "error");
    
    /* 100base-tx half duplex */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_100BASE_TX_HALF_DUPLEX);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set 100base-tx half duplex.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_100BASE_TX_HALF_DUPLEX ? "ok" : "error");
    
    /* 100base-tx full duplex */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_100BASE_TX_FULL_DUPLEX);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set 100base-tx full duplex.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_100BASE_TX_FULL_DUPLEX ? "ok" : "error");
    
    /* 100base-tx half duplex is advertised */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_100BASE_TX_HALF_DUPLEX_ADVERTISED);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set 100base-tx half duplex advertised.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_100BASE_TX_HALF_DUPLEX_ADVERTISED ? "ok" : "error");
    
    /* repeater mode */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_REPEATER_MODE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set repeater mode.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_REPEATER_MODE ? "ok" : "error");
    
    /* power down mode */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_POWER_DOWN_MODE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set power down mode.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_POWER_DOWN_MODE ? "ok" : "error");
    
    /* all capable */
    res = lan8720_set_mode(&gs_handle, LAN8720_MODE_ALL_CAPABLE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set all capable.\n");
    res = lan8720_get_mode(&gs_handle, &mode_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get mode failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check mode %s.\n", mode_check == LAN8720_MODE_ALL_CAPABLE ? "ok" : "error");
    
    /* lan8720_set_phy_address/lan8720_get_phy_address test */
    lan8720_interface_debug_print("lan8720: lan8720_set_phy_address/lan8720_get_phy_address test.\n");
    
    /* set phy address */
    res = lan8720_set_phy_address(&gs_handle, 0x01);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set phy address failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set phy address 0x01.\n");
    res = lan8720_get_phy_address(&gs_handle, &reg_check);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get phy address failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check phy address %s.\n", reg_check == 0x01 ? "ok" : "error");
    
    /* lan8720_get_symbol_error_counter test */
    lan8720_interface_debug_print("lan8720: lan8720_get_symbol_error_counter test.\n");
    
    /* get symbol error counter */
    res = lan8720_get_symbol_error_counter(&gs_handle, &cnt);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get symbol error counter failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: symbol error counter is %d.\n", cnt);
    
    /* lan8720_set_auto_mdix/lan8720_get_auto_mdix test */
    lan8720_interface_debug_print("lan8720: lan8720_set_auto_mdix/lan8720_get_auto_mdix test.\n");
    
    /* enable */
    res = lan8720_set_auto_mdix(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto mdix failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto mdix.\n");
    res = lan8720_get_auto_mdix(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto mdix failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto mdix %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_auto_mdix(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set auto mdix failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto mdix.\n");
    res = lan8720_get_auto_mdix(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto mdix failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto mdix %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_set_manual_channel_select/lan8720_get_manual_channel_select test */
    lan8720_interface_debug_print("lan8720: lan8720_set_manual_channel_select/lan8720_get_manual_channel_select test.\n");
    
    /* tx transmits, rx receives */
    res = lan8720_set_manual_channel_select(&gs_handle, LAN8720_MANUAL_CHANNEL_SELECT_MDI);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set manual channel select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set tx transmits, rx receives.\n");
    res = lan8720_get_manual_channel_select(&gs_handle, &select);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get manual channel select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check manual channel select %s.\n", select == LAN8720_MANUAL_CHANNEL_SELECT_MDI ? "ok" : "error");
    
    /* tx receives, rx transmits  */
    res = lan8720_set_manual_channel_select(&gs_handle, LAN8720_MANUAL_CHANNEL_SELECT_MDIX);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set manual channel select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: set tx receives, rx transmits.\n");
    res = lan8720_get_manual_channel_select(&gs_handle, &select);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get manual channel select failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check manual channel select %s.\n", select == LAN8720_MANUAL_CHANNEL_SELECT_MDIX ? "ok" : "error");
    
    /* lan8720_set_sqe_test_off/lan8720_get_sqe_test_off test */
    lan8720_interface_debug_print("lan8720: lan8720_set_sqe_test_off/lan8720_get_sqe_test_off test.\n");
    
    /* enable */
    res = lan8720_set_sqe_test_off(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set sqe test off failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable sqe test off.\n");
    res = lan8720_get_sqe_test_off(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get sqe test off failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check sqe test off %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable */
    res = lan8720_set_sqe_test_off(&gs_handle, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set sqe test off failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable sqe test off.\n");
    res = lan8720_get_sqe_test_off(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get sqe test off failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check sqe test off %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_get_polarity test */
    lan8720_interface_debug_print("lan8720: lan8720_get_polarity test.\n");
    
    /* get polarity */
    res = lan8720_get_polarity(&gs_handle, &polarity);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get polarity failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check polarity %s.\n", polarity == LAN8720_POLARITY_NORMAL ? "normal polarity" : "reversed polarity");
    
    /* lan8720_get_interrupt_flag test */
    lan8720_interface_debug_print("lan8720: lan8720_get_interrupt_flag test.\n");
    
    /* energy on generated */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_ENERGY_ON, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check energy on generated interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* auto negotiation complete */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_COMPLETE, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation complete interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* remote fault detected */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_REMOTE_FAULT, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check remote fault detected interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* link down */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_LINK_DOWN, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check link down interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* auto negotiation lp acknowledge */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_LP_ACK, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation lp acknowledge interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* parallel detection fault */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_PARALLEL_DETECTION_FAULT, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check parallel detection fault interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* auto negotiation page received */
    res = lan8720_get_interrupt_flag(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_PAGE_RECEIVED, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt flag failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation page received interrupt flag %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_set_interrupt_mask/lan8720_get_interrupt_mask test */
    lan8720_interface_debug_print("lan8720: lan8720_set_interrupt_mask/lan8720_get_interrupt_mask test.\n");
    
    /* enable energy on generated */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_ENERGY_ON, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable energy on generated.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_ENERGY_ON, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable energy on generated */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_ENERGY_ON, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable energy on generated.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_ENERGY_ON, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* enable auto negotiation complete */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_COMPLETE, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation complete.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_COMPLETE, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable auto negotiation complete */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_COMPLETE, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation complete.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_COMPLETE, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* enable remote fault detected */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_REMOTE_FAULT, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable remote fault detected.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_REMOTE_FAULT, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable remote fault detected */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_REMOTE_FAULT, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable remote fault detected.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_REMOTE_FAULT, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* enable link down */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_LINK_DOWN, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable link down.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_LINK_DOWN, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable link down */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_LINK_DOWN, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable link down.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_LINK_DOWN, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* enable auto negotiation lp acknowledge */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_LP_ACK, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation lp acknowledge.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_LP_ACK, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable auto negotiation lp acknowledge */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_LP_ACK, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation lp acknowledge.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_LP_ACK, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* enable parallel detection fault */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_PARALLEL_DETECTION_FAULT, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable parallel detection fault.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_PARALLEL_DETECTION_FAULT, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable parallel detection fault */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_PARALLEL_DETECTION_FAULT, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable parallel detection fault.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_PARALLEL_DETECTION_FAULT, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* enable auto negotiation page received */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_PAGE_RECEIVED, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable auto negotiation page received.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_PAGE_RECEIVED, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_TRUE ? "ok" : "error");
    
    /* disable auto negotiation page received */
    res = lan8720_set_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_PAGE_RECEIVED, LAN8720_BOOL_FALSE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: disable auto negotiation page received.\n");
    res = lan8720_get_interrupt_mask(&gs_handle, LAN8720_INTERRUPT_AUTO_NEGOTIATION_PAGE_RECEIVED, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get interrupt mask failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check interrupt mask %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* lan8720_get_auto_negotiation_done test */
    lan8720_interface_debug_print("lan8720: lan8720_get_auto_negotiation_done test.\n");
    
    /* get auto negotiation done */
    res = lan8720_get_auto_negotiation_done(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get auto negotiation done failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check auto negotiation done %s.\n", enable == LAN8720_BOOL_TRUE ? "true" : "false");
    
    /* lan8720_get_speed_indication test */
    lan8720_interface_debug_print("lan8720: lan8720_get_speed_indication test.\n");
    
    /* get speed indication */
    res = lan8720_get_speed_indication(&gs_handle, &speed_indication);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get speed indication failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check speed indication 0x%02X.\n", speed_indication);
    
    /* lan8720_set_soft_reset/lan8720_get_soft_reset test */
    lan8720_interface_debug_print("lan8720: lan8720_set_soft_reset/lan8720_get_soft_reset test.\n");
    
    /* enable soft reset */
    res = lan8720_set_soft_reset(&gs_handle, LAN8720_BOOL_TRUE);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: set soft reset failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: enable soft reset.\n");
    lan8720_interface_delay_ms(50);
    res = lan8720_get_soft_reset(&gs_handle, &enable);
    if (res != 0)
    {
        lan8720_interface_debug_print("lan8720: get soft reset failed.\n");
        (void)lan8720_deinit(&gs_handle);
        
        return 1;
    }
    lan8720_interface_debug_print("lan8720: check soft reset %s.\n", enable == LAN8720_BOOL_FALSE ? "ok" : "error");
    
    /* finish register */
    lan8720_interface_debug_print("lan8720: finish register test.\n");
    (void)lan8720_deinit(&gs_handle);
    
    return 0;
}
