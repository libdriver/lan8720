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
 * @file      driver_lan8720.h
 * @brief     driver lan8720 header file
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

#ifndef DRIVER_LAN8720_H
#define DRIVER_LAN8720_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup lan8720_driver lan8720 driver function
 * @brief    lan8720 driver modules
 * @{
 */

/**
 * @addtogroup lan8720_base_driver
 * @{
 */

/**
 * @brief lan8720 bool enumeration definition
 */
typedef enum
{
    LAN8720_BOOL_FALSE = 0x00,        /**< disable */
    LAN8720_BOOL_TRUE  = 0x01,        /**< enable */
} lan8720_bool_t;

/**
 * @brief lan8720 speed enumeration definition
 */
typedef enum
{
    LAN8720_SPEED_10M  = 0x00,        /**< 10mbps */
    LAN8720_SPEED_100M = 0x01,        /**< 100mbps */
} lan8720_speed_t;

/**
 * @brief lan8720 duplex enumeration definition
 */
typedef enum
{
    LAN8720_DUPLEX_HALF = 0x00,        /**< half duplex */
    LAN8720_DUPLEX_FULL = 0x01,        /**< full duplex */
} lan8720_duplex_t;

/**
 * @brief lan8720 link enumeration definition
 */
typedef enum
{
    LAN8720_LINK_DOWN = 0x00,        /**< link is down */
    LAN8720_LINK_UP   = 0x01,        /**< link is up */
} lan8720_link_t;

/**
 * @}
 */

/**
 * @addtogroup lan8720_advance_driver
 * @{
 */

/**
 * @brief lan8720 pause enumeration definition
 */
typedef enum
{
    LAN8720_PAUSE_NO         = 0x00,        /**< no pause */
    LAN8720_PAUSE_SYMMETRIC  = 0x01,        /**< symmetric pause */
    LAN8720_PAUSE_ASYMMETRIC = 0x02,        /**< asymmetric pause toward link partner */
    LAN8720_PAUSE_BOTH       = 0x03,        /**< advertise support for both symmetric pause 
                                                 and asymmetric pause toward local device */
} lan8720_pause_t;

/**
 * @brief lan8720 mode enumeration definition
 */
typedef enum
{
    LAN8720_MODE_10BASE_T_HALF_DUPLEX              = 0x00,        /**< 10base-t half duplex */
    LAN8720_MODE_10BASE_T_FULL_DUPLEX              = 0x01,        /**< 10base-t full duplex */
    LAN8720_MODE_100BASE_TX_HALF_DUPLEX            = 0x02,        /**< 100base-tx half duplex */
    LAN8720_MODE_100BASE_TX_FULL_DUPLEX            = 0x03,        /**< 100base-tx full duplex */
    LAN8720_MODE_100BASE_TX_HALF_DUPLEX_ADVERTISED = 0x04,        /**< 100base-tx half duplex is advertised */
    LAN8720_MODE_REPEATER_MODE                     = 0x05,        /**< repeater mode */
    LAN8720_MODE_POWER_DOWN_MODE                   = 0x06,        /**< power down mode */
    LAN8720_MODE_ALL_CAPABLE                       = 0x07,        /**< all capable, auto-negotiation enabled */
} lan8720_mode_t;

/**
 * @brief lan8720 manual channel select enumeration definition
 */
typedef enum
{
    LAN8720_MANUAL_CHANNEL_SELECT_MDI  = 0x00,        /**< tx transmits, rx receives */
    LAN8720_MANUAL_CHANNEL_SELECT_MDIX = 0x01,        /**< tx receives, rx transmits */
} lan8720_manual_channel_select_t;

/**
 * @brief lan8720 polarity enumeration definition
 */
typedef enum
{
    LAN8720_POLARITY_NORMAL   = 0x00,        /**< normal polarity */
    LAN8720_POLARITY_REVERSED = 0x01,        /**< reversed polarity */
} lan8720_polarity_t;

/**
 * @brief lan8720 interrupt enumeration definition
 */
typedef enum
{
    LAN8720_INTERRUPT_ENERGY_ON                      = 7,        /**< energy on generated */
    LAN8720_INTERRUPT_AUTO_NEGOTIATION_COMPLETE      = 6,        /**< auto negotiation complete */
    LAN8720_INTERRUPT_REMOTE_FAULT                   = 5,        /**< remote fault detected */
    LAN8720_INTERRUPT_LINK_DOWN                      = 4,        /**< link down(link status negated) */
    LAN8720_INTERRUPT_AUTO_NEGOTIATION_LP_ACK        = 3,        /**< auto negotiation lp acknowledge */
    LAN8720_INTERRUPT_PARALLEL_DETECTION_FAULT       = 2,        /**< parallel detection fault */
    LAN8720_INTERRUPT_AUTO_NEGOTIATION_PAGE_RECEIVED = 1,        /**< auto negotiation page received */
} lan8720_interrupt_t;

/**
 * @brief lan8720 speed indication enumeration definition
 */
typedef enum
{
    LAN8720_SPEED_INDICATION_10BASE_T_HALF_DUPLEX   = 0x01,        /**< 10base-t half duplex */
    LAN8720_SPEED_INDICATION_10BASE_T_FULL_DUPLEX   = 0x05,        /**< 10base-t full duplex */
    LAN8720_SPEED_INDICATION_100BASE_TX_HALF_DUPLEX = 0x02,        /**< 100base-tx half duplex */
    LAN8720_SPEED_INDICATION_100BASE_TX_FULL_DUPLEX = 0x06,        /**< 100bae-tx full duplex */
} lan8720_speed_indication_t;

/**
 * @brief lan8720 handle structure definition
 */
typedef struct lan8720_handle_s
{
    uint8_t smi_addr;                                                      /**< smi device address */
    uint8_t (*smi_init)(void);                                             /**< point to an smi_init function address */
    uint8_t (*smi_deinit)(void);                                           /**< point to an smi_deinit function address */
    uint8_t (*smi_read)(uint8_t addr, uint8_t reg, uint16_t *data);        /**< point to an smi_read function address */
    uint8_t (*smi_write)(uint8_t addr, uint8_t reg, uint16_t data);        /**< point to an smi_write function address */
    uint8_t (*reset_gpio_init)(void);                                      /**< point to a reset_gpio_init function address */
    uint8_t (*reset_gpio_deinit)(void);                                    /**< point to a reset_gpio_deinit function address */
    uint8_t (*reset_gpio_write)(uint8_t level);                            /**< point to a reset_gpio_write function address */
    void (*delay_ms)(uint32_t ms);                                         /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                       /**< point to a debug_print function address */
    uint8_t inited;                                                        /**< inited flag */
} lan8720_handle_t;

/**
 * @brief lan8720 information structure definition
 */
typedef struct lan8720_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[16];                /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} lan8720_info_t;

