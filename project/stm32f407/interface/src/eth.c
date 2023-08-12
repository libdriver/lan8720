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
 * @file      eth.c
 * @brief     eth source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-11-11
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/11/11  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "eth.h"
#include <string.h>

/**
 * @brief eth var definition
 */
ETH_HandleTypeDef g_eth_handle;                               /**< eth handle */
ETH_TxPacketConfig g_tx_config;                               /**< eth tx config */
ETH_DMADescTypeDef g_dma_rx_dscr_tab[ETH_RX_DESC_CNT];        /**< ethernet rx dma descriptors */
ETH_DMADescTypeDef g_dma_tx_dscr_tab[ETH_TX_DESC_CNT];        /**< ethernet tx dma descriptors */

/**
 * @brief     eth init
 * @param[in] *mac points to a mac buffer
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      ETH_MDIO is PA2
 *            ETH_RMII_REF_CLK is PA1
 *            ETH_RMII_CRS_DV is PA7
 *            ETH_MDC is PC1
 *            ETH_RMII_RXD0 is PC4
 *            ETH_RMII_RXD1 is PC5
 *            ETH_RMII_TX_EN is PG11
 *            ETH_RMII_TXD0 is PG13
 *            ETH_RMII_TXD1 is PG14
 *            ETH_RESET is PD3
 */
uint8_t eth_init(uint8_t mac[6])
{
    g_eth_handle.Instance = ETH;
    g_eth_handle.Init.MACAddr = mac;
    g_eth_handle.Init.MediaInterface = HAL_ETH_RMII_MODE;
    g_eth_handle.Init.TxDesc = g_dma_tx_dscr_tab;
    g_eth_handle.Init.RxDesc = g_dma_rx_dscr_tab;
    g_eth_handle.Init.RxBuffLen = ETH_RX_BUF_SIZE;
    if (HAL_ETH_Init(&g_eth_handle) != HAL_OK)
    {
        return 1;
    }
    
    /* set tx packet config common parameters */
    memset(&g_tx_config, 0, sizeof(ETH_TxPacketConfig));
    g_tx_config.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
    g_tx_config.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
    g_tx_config.CRCPadCtrl = ETH_CRC_PAD_INSERT;
    
    return 0;
}

/**
 * @brief  eth deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t eth_deinit(void)
{
    if (HAL_ETH_DeInit(&g_eth_handle) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief      eth phy read
 * @param[in]  addr is the device address
 * @param[in]  reg is the register address
 * @param[out] *data points to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note
 */
uint8_t eth_read_phy(uint8_t addr, uint8_t reg, uint16_t *data)
{
    uint32_t out;
    
    if (HAL_ETH_ReadPHYRegister(&g_eth_handle, addr, reg, &out) != HAL_OK)
    {
        return 1;
    }
    *data = out & 0xFFFF;
    
    return 0;
}

/**
 * @brief     eth phy write
 * @param[in] addr is the device waddress
 * @param[in] reg is the register address
 * @param[in] data is the set data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t eth_write_phy(uint8_t addr, uint8_t reg, uint16_t data)
{
    if (HAL_ETH_WritePHYRegister(&g_eth_handle, addr, reg, data) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief     eth write
 * @param[in] *tx_buffer points to ETH_BufferTypeDef structure
 * @param[in] *data points to a data buffer
 * @param[in] len is the set length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t eth_write(ETH_BufferTypeDef *tx_buffer, void *data, uint32_t len)
{
    uint32_t timeout = 1000;
    
    g_tx_config.Length = len;
    g_tx_config.TxBuffer = tx_buffer;
    g_tx_config.pData = data;
    if (HAL_ETH_Transmit(&g_eth_handle, &g_tx_config, timeout) != HAL_OK)
    {
        return 1;
    }
    
    return 0;
}

/**
 * @brief  eth get the handle
 * @return points to an eth handle
 * @note   none
 */
ETH_HandleTypeDef* eth_get_handle(void)
{
    return &g_eth_handle;
}
