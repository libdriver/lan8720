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
 * @file      driver_lan8720.c
 * @brief     driver lan8720 source file
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

#include "driver_lan8720.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Microchip LAN8720"        /**< chip name */
#define MANUFACTURER_NAME         "Microchip"                /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.0f                       /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                       /**< chip max supply voltage */
#define MAX_CURRENT               60.0f                      /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                     /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                      /**< chip max operating temperature */
#define DRIVER_VERSION            1000                       /**< driver version */

/**
 * @brief chip register definition
 */
#define LAN8720_REG_BASIC_CONTROL                              0x00        /**< basic control register */
#define LAN8720_REG_BASIC_STATUS                               0x01        /**< basic status register */
#define LAN8720_REG_PHY_IDENTIFIER_1                           0x02        /**< phy identifier 1 register */
#define LAN8720_REG_PHY_IDENTIFIER_2                           0x03        /**< phy identifier 2 register */
#define LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT             0x04        /**< auto-negotiation advertisement register */
#define LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY      0x05        /**< auto-negotiation link partner ability register */
#define LAN8720_REG_AUTO_NEGOTIATION_EXPANSION                 0x06        /**< auto-negotiation expansion register */
#define LAN8720_REG_MODE_CONTROL_STATUS                        0x11        /**< mode control/status register */
#define LAN8720_REG_SPECIAL_MODES                              0x12        /**< special modes register */
#define LAN8720_REG_SYMBOL_ERROR_COUNTER_REGISTER              0x1A        /**< symbol error counter register */
#define LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS         0x1B        /**< control/status indication register */
#define LAN8720_REG_INTERRUPT_SOURCE_FLAG                      0x1D        /**< interrupt source register */
#define LAN8720_REG_INTERRUPT_MASK                             0x1E        /**< interrupt mask register */
#define LAN8720_REG_PHY_SPECIAL_CONTROL_STATUS                 0x1F        /**< phy special control/status register */

/**
 * @brief      read bytes
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[in]  reg is the register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
static uint8_t a_lan8720_smi_read(lan8720_handle_t *handle, uint8_t reg, uint16_t *data)
{
    if (handle->smi_read(handle->smi_addr, reg, data) != 0)        /* read data */
    {
        return 1;                                                  /* return error */
    }

    return 0;                                                      /* success return 0 */
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an lan8720 handle structure
 * @param[in] reg is the register address
 * @param[in] data is the sent data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
static uint8_t a_lan8720_smi_write(lan8720_handle_t *handle, uint8_t reg, uint16_t data)
{
    if (handle->smi_write(handle->smi_addr, reg, data) != 0)       /* write data */
    {
        return 1;                                                  /* return error */
    }

    return 0;                                                      /* success return 0 */
}

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
uint8_t lan8720_set_address(lan8720_handle_t *handle, uint8_t addr)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (addr > 0x1F)                                          /* check addr */
    {
        handle->debug_print("lan8720: addr > 0x1F.\n");       /* addr > 0x1F */

        return 4;                                             /* return error */
    }

    handle->smi_addr = addr;                                  /* set smi addr */

    return 0;                                                 /* success return 0 */
}