/**
 * @}
 */

/**
 * @defgroup lan8720_link_driver lan8720 link driver function
 * @brief    lan8720 link driver modules
 * @ingroup  lan8720_driver
 * @{
 */

/**
 * @brief     initialize lan8720_handle_t structure
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] STRUCTURE is lan8720_handle_t
 * @note      none
 */
#define DRIVER_LAN8720_LINK_INIT(HANDLE, STRUCTURE)           memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link smi_init function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to an smi_init function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_SMI_INIT(HANDLE, FUC)            (HANDLE)->smi_init = FUC

/**
 * @brief     link smi_deinit function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to an smi_deinit function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_SMI_DEINIT(HANDLE, FUC)          (HANDLE)->smi_deinit = FUC

/**
 * @brief     link smi_read function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to an smi_read function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_SMI_READ(HANDLE, FUC)            (HANDLE)->smi_read = FUC

/**
 * @brief     link smi_write function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to an smi_write function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_SMI_WRITE(HANDLE, FUC)           (HANDLE)->smi_write = FUC

/**
 * @brief     link reset_gpio_init function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to a reset_gpio_init function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_RESET_GPIO_INIT(HANDLE, FUC)     (HANDLE)->reset_gpio_init = FUC

/**
 * @brief     link reset_gpio_deinit function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to a reset_gpio_deinit function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_RESET_GPIO_DEINIT(HANDLE, FUC)   (HANDLE)->reset_gpio_deinit = FUC

/**
 * @brief     link reset_gpio_write function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to a reset_gpio_write function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_RESET_GPIO_WRITE(HANDLE, FUC)    (HANDLE)->reset_gpio_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_DELAY_MS(HANDLE, FUC)            (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an lan8720 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_LAN8720_LINK_DEBUG_PRINT(HANDLE, FUC)         (HANDLE)->debug_print = FUC

/**
 * @}
 */

