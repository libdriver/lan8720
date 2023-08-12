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
 * @file      main.c
 * @brief     main source file
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
#include "driver_lan8720_basic.h"
#include "app_lwip.h"
#include "shell.h"
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "getopt.h"
#include <stdlib.h>

/**
 * @brief global var definition
 */
uint8_t g_buf[256];             /**< uart buffer */
volatile uint16_t g_len;        /**< uart buffer length */

/**
 * @brief     lan8720 full function
 * @param[in] argc is arg numbers
 * @param[in] **argv is the arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t lan8720(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    char short_options[] = "hipe:t:";
    struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"addr", required_argument, NULL, 1},
        {"name", required_argument, NULL, 2},
        {"operate", required_argument, NULL, 3},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    char name[33] = "www.bing.com";
    uint8_t addr = 0x01;
    uint8_t operate = 0x02;

    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }

    /* init 0 */
    optind = 0;

    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);

        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");

                break;
            }

            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");

                break;
            }

            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");

                break;
            }

            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);

                break;
            }

            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);

                break;
            }

            /* addr */
            case 1 :
            {
                /* set the addr pin */
                addr = atoi(optarg);
                
                break;
            }

            /* name */
            case 2 :
            {
                /* set the name */
                memset(name, 0, sizeof(char) * 33);
                snprintf(name, 32, "%s", optarg);
                
                break;
            }

            /* operate */
            case 3 :
            {
                if (strcmp(optarg, "init") == 0)
                {
                    operate = 0;
                }
                else if (strcmp(optarg, "dns") == 0)
                {
                    operate = 1;
                }
                else
                {
                    return 5;
                }

                break;
            }

            /* the end */
            case -1 :
            {
                break;
            }

            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run the reg test */
        if (lan8720_register_test(addr) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_net", type) == 0)
    {
        if (operate == 0)
        {
            /* initialize the lwip stack */
            eth_set_address(addr);
            lwip_init();
            netif_config();
            
            lan8720_interface_debug_print("start dhcp.\n");
            
            return 0;
        }
        else if (operate == 1)
        {
            ip_addr_t ip_addr;
            
            /* run dns */
            err_t err = dns_gethostbyname(name, &ip_addr, NULL, NULL);
            if (err == ERR_OK)
            {
                char output[32] = {0};
                
                ipaddr_ntoa_r(&ip_addr, output, 32);
                lan8720_interface_debug_print("%s dns: %s\n", name, output);
            }
            else
            {
                lan8720_interface_debug_print("dns error.\n");
            }
            
            return 0;
        }
        else
        {
            lan8720_interface_debug_print("operate is invalid:\n");
            
            return 5;
        }
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        lan8720_interface_debug_print("Usage:\n");
        lan8720_interface_debug_print("  lan8720 (-i | --information)\n");
        lan8720_interface_debug_print("  lan8720 (-h | --help)\n");
        lan8720_interface_debug_print("  lan8720 (-p | --port)\n");
        lan8720_interface_debug_print("  lan8720 (-t reg | --test=reg) [--addr=<num>]\n");
        lan8720_interface_debug_print("  lan8720 (-e net | --example=net) (--operate=<init | dns>) [--addr=<num>] [--name=<domain>]\n");
        lan8720_interface_debug_print("\n");
        lan8720_interface_debug_print("Options:\n");
        lan8720_interface_debug_print("      --addr=<num>                  Set the chip address number.([default: 1])\n");
        lan8720_interface_debug_print("  -e <net>, --example=<net>         Run the driver example.\n");
        lan8720_interface_debug_print("  -h, --help                        Show the help.\n");
        lan8720_interface_debug_print("  -i, --information                 Show the chip information.\n");
        lan8720_interface_debug_print("      --name=<domain>               Set domain name.([default: www.bing.com])\n");
        lan8720_interface_debug_print("      --operate=<init | dns>        Set operate, init is init the net and dns is running the dns.\n");
        lan8720_interface_debug_print("  -p, --port                        Display the pins used by this device to connect the chip.\n");
        lan8720_interface_debug_print("  -t <reg>, --test=<reg>            Run the driver test.\n");

        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        lan8720_info_t info;

        /* print lan8720 info */
        lan8720_info(&info);
        lan8720_interface_debug_print("lan8720: chip is %s.\n", info.chip_name);
        lan8720_interface_debug_print("lan8720: manufacturer is %s.\n", info.manufacturer_name);
        lan8720_interface_debug_print("lan8720: interface is %s.\n", info.interface);
        lan8720_interface_debug_print("lan8720: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        lan8720_interface_debug_print("lan8720: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        lan8720_interface_debug_print("lan8720: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        lan8720_interface_debug_print("lan8720: max current is %0.2fmA.\n", info.max_current_ma);
        lan8720_interface_debug_print("lan8720: max temperature is %0.1fC.\n", info.temperature_max);
        lan8720_interface_debug_print("lan8720: min temperature is %0.1fC.\n", info.temperature_min);

        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        lan8720_interface_debug_print("lan8720: MDC connected to GPIOC PIN1.\n");
        lan8720_interface_debug_print("lan8720: MDIO connected to GPIOA PIN2.\n");
        lan8720_interface_debug_print("lan8720: RESET connected to GPIOA PIN8.\n");
        lan8720_interface_debug_print("lan8720: REF_CLK connected to GPIOA PIN1.\n");
        lan8720_interface_debug_print("lan8720: CRS_DV connected to GPIOA PIN7.\n");
        lan8720_interface_debug_print("lan8720: RXD0 connected to GPIOC PIN4.\n");
        lan8720_interface_debug_print("lan8720: RXD1 connected to GPIOC PIN5.\n");
        lan8720_interface_debug_print("lan8720: TX_EN connected to GPIOG PI11.\n");
        lan8720_interface_debug_print("lan8720: TXD0 connected to GPIOG PIN13.\n");
        lan8720_interface_debug_print("lan8720: TXD1 connected to GPIOG PIN14.\n");

        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief main function
 * @note  none
 */
int main(void)
{
    uint8_t res;

    /* stm32f407 clock init and hal init */
    clock_init();

    /* delay init */
    delay_init();

    /* uart init */
    uart_init(115200);

    /* shell init && register lan8720 function */
    shell_init();
    shell_register("lan8720", lan8720);
    uart_print("lan8720: welcome to libdriver lan8720.\n");
    
    while (1)
    {
        /* read uart */
        g_len = uart_read(g_buf, 256);
        if (g_len != 0)
        {
            /* run shell */
            res = shell_parse((char *)g_buf, g_len);
            if (res == 0)
            {
                /* run success */
            }
            else if (res == 1)
            {
                uart_print("lan8720: run failed.\n");
            }
            else if (res == 2)
            {
                uart_print("lan8720: unknown command.\n");
            }
            else if (res == 3)
            {
                uart_print("lan8720: length is too long.\n");
            }
            else if (res == 4)
            {
                uart_print("lan8720: pretreat failed.\n");
            }
            else if (res == 5)
            {
                uart_print("lan8720: param is invalid.\n");
            }
            else
            {
                uart_print("lan8720: unknown status code.\n");
            }
            uart_flush();
        }
        lwip_server();
        delay_ms(100);
    }
}