/**
 * @brief      get the chip address
 * @param[in]  *handle points to an lan8720 handle structure
 * @param[out] *addr points to a chip address
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t lan8720_get_address(lan8720_handle_t *handle, uint8_t *addr)
{
    if (handle == NULL)              /* check handle */
    {
        return 2;                    /* return error */
    }

    *addr = handle->smi_addr;        /* get smi address */

    return 0;                        /* success return 0 */
}

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
uint8_t lan8720_init(lan8720_handle_t *handle)
{
    uint16_t config;

    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                                /* check debug_print */
    {
        return 3;                                                                   /* return error */
    }
    if (handle->smi_init == NULL)                                                   /* check smi_init */
    {
        handle->debug_print("lan8720: smi_init is null.\n");                        /* smi_init is null */

        return 3;                                                                   /* return error */
    }
    if (handle->smi_deinit == NULL)                                                 /* check smi_deinit */
    {
        handle->debug_print("lan8720: smi_deinit is null.\n");                      /* smi_deinit is null */

        return 3;                                                                   /* return error */
    }
    if (handle->smi_read == NULL)                                                   /* check smi_read */
    {
        handle->debug_print("lan8720: smi_read is null.\n");                        /* smi_read is null */

        return 3;                                                                   /* return error */
    }
    if (handle->smi_write == NULL)                                                  /* check smi_write */
    {
        handle->debug_print("lan8720: smi_write is null.\n");                       /* smi_write is null */

        return 3;                                                                   /* return error */
    }
    if (handle->reset_gpio_init == NULL)                                            /* check reset_gpio_init */
    {
        handle->debug_print("lan8720: reset_gpio_init is null.\n");                 /* reset_gpio_init is null */

        return 3;                                                                   /* return error */
    }
    if (handle->reset_gpio_deinit == NULL)                                          /* check reset_gpio_deinit */
    {
        handle->debug_print("lan8720: reset_gpio_deinit is null.\n");               /* reset_gpio_deinit is null */

        return 3;                                                                   /* return error */
    }
    if (handle->reset_gpio_write == NULL)                                           /* check reset_gpio_write */
    {
        handle->debug_print("lan8720: reset_gpio_write is null.\n");                /* reset_gpio_write is null */

        return 3;                                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                                   /* check delay_ms */
    {
        handle->debug_print("lan8720: delay_ms is null.\n");                        /* delay_ms is null */

        return 3;                                                                   /* return error */
    }

    if (handle->reset_gpio_init() != 0)                                             /* reset gpio init */
    {
        handle->debug_print("lan8720: reset gpio init failed.\n");                  /* reset gpio init failed */

        return 1;                                                                   /* return error */
    }
    if (handle->reset_gpio_write(0) != 0)                                           /* set low */
    {
        handle->debug_print("lan8720: reset gpio write failed.\n");                 /* reset gpio write failed */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 1;                                                                   /* return error */
    }
    handle->delay_ms(50);                                                           /* delay 50 ms */
    if (handle->reset_gpio_write(1) != 0)                                           /* set high */
    {
        handle->debug_print("lan8720: reset gpio write failed.\n");                 /* reset gpio write failed */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 1;                                                                   /* return error */
    }
    handle->delay_ms(10);                                                           /* delay 10 ms */
    if (handle->smi_init() != 0)                                                    /* smi init */
    {
        handle->debug_print("lan8720: smi init failed.\n");                         /* smi init failed */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 1;                                                                   /* return error */
    }

    if (a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config) != 0)        /* read basic control */
    {
        handle->debug_print("lan8720: reset failed.\n");                            /* reset failed */
        (void)handle->smi_deinit();                                                 /* smi deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 4;                                                                   /* return error */
    }
    config &= ~(1 << 15);                                                           /* clear config */
    config |= 1 << 15;                                                              /* set soft reset */
    if (a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config) != 0)        /* write basic control */
    {
        handle->debug_print("lan8720: reset failed.\n");                            /* reset failed */
        (void)handle->smi_deinit();                                                 /* smi deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 4;                                                                   /* return error */
    }
    handle->delay_ms(50);                                                           /* delay 50 ms */
    if (a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config) != 0)        /* read basic control */
    {
        handle->debug_print("lan8720: reset failed.\n");                            /* reset failed */
        (void)handle->smi_deinit();                                                 /* smi deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 4;                                                                   /* return error */
    }
    if ((config & (1 <<15)) != 0)                                                   /* check self-clear bit */
    {
        handle->debug_print("lan8720: reset failed.\n");                            /* reset failed */
        (void)handle->smi_deinit();                                                 /* smi deinit */
        (void)handle->reset_gpio_deinit();                                          /* reset gpio deinit */

        return 4;                                                                   /* return error */
    }
    handle->inited = 1;                                                             /* flag inited */

    return 0;                                                                       /* success return 0 */
}

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
uint8_t lan8720_deinit(lan8720_handle_t *handle)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }

    if (a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config) != 0)        /* read basic control */
    {
        handle->debug_print("lan8720: reset failed.\n");                            /* reset failed */

        return 4;                                                                   /* return error */
    }
    config &= ~(1 << 11);                                                           /* clear config */
    config |= 1 << 11;                                                              /* set soft reset */
    if (a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config) != 0)        /* write basic control */
    {
        handle->debug_print("lan8720: reset failed.\n");                            /* reset failed */

        return 4;                                                                   /* return error */
    }
    res = handle->smi_deinit();                                                     /* close smi */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("lan8720: smi deinit failed.\n");                       /* smi deinit failed */

        return 1;                                                                   /* return error */
    }
    res = handle->reset_gpio_deinit();                                              /* close reset gpio */
    if (res != 0)                                                                   /* check the result */
    {
        handle->debug_print("lan8720: reset gpio deinit failed.\n");                /* reset gpio deinit failed */

        return 1;                                                                   /* return error */
    }
    handle->inited = 0;                                                             /* flag close */

    return 0;                                                                       /* success return 0 */
}

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
uint8_t lan8720_set_soft_reset(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 15);                                                        /* clear config */
    config |= enable << 15;                                                      /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_soft_reset(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 15) & 0x01);                           /* get the bool */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_loop_back(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 14);                                                        /* clear config */
    config |= enable << 14;                                                      /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_loop_back(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 14) & 0x01);                           /* get the bool */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_speed_select(lan8720_handle_t *handle, lan8720_speed_t speed)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 13);                                                        /* clear config */
    config |= speed << 13;                                                       /* set speed */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_speed_select(lan8720_handle_t *handle, lan8720_speed_t *speed)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *speed = (lan8720_speed_t)((config >> 13) & 0x01);                           /* get the speed */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 12);                                                        /* clear config */
    config |= enable << 12;                                                      /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 12) & 0x01);                           /* get the bool */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_power_down(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 11);                                                        /* clear config */
    config |= enable << 11;                                                      /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_power_down(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 11) & 0x01);                           /* get the bool */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_electrical_isolation(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 10);                                                        /* clear config */
    config |= enable << 10;                                                      /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_electrical_isolation(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 10) & 0x01);                           /* get the bool */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_restart_auto_negotiate(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 9);                                                         /* clear config */
    config |= enable << 9;                                                       /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_restart_auto_negotiate(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 9) & 0x01);                            /* get the bool */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_duplex_mode(lan8720_handle_t *handle, lan8720_duplex_t mode)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    config &= ~(1 << 8);                                                         /* clear config */
    config |= mode << 8;                                                         /* set mode */
    res = a_lan8720_smi_write(handle, LAN8720_REG_BASIC_CONTROL, config);        /* write basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write basic control failed.\n");           /* write basic control failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_duplex_mode(lan8720_handle_t *handle, lan8720_duplex_t *mode)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_CONTROL, &config);        /* read basic control */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read basic control failed.\n");            /* read basic control failed */

        return 1;                                                                /* return error */
    }
    *mode = (lan8720_duplex_t)((config >> 8) & 0x01);                            /* get the mode */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_100base_t4(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 15) & 0x01);                          /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 14) & 0x01);                          /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_100base_tx_half_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 13) & 0x01);                          /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 12) & 0x01);                          /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_10base_t_half_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 11) & 0x01);                          /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_100base_t2_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 10) & 0x01);                          /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_100base_t2_half_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 9) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_extended_status_information(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 8) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiate_complete(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 5) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_remote_fault(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 4) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiate_ability(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 3) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_link_status(lan8720_handle_t *handle, lan8720_link_t *status)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *status = (lan8720_link_t)((config >> 2) & 0x01);                           /* get the link status */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_jabber_detect(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 1) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