/**
 * @defgroup lan8720_base_driver lan8720 base driver function
 * @brief    lan8720 base driver modules
 * @ingroup  lan8720_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an lan8720 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t lan8720_info(lan8720_info_t *info);

/**
 * @brief     set the chip address
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] addr is the chip address
 * @return    status code
 *            - 0 success
 *            - 2 handle is NULL
 *            - 4 addr > 0x1F
 * @note      none
 */
uint8_t lan8720_set_address(lan8720_handle_t *handle, uint8_t addr);

/**
 * @brief      get the chip address
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *addr points to a chip address
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t lan8720_get_address(lan8720_handle_t *handle, uint8_t *addr);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an lan8720 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 smi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 reset failed
 * @note      none
 */
uint8_t lan8720_init(lan8720_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an lan8720 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 smi deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 power down failed
 * @note      none
 */
uint8_t lan8720_deinit(lan8720_handle_t *handle);

/**
 * @brief     enable or disable soft reset
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set soft reset failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_soft_reset(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get soft reset status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get soft reset failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_soft_reset(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable loop back
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set loop back failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_loop_back(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get loop back status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get loop back failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_loop_back(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set speed select
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] speed is the set speed
 * @return    status code
 *            - 0 success
 *            - 1 set speed select failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_speed_select(lan8720_handle_t *handle, lan8720_speed_t speed);

/**
 * @brief      get speed select status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *speed points to a speed buffer
 * @return     status code
 *             - 0 success
 *             - 1 get speed select failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_speed_select(lan8720_handle_t *handle, lan8720_speed_t *speed);

/**
 * @brief     enable or disable auto negotiation
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto negotiation status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable power down
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_power_down(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get power down status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get power down failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_power_down(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable electrical isolation
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set electrical isolation failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_electrical_isolation(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get electrical isolation status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get electrical isolation failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_electrical_isolation(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable restart auto negotiate
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set restart auto negotiate failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_restart_auto_negotiate(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get restart auto negotiate status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get restart auto negotiate failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_restart_auto_negotiate(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set duplex mode
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] mode is the set mode
 * @return    status code
 *            - 0 success
 *            - 1 set duplex mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_duplex_mode(lan8720_handle_t *handle, lan8720_duplex_t mode);

/**
 * @brief      get duplex mode
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *mode points to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get duplex mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_duplex_mode(lan8720_handle_t *handle, lan8720_duplex_t *mode);

/**
 * @brief      get 100base t4 status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 100base t4 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_100base_t4(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get 100base tx full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 100base tx full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get 100base tx half duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 100base tx half duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_100base_tx_half_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get 10base t full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 10base t full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get 10base t half duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 10base t half duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_10base_t_half_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get 100base t2 full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 100base t2 full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_100base_t2_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get 100base t2 half duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get 100base t2 half duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_100base_t2_half_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get extended status information status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get extended status information failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_extended_status_information(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiate complete status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiate complete failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiate_complete(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get remote fault status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get remote fault failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_remote_fault(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiate ability status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiate ability failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiate_ability(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get link status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get link status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_link_status(lan8720_handle_t *handle, lan8720_link_t *status);

/**
 * @brief      get jabber detect status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get jabber detect failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_jabber_detect(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get extended capabilities status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get extended capabilities failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_extended_capabilities(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @}
 */

/**
 * @defgroup lan8720_advance_driver lan8720 advance driver function
 * @brief    lan8720 advance driver modules
 * @ingroup  lan8720_driver
 * @{
 */

/**
 * @brief     set identifier
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] phy_id is the phy id
 * @param[in] model_number is the model number
 * @param[in] revision_number is the revision number
 * @return     status code
 *             - 0 success
 *             - 1 set identifier failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 phy_id > 0x3FFFFF
 *             - 5 model_number > 0x3F
 *             - 6 revision_number > 0xF
 * @note       none
 */
uint8_t lan8720_set_identifier(lan8720_handle_t *handle, uint32_t phy_id, 
                               uint8_t model_number, uint8_t revision_number);

/**
 * @brief      get identifier
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *phy_id points to a phy id buffer
 * @param[out] *model_number points to an model number buffer
 * @param[out] *revision_number points to a revision number buffer
 * @return     status code
 *             - 0 success
 *             - 1 get identifier failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_identifier(lan8720_handle_t *handle, uint32_t *phy_id, 
                               uint8_t *model_number, uint8_t *revision_number);

/**
 * @brief     enable or disable auto negotiation advertisement remote fault
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation advertisement remote fault failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_remote_fault(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto negotiation advertisement remote fault status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation advertisement remote fault failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_remote_fault(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set auto negotiation advertisement pause
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] pause is the set pause
 * @return    status code
 *            - 0 success
 *            - 1 set_auto negotiation advertisement pause failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_pause(lan8720_handle_t *handle, lan8720_pause_t pause);

/**
 * @brief      get auto negotiation advertisement pause
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *pause points to a pause buffer
 * @return     status code
 *             - 0 success
 *             - 1 get_auto negotiation advertisement pause failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_pause(lan8720_handle_t *handle, lan8720_pause_t *pause);

/**
 * @brief     enable or disable auto negotiation advertisement 100base tx full duplex
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation advertisement 100base tx full duplex failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto negotiation advertisement 100base tx full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation advertisement 100base tx full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable auto negotiation advertisement 100base tx
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation advertisement 100base tx failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_100base_tx(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto negotiation advertisement 100base tx status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation advertisement 100base tx failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_100base_tx(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable auto negotiation advertisement 10base t full duplex
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation advertisement 10base t full duplex failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto negotiation advertisement 10base t full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation advertisement 10base t full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable auto negotiation advertisement 10base t
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation advertisement 10base t failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_10base_t(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto negotiation advertisement 10base t status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation advertisement 10base t failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_10base_t(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set auto negotiation advertisement selector field
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] selector is the set selector
 * @return    status code
 *            - 0 success
 *            - 1 set auto negotiation advertisement selector field failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 selector > 0x1F
 * @note      none
 */
uint8_t lan8720_set_auto_negotiation_advertisement_selector_field(lan8720_handle_t *handle, uint8_t selector);

/**
 * @brief      get auto negotiation advertisement selector field
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *selector points to a selector buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation advertisement selector field failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_advertisement_selector_field(lan8720_handle_t *handle, uint8_t *selector);

/**
 * @brief      get auto negotiation link partner ability next page status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability next page failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_next_page(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability acknowledge status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability acknowledge failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_acknowledge(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability remote fault status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability remote fault failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_remote_fault(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability pause status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability pause failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_pause(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability 100base t4 status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability 100base t4 failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_100base_t4(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability 100base tx full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability 100base tx full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability 100base tx status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability 100base tx failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_100base_tx(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability 10base t full duplex status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability 10base t full duplex failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability 10base t status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability 10base t failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_10base_t(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation link partner ability selector field
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *selector points to a selector buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation link partner ability selector field failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_link_partner_ability_selector_field(lan8720_handle_t *handle, uint8_t *selector);

/**
 * @brief      get auto negotiation expansion parallel detection fault status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation expansion parallel detection fault failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_expansion_parallel_detection_fault(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation expansion link partner next page able status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation expansion link partner next page able failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_expansion_link_partner_next_page_able(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation expansion next page able status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation expansion next page able failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_expansion_next_page_able(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation expansion page received status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation expansion page received failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_expansion_page_received(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation expansion link partner auto negotiation able status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation expansion link partner auto negotiation able failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_expansion_link_partner_auto_negotiation_able(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable energy detect power down mode
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set energy detect power down mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_energy_detect_power_down_mode(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get energy detect power down mode status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get energy detect power down mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_energy_detect_power_down_mode(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable far loop back
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set far loop back failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_far_loop_back(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get far loop back status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get far loop back failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_far_loop_back(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     enable or disable alternate interrupt mode
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set alternate interrupt mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_alternate_interrupt_mode(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get alternate interrupt mode status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get alternate interrupt mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_alternate_interrupt_mode(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get energy detected status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get energy detected failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_energy_detected(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set mode
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] mode is the set mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_mode(lan8720_handle_t *handle, lan8720_mode_t mode);

/**
 * @brief      get mode
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *mode points to a mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_mode(lan8720_handle_t *handle, lan8720_mode_t *mode);

/**
 * @brief     set phy address
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] addr is the phy address
 * @return    status code
 *            - 0 success
 *            - 1 set phy address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 addr > 0x1F
 * @note      none
 */
uint8_t lan8720_set_phy_address(lan8720_handle_t *handle, uint8_t addr);

/**
 * @brief      get phy address
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *addr points to a phy address buffer
 * @return     status code
 *             - 0 success
 *             - 1 get phy address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_phy_address(lan8720_handle_t *handle, uint8_t *addr);

/**
 * @brief      get symbol error counter
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *cnt points to a counter buffer
 * @return     status code
 *             - 0 success
 *             - 1 get symbol error counter failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_symbol_error_counter(lan8720_handle_t *handle, uint16_t *cnt);

/**
 * @brief     enable or disable auto mdix
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto mdix failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_auto_mdix(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get auto mdix status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto mdix failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_mdix(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set manual channel select
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] select is the manual channel select
 * @return    status code
 *            - 0 success
 *            - 1 set manual channel select failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_manual_channel_select(lan8720_handle_t *handle, lan8720_manual_channel_select_t select);

/**
 * @brief      get manual channel select
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *select points to a manual channel select buffer
 * @return     status code
 *             - 0 success
 *             - 1 get manual channel select failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_manual_channel_select(lan8720_handle_t *handle, lan8720_manual_channel_select_t *select);

/**
 * @brief     enable or disable sqe test off
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set sqe test off failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_sqe_test_off(lan8720_handle_t *handle, lan8720_bool_t enable);

/**
 * @brief      get sqe test off status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get sqe test off failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_sqe_test_off(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief     set polarity
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] polarity is the set polarity
 * @return    status code
 *            - 0 success
 *            - 1 set polarity failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_polarity(lan8720_handle_t *handle, lan8720_polarity_t polarity);

/**
 * @brief      get polarity
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *polarity points to a polarity buffer
 * @return     status code
 *             - 0 success
 *             - 1 get polarity failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_polarity(lan8720_handle_t *handle, lan8720_polarity_t *polarity);

/**
 * @brief      get interrupt flag status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[in]  interrupt is the set interrupt
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt flag failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_interrupt_flag(lan8720_handle_t *handle, lan8720_interrupt_t interrupt, lan8720_bool_t *enable);

/**
 * @brief     set interrupt mask
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] interrupt is the set interrupt
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set interrupt mask failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t lan8720_set_interrupt_mask(lan8720_handle_t *handle, lan8720_interrupt_t interrupt, lan8720_bool_t enable);

/**
 * @brief      get interrupt mask status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[in]  interrupt is the set interrupt
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get interrupt mask failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_interrupt_mask(lan8720_handle_t *handle, lan8720_interrupt_t interrupt, lan8720_bool_t *enable);

/**
 * @brief      get auto negotiation done status
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto negotiation done failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_auto_negotiation_done(lan8720_handle_t *handle, lan8720_bool_t *enable);

/**
 * @brief      get speed indication
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *speed points to a speed buffer
 * @return     status code
 *             - 0 success
 *             - 1 get speed indication failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t lan8720_get_speed_indication(lan8720_handle_t *handle, lan8720_speed_indication_t *speed);

/**
 * @}
 */

/**
 * @defgroup lan8720_extend_driver lan8720 extend driver function
 * @brief    lan8720 extend driver modules
 * @ingroup  lan8720_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] reg is the register address
 * @param[in] value is the data written to the register
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 reg > 0x1F
 * @note      none
 */
uint8_t lan8720_set_reg(lan8720_handle_t *handle, uint8_t reg, uint16_t value);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[in]  reg is the register address
 * @param[out] *value points to a read data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 reg > 0x1F
 * @note       none
 */
uint8_t lan8720_get_reg(lan8720_handle_t *handle, uint8_t reg, uint16_t *value);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