uint8_t lan8720_get_extended_capabilities(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                         /* check handle */
    {
        return 2;                                                               /* return error */
    }
    if (handle->inited != 1)                                                    /* check handle initialization */
    {
        return 3;                                                               /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_BASIC_STATUS, &config);        /* read status control */
    if (res != 0)                                                               /* check result */
    {
        handle->debug_print("lan8720: read status control failed.\n");          /* read status control failed */

        return 1;                                                               /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 0) & 0x01);                           /* get the bool */

    return 0;                                                                   /* success return 0 */
}

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
                               uint8_t model_number, uint8_t revision_number)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    if (phy_id > 0x3FFFFF)                                                       /* check phy_id */
    {
        handle->debug_print("lan8720: phy_id > 0x3FFFFF.\n");                    /* phy_id > 0x3FFFFF */

        return 4;                                                                /* return error */
    }
    if (model_number > 0x3F)                                                     /* check model_number */
    {
        handle->debug_print("lan8720: model_number > 0x3F.\n");                  /* model_number > 0x3F */

        return 5;                                                                /* return error */
    }
    if (revision_number > 0xF)                                                   /* check revision_number */
    {
        handle->debug_print("lan8720: revision_number > 0xF.\n");                /* revision_number > 0xF*/

        return 6;                                                                /* return error */
    }

    config = phy_id & 0xFFFFL;                                                   /* set phy id bit3 - bit18 */
    res = a_lan8720_smi_write(handle, LAN8720_REG_PHY_IDENTIFIER_1, config);     /* write phy identifier 1 */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write phy identifier 1 failed.\n");        /* write phy identifier 1 failed */

        return 1;                                                                /* return error */
    }

    config = (((phy_id >> 16) & 0x3F) << 10)
             | ((model_number & 0x3F) << 4) | (revision_number & 0xF);           /* set phy id bit`19 - bit24, model number, revision_number number */
    res = a_lan8720_smi_write(handle, LAN8720_REG_PHY_IDENTIFIER_2, config);     /* write phy identifier 2 */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write phy identifier 2 failed.\n");        /* write phy identifier 2 failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
                               uint8_t *model_number, uint8_t *revision_number)
{
    uint8_t res;
    uint16_t config1;
    uint16_t config2;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_PHY_IDENTIFIER_1, &config1);    /* read phy identifier 1 */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read phy identifier 1 failed.\n");         /* read phy identifier 1 failed */

        return 1;                                                                /* return error */
    }
    res = a_lan8720_smi_read(handle, LAN8720_REG_PHY_IDENTIFIER_2, &config2);    /* read phy identifier 2 */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read phy identifier 2 failed.\n");         /* read phy identifier 2 failed */

        return 1;                                                                /* return error */
    }
    *phy_id = config1 | (uint32_t)((config2 >> 10) & 0x3F) << 16;                /* get phy_id */
    *model_number = (config2 >> 4) & 0x3F;                                       /* get model_number */
    *revision_number = (config2 >> 0) & 0xF;                                     /* get revision_number */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_remote_fault(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(1 << 13);                                                                         /* clear config */
    config |= enable << 13;                                                                       /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_remote_fault(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 13) & 0x01);                                            /* get the bool */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_pause(lan8720_handle_t *handle, lan8720_pause_t pause)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(3 << 10);                                                                         /* clear config */
    config |= pause << 10;                                                                        /* set pause */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_pause(lan8720_handle_t *handle, lan8720_pause_t *pause)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *pause = (lan8720_pause_t)((config >> 10) & 0x03);                                            /* get the pause */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(1 << 8);                                                                          /* clear config */
    config |= enable << 8;                                                                        /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 8) & 0x01);                                             /* get the bool */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_100base_tx(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(1 << 7);                                                                          /* clear config */
    config |= enable << 7;                                                                        /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_100base_tx(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 7) & 0x01);                                             /* get the bool */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(1 << 6);                                                                          /* clear config */
    config |= enable << 6;                                                                        /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 6) & 0x01);                                             /* get the bool */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_10base_t(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(1 << 5);                                                                          /* clear config */
    config |= enable << 5;                                                                        /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_10base_t(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 5) & 0x01);                                             /* get the bool */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_set_auto_negotiation_advertisement_selector_field(lan8720_handle_t *handle, uint8_t selector)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }
    if (selector > 0x1F)                                                                          /* check selector */
    {
        handle->debug_print("lan8720: selector > 0x1F.\n");                                       /* selector > 0x1F */

        return 4;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    config &= ~(0x1F << 0);                                                                       /* clear config */
    config |= selector << 0;                                                                      /* set selector */
    res = a_lan8720_smi_write(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, config);        /* write auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: write auto negotiation advertisement failed.\n");           /* write auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_advertisement_selector_field(lan8720_handle_t *handle, uint8_t *selector)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                           /* check handle */
    {
        return 2;                                                                                 /* return error */
    }
    if (handle->inited != 1)                                                                      /* check handle initialization */
    {
        return 3;                                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_ADVERTISEMENT, &config);        /* read auto negotiation advertisement */
    if (res != 0)                                                                                 /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation advertisement failed.\n");            /* read auto negotiation advertisement failed */

        return 1;                                                                                 /* return error */
    }
    *selector = (config) & 0x1F;                                                                  /*get selector */

    return 0;                                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_next_page(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 15) & 0x01);                                                   /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_acknowledge(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 14) & 0x01);                                                   /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_remote_fault(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 13) & 0x01);                                                   /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_pause(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 10) & 0x01);                                                   /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_100base_t4(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 9) & 0x01);                                                    /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_100base_tx_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 8) & 0x01);                                                    /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_100base_tx(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 7) & 0x01);                                                    /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_10base_t_full_duplex(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 6) & 0x01);                                                    /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_10base_t(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 5) & 0x01);                                                    /* get the bool */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_link_partner_ability_selector_field(lan8720_handle_t *handle, uint8_t *selector)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                                  /* check handle */
    {
        return 2;                                                                                        /* return error */
    }
    if (handle->inited != 1)                                                                             /* check handle initialization */
    {
        return 3;                                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_LINK_PARTNER_ABILITY, &config);        /* read auto negotiation link partner ability */
    if (res != 0)                                                                                        /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation link partner ability failed.\n");            /* read auto negotiation link partner ability failed */

        return 1;                                                                                        /* return error */
    }
    *selector = config & 0x1F;                                                                           /* get the selector */

    return 0;                                                                                            /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_expansion_parallel_detection_fault(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_EXPANSION, &config);        /* read auto negotiation expansion */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation expansion failed.\n");            /* read auto negotiation expansion failed */

        return 1;                                                                             /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 4) & 0x01);                                         /* get the bool */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_expansion_link_partner_next_page_able(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_EXPANSION, &config);        /* read auto negotiation expansion */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation expansion failed.\n");            /* read auto negotiation expansion failed */

        return 1;                                                                             /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 3) & 0x01);                                         /* get the bool */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_expansion_next_page_able(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_EXPANSION, &config);        /* read auto negotiation expansion */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation expansion failed.\n");            /* read auto negotiation expansion failed */

        return 1;                                                                             /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 2) & 0x01);                                         /* get the bool */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_expansion_page_received(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_EXPANSION, &config);        /* read auto negotiation expansion */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation expansion failed.\n");            /* read auto negotiation expansion failed */

        return 1;                                                                             /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 1) & 0x01);                                         /* get the bool */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_expansion_link_partner_auto_negotiation_able(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_AUTO_NEGOTIATION_EXPANSION, &config);        /* read auto negotiation expansion */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read auto negotiation expansion failed.\n");            /* read auto negotiation expansion failed */

        return 1;                                                                             /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 0) & 0x01);                                         /* get the bool */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_set_energy_detect_power_down_mode(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    config &= ~(1 << 13);                                                              /* clear config */
    config |= enable << 13;                                                            /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_MODE_CONTROL_STATUS, config);        /* write mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: write mode control status failed.\n");           /* write mode control status failed */

        return 1;                                                                      /* return error */
    }

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_get_energy_detect_power_down_mode(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 13) & 0x01);                                 /* get the bool */

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_set_far_loop_back(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    config &= ~(1 << 9);                                                               /* clear config */
    config |= enable << 9;                                                             /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_MODE_CONTROL_STATUS, config);        /* write mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: write mode control status failed.\n");           /* write mode control status failed */

        return 1;                                                                      /* return error */
    }

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_get_far_loop_back(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 9) & 0x01);                                  /* get the bool */

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_set_alternate_interrupt_mode(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    config &= ~(1 << 6);                                                               /* clear config */
    config |= enable << 6;                                                             /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_MODE_CONTROL_STATUS, config);        /* write mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: write mode control status failed.\n");           /* write mode control status failed */

        return 1;                                                                      /* return error */
    }

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_get_alternate_interrupt_mode(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 6) & 0x01);                                  /* get the bool */

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_get_energy_detected(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_MODE_CONTROL_STATUS, &config);        /* read mode control status */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("lan8720: read mode control status failed.\n");            /* read mode control status failed */

        return 1;                                                                      /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 1) & 0x01);                                  /* get the bool */

    return 0;                                                                          /* success return 0 */
}

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
uint8_t lan8720_set_mode(lan8720_handle_t *handle, lan8720_mode_t mode)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_MODES, &config);        /* read special modes */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read special modes failed.\n");            /* read special modes failed */

        return 1;                                                                /* return error */
    }
    config &= ~(0x07 << 5);                                                      /* clear config */
    config |= mode << 5;                                                         /* set mode */
    res = a_lan8720_smi_write(handle, LAN8720_REG_SPECIAL_MODES, config);        /* write special modes */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write special modes failed.\n");           /* write special modes failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_mode(lan8720_handle_t *handle, lan8720_mode_t *mode)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_MODES, &config);        /* read special modes */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read special modes failed.\n");            /* read special modes failed */

        return 1;                                                                /* return error */
    }
    *mode = (lan8720_mode_t)((config >> 5) & 0x07);                              /* get the mode */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_phy_address(lan8720_handle_t *handle, uint8_t addr)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    if (addr > 0x1F)                                                             /* check result */
    {
        handle->debug_print("lan8720: addr > 0x1F.\n");                          /* addr > 0x1F */

        return 4;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_MODES, &config);        /* read special modes */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read special modes failed.\n");            /* read special modes failed */

        return 1;                                                                /* return error */
    }
    config &= ~(0x1F << 0);                                                      /* clear config */
    config |= addr << 0;                                                         /* set addr */
    res = a_lan8720_smi_write(handle, LAN8720_REG_SPECIAL_MODES, config);        /* write special modes */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: write special modes failed.\n");           /* write special modes failed */

        return 1;                                                                /* return error */
    }

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_phy_address(lan8720_handle_t *handle, uint8_t *addr)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_MODES, &config);        /* read special modes */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("lan8720: read special modes failed.\n");            /* read special modes failed */

        return 1;                                                                /* return error */
    }
    *addr = config & 0x1F;                                                       /* get addr */

    return 0;                                                                    /* success return 0 */
}

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
uint8_t lan8720_get_symbol_error_counter(lan8720_handle_t *handle, uint16_t *cnt)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SYMBOL_ERROR_COUNTER_REGISTER, &config);        /* read symbol error counter */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("lan8720: read symbol error counter failed.\n");                     /* read symbol error counter failed */

        return 1;                                                                                /* return error */
    }
    *cnt = config;                                                                               /* get cnt */

    return 0;                                                                                    /* success return 0 */
}

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
uint8_t lan8720_set_auto_mdix(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    config &= ~(1 << 15);                                                                             /* clear config */
    config |= enable << 15;                                                                           /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, config);        /* write control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: write control status indication failed.\n");                    /* write control status indication failed */

        return 1;                                                                                     /* return error */
    }

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_get_auto_mdix(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 15) & 0x01);                                                /* get the bool */

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_set_manual_channel_select(lan8720_handle_t *handle, lan8720_manual_channel_select_t select)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    config &= ~(1 << 13);                                                                             /* clear config */
    config |= select << 13;                                                                           /* set select */
    res = a_lan8720_smi_write(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, config);        /* write control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: write control status indication failed.\n");                    /* write control status indication failed */

        return 1;                                                                                     /* return error */
    }

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_get_manual_channel_select(lan8720_handle_t *handle, lan8720_manual_channel_select_t *select)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    *select = (lan8720_manual_channel_select_t)((config >> 13) & 0x01);                               /* get select */

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_set_sqe_test_off(lan8720_handle_t *handle, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    config &= ~(1 << 11);                                                                             /* clear config */
    config |= enable << 11;                                                                           /* set bool */
    res = a_lan8720_smi_write(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, config);        /* write control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: write control status indication failed.\n");                    /* write control status indication failed */

        return 1;                                                                                     /* return error */
    }

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_get_sqe_test_off(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 11) & 0x01);                                                /* get the bool */

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_get_polarity(lan8720_handle_t *handle, lan8720_polarity_t *polarity)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                               /* check handle */
    {
        return 2;                                                                                     /* return error */
    }
    if (handle->inited != 1)                                                                          /* check handle initialization */
    {
        return 3;                                                                                     /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_SPECIAL_CONTROL_STATUS_INDICATIONS, &config);        /* read control status indication */
    if (res != 0)                                                                                     /* check result */
    {
        handle->debug_print("lan8720: read control status indication failed.\n");                     /* read control status indication failed */

        return 1;                                                                                     /* return error */
    }
    *polarity = (lan8720_polarity_t)((config >> 4) & 0x01);                                           /* get the polarity */

    return 0;                                                                                         /* success return 0 */
}

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
uint8_t lan8720_get_interrupt_flag(lan8720_handle_t *handle, lan8720_interrupt_t interrupt, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                  /* check handle */
    {
        return 2;                                                                        /* return error */
    }
    if (handle->inited != 1)                                                             /* check handle initialization */
    {
        return 3;                                                                        /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_INTERRUPT_SOURCE_FLAG, &config);        /* read interrupt source */
    if (res != 0)                                                                        /* check result */
    {
        handle->debug_print("lan8720: read interrupt source failed.\n");                 /* read interrupt source failed */

        return 1;                                                                        /* return error */
    }
    *enable = (lan8720_bool_t)((config >> interrupt) & 0x01);                            /* get the bool */

    return 0;                                                                            /* success return 0 */
}

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
uint8_t lan8720_set_interrupt_mask(lan8720_handle_t *handle, lan8720_interrupt_t interrupt, lan8720_bool_t enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_INTERRUPT_MASK, &config);        /* read interrupt mask */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("lan8720: read interrupt mask failed.\n");            /* read interrupt mask failed */

        return 1;                                                                 /* return error */
    }
    config &= ~(1 << interrupt);                                                  /* clear config */
    config |= enable << interrupt;                                                /* set polarity */
    res = a_lan8720_smi_write(handle, LAN8720_REG_INTERRUPT_MASK, config);        /* write interrupt mask */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("lan8720: write interrupt mask failed.\n");           /* write interrupt mask failed */

        return 1;                                                                 /* return error */
    }

    return 0;                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_interrupt_mask(lan8720_handle_t *handle, lan8720_interrupt_t interrupt, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_INTERRUPT_MASK, &config);        /* read interrupt mask */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("lan8720: read interrupt mask failed.\n");            /* read interrupt mask failed */

        return 1;                                                                 /* return error */
    }
    *enable = (lan8720_bool_t)((config >> interrupt) & 0x01);                     /* get the bool */

    return 0;                                                                     /* success return 0 */
}

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
uint8_t lan8720_get_auto_negotiation_done(lan8720_handle_t *handle, lan8720_bool_t *enable)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_PHY_SPECIAL_CONTROL_STATUS, &config);        /* read phy special control status */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read phy special control status failed.\n");            /* read phy special control status failed */

        return 1;                                                                             /* return error */
    }
    *enable = (lan8720_bool_t)((config >> 12) & 0x01);                                        /* get the bool */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_get_speed_indication(lan8720_handle_t *handle, lan8720_speed_indication_t *speed)
{
    uint8_t res;
    uint16_t config;

    if (handle == NULL)                                                                       /* check handle */
    {
        return 2;                                                                             /* return error */
    }
    if (handle->inited != 1)                                                                  /* check handle initialization */
    {
        return 3;                                                                             /* return error */
    }

    res = a_lan8720_smi_read(handle, LAN8720_REG_PHY_SPECIAL_CONTROL_STATUS, &config);        /* read phy special control status */
    if (res != 0)                                                                             /* check result */
    {
        handle->debug_print("lan8720: read phy special control status failed.\n");            /* read phy special control status failed */

        return 1;                                                                             /* return error */
    }
    *speed = (lan8720_speed_indication_t)((config >> 2) & 0x07);                              /* get the speed indication */

    return 0;                                                                                 /* success return 0 */
}

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
uint8_t lan8720_set_reg(lan8720_handle_t *handle, uint8_t reg, uint16_t value)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
    if (reg > 0x1F)                                           /* check reg */
    {
        handle->debug_print("lan8720: reg > 0x1F.\n");        /* reg > 0x1F */

        return 4;                                             /* return error */
    }

    return a_lan8720_smi_write(handle, reg, value);           /* write reg */
}

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
uint8_t lan8720_get_reg(lan8720_handle_t *handle, uint8_t reg, uint16_t *value)
{
    if (handle == NULL)                                       /* check handle */
    {
        return 2;                                             /* return error */
    }
    if (handle->inited != 1)                                  /* check handle initialization */
    {
        return 3;                                             /* return error */
    }
    if (reg > 0x1F)                                           /* check reg */
    {
        handle->debug_print("lan8720: reg > 0x1F.\n");        /* reg > 0x1F */

        return 4;                                             /* return error */
    }

    return a_lan8720_smi_read(handle, reg, value);            /* read reg */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an lan8720 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t lan8720_info(lan8720_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }

    memset(info, 0, sizeof(lan8720_info_t));                        /* initialize lan8720 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SMI RMII", 16);                       /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */

    return 0;                                                       /* success return 0 */
}
