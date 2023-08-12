### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

SMI Pin: MDC/MDIO PC1/PA2.

GPIO Pin: RESET PA8.

RMII Pin: REF_CLK/CRS_DV/RXD0/RXD1/TX_EN/TXD0/TXD1 PA1/PA7/PC4/PC5/PG11/PG13/PG14.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. LAN8720

#### 3.1 Command Instruction

1. Show lan8720 chip and driver information.

    ```shell
    lan8720 (-i | --information)  
    ```

2. Show lan8720 help.

    ```shell
    lan8720 (-h | --help)        
    ```

3. Show lan8720 pin connections of the current board.

    ```shell
    lan8720 (-p | --port)        
    ```

4. Run lan8720 register test, num is the chip address number.

    ```shell
    lan8720 (-t reg | --test=reg) [--addr=<num>]       
    ```

5. Run lan8720 net function, num is the chip address number, domain is the domain name.

    ```shell
    lan8720 (-e net | --example=net) (--operate=<init | dns>) [--addr=<num>] [--name=<domain>] 
    ```

#### 3.2 Command Example

```shell
lan8720 -i

lan8720: chip is Microchip LAN8720.
lan8720: manufacturer is Microchip.
lan8720: interface is SMI RMII.
lan8720: driver version is 1.0.
lan8720: min supply voltage is 3.0V.
lan8720: max supply voltage is 3.6V.
lan8720: max current is 60.00mA.
lan8720: max temperature is 85.0C.
lan8720: min temperature is -40.0C.
```

```shell
lan8720 -p

lan8720: MDC connected to GPIOC PIN1.
lan8720: MDIO connected to GPIOA PIN2.
lan8720: RESET connected to GPIOA PIN8.
lan8720: REF_CLK connected to GPIOA PIN1.
lan8720: CRS_DV connected to GPIOA PIN7.
lan8720: RXD0 connected to GPIOC PIN4.
lan8720: RXD1 connected to GPIOC PIN5.
lan8720: TX_EN connected to GPIOG PI11.
lan8720: TXD0 connected to GPIOG PIN13.
lan8720: TXD1 connected to GPIOG PIN14.
```

```shell
lan8720 -t reg --addr=1

lan8720: chip is Microchip LAN8720.
lan8720: manufacturer is Microchip.
lan8720: interface is SMI RMII.
lan8720: driver version is 1.0.
lan8720: min supply voltage is 3.0V.
lan8720: max supply voltage is 3.6V.
lan8720: max current is 60.00mA.
lan8720: max temperature is 85.0C.
lan8720: min temperature is -40.0C.
lan8720: start register test.
lan8720: lan8720_set_address/lan8720_get_address test.
lan8720: set address 0x10.
lan8720: check address ok.
lan8720: lan8720_set_loopback/lan8720_get_loopback test.
lan8720: enable loopback.
lan8720: check loopback ok.
lan8720: disable loopback.
lan8720: check loopback ok.
lan8720: lan8720_set_speed_select/lan8720_get_speed_select test.
lan8720: set speed select 10mbps.
lan8720: check speed select ok.
lan8720: set speed select 100mbps.
lan8720: check speed select ok.
lan8720: lan8720_set_auto_negotiation/lan8720_get_auto_negotiation test.
lan8720: enable auto negotiation failed.
lan8720: check auto negotiation ok.
lan8720: disable auto negotiation failed.
lan8720: check auto negotiation ok.
lan8720: lan8720_set_power_down/lan8720_get_power_down test.
lan8720: enable power down.
lan8720: check power down ok.
lan8720: disable power down.
lan8720: check power down ok.
lan8720: lan8720_set_electrical_isolation/lan8720_get_electrical_isolation test.
lan8720: enable electrical isolation.
lan8720: check electrical isolation ok.
lan8720: disable electrical isolation.
lan8720: check electrical isolation ok.
lan8720: lan8720_set_restart_auto_negotiate/lan8720_get_restart_auto_negotiate test.
lan8720: enable restart auto negotiate.
lan8720: check restart auto negotiate ok.
lan8720: disable restart auto negotiate.
lan8720: check restart auto negotiate ok.
lan8720: lan8720_set_duplex_mode/lan8720_get_duplex_mode test.
lan8720: set half duplex mode.
lan8720: check duplex mode ok.
lan8720: set full duplex mode.
lan8720: check duplex mode ok.
lan8720: lan8720_get_100base_t4 test.
lan8720: 100base t4 is no.
lan8720: lan8720_get_100base_tx_full_duplex test.
lan8720: 100base tx full duplex is yes.
lan8720: lan8720_get_100base_tx_half_duplex test.
lan8720: 100base tx half duplex is yes.
lan8720: lan8720_get_10base_t_full_duplex test.
lan8720: 10base t full duplex is yes.
lan8720: lan8720_get_10base_t_half_duplex test.
lan8720: 10base t half duplex is yes.
lan8720: lan8720_get_100base_t2_full_duplex test.
lan8720: 100base t2 full duplex is no.
lan8720: lan8720_get_100base_t2_half_duplex test.
lan8720: 100base t2 half duplex is no.
lan8720: lan8720_get_extended_status_information test.
lan8720: extended status information is no.
lan8720: lan8720_get_auto_negotiate_complete test.
lan8720: auto negotiate complete is no.
lan8720: lan8720_get_remote_fault test.
lan8720: remote fault is no.
lan8720: lan8720_get_auto_negotiate_ability test.
lan8720: auto negotiate ability is yes.
lan8720: lan8720_get_link_status test.
lan8720: link status is down.
lan8720: lan8720_get_jabber_detect test.
lan8720: jabber detect is no.
lan8720: lan8720_get_extended_capabilities test.
lan8720: extended capabilities is yes.
lan8720: lan8720_set_identifier/lan8720_get_identifier test.
lan8720: set phy_id 0x002391.
lan8720: set model_number 0x04.
lan8720: set revison_number 0x05.
lan8720: check phy_id ok.
lan8720: check model_number ok.
lan8720: check revison_number ok.
lan8720: lan8720_set_auto_negotiation_advertisement_remote_fault/lan8720_get_auto_negotiation_advertisement_remote_fault test.
lan8720: enable auto negotiation advertisement remote fault.
lan8720: check auto negotiation advertisement remote fault ok.
lan8720: disable auto negotiation advertisement remote fault.
lan8720: check auto negotiation advertisement remote fault ok.
lan8720: lan8720_set_auto_negotiation_advertisement_pause/lan8720_get_auto_negotiation_advertisement_pause test.
lan8720: set no pause.
lan8720: check auto negotiation advertisement pause ok.
lan8720: set symmetric pause.
lan8720: check auto negotiation advertisement pause ok.
lan8720: set asymmetric pause.
lan8720: check auto negotiation advertisement pause ok.
lan8720: set both pause.
lan8720: check auto negotiation advertisement pause ok.
lan8720: lan8720_set_auto_negotiation_advertisement_100base_tx_full_duplex/lan8720_get_auto_negotiation_advertisement_100base_tx_full_duplex test.
lan8720: enable auto negotiation advertisement 100base tx full duplex.
lan8720: check auto negotiation advertisement 100base tx full duplex ok.
lan8720: disable auto negotiation advertisement 100base tx full duplex.
lan8720: check auto negotiation advertisement 100base tx full duplex ok.
lan8720: lan8720_set_auto_negotiation_advertisement_100base_tx/lan8720_get_auto_negotiation_advertisement_100base_tx test.
lan8720: enable auto negotiation advertisement 100base tx.
lan8720: check auto negotiation advertisement 100base tx ok.
lan8720: disable auto negotiation advertisement 100base tx.
lan8720: check auto negotiation advertisement 100base tx ok.
lan8720: lan8720_set_auto_negotiation_advertisement_10base_t_full_duplex/lan8720_get_auto_negotiation_advertisement_10base_t_full_duplex test.
lan8720: enable auto negotiation advertisement 10base t full duplex failed.
lan8720: check auto negotiation advertisement 10base t full duplex ok.
lan8720: disable auto negotiation advertisement 10base t full duplex failed.
lan8720: check auto negotiation advertisement 10base t full duplex ok.
lan8720: lan8720_set_auto_negotiation_advertisement_10base_t/lan8720_get_auto_negotiation_advertisement_10base_t test.
lan8720: enable auto negotiation advertisement 10base t.
lan8720: check auto negotiation advertisement 10base t ok.
lan8720: disable auto negotiation advertisement 10base t.
lan8720: check auto negotiation advertisement 10base t ok.
lan8720: lan8720_set_auto_negotiation_advertisement_selector_field/lan8720_get_auto_negotiation_advertisement_selector_field test.
lan8720: set auto negotiation advertisement selector field 0x04.
lan8720: check auto negotiation advertisement selector field ok.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_next_page test.
lan8720: check auto negotiation link partner ability next page false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_acknowledge test.
lan8720: check auto negotiation link partner ability acknowledge false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_remote_fault test.
lan8720: check auto negotiation link partner ability remote fault false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_pause test.
lan8720: check auto negotiation link partner ability pause false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_100base_t4 test.
lan8720: check auto negotiation link partner ability 100base t4 false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_100base_tx_full_duplex test.
lan8720: check auto negotiation link partner ability 100base tx full duplex false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_100base_tx test.
lan8720: check auto negotiation link partner ability 100base tx false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_10base_t_full_duplex test.
lan8720: check auto negotiation link partner ability 10base t full duplex false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_10base_t test.
lan8720: check auto negotiation link partner ability 10base t false.
lan8720: lan8720_get_auto_negotiation_link_partner_ability_selector_field test.
lan8720: auto negotiation link partner ability selector field is 0x01.
lan8720: lan8720_get_auto_negotiation_expansion_parallel_detection_fault test.
lan8720: check auto negotiation expansion parallel detection fault false.
lan8720: lan8720_get_auto_negotiation_expansion_link_partner_next_page_able test.
lan8720: check auto negotiation expansion link partner next page able false.
lan8720: lan8720_get_auto_negotiation_expansion_next_page_able test.
lan8720: check auto negotiation expansion next page able false.
lan8720: lan8720_get_auto_negotiation_expansion_page_received test.
lan8720: check auto negotiation expansion page received false.
lan8720: lan8720_get_auto_negotiation_expansion_link_partner_auto_negotiation_able test.
lan8720: check auto negotiation expansion link partner auto negotiation able false.
lan8720: lan8720_set_energy_detect_power_down_mode/lan8720_get_energy_detect_power_down_mode test.
lan8720: enable energy detect power down mode.
lan8720: check energy detect power down mode ok.
lan8720: disable energy detect power down mode.
lan8720: check energy detect power down mode ok.
lan8720: lan8720_set_far_loopback/lan8720_get_far_loopback test.
lan8720: enable far loopback.
lan8720: check far loopback ok.
lan8720: disable far loopback.
lan8720: check far loopback ok.
lan8720: lan8720_set_alternate_interrupt_mode/lan8720_get_alternate_interrupt_mode test.
lan8720: enable alternate interrupt mode.
lan8720: check alternate interrupt mode ok.
lan8720: disable alternate interrupt mode.
lan8720: check alternate interrupt mode ok.
lan8720: lan8720_get_energy_detected test.
lan8720: check energy detected ok.
lan8720: lan8720_set_mode/lan8720_get_mode test.
lan8720: set 10base-t half duplex.
lan8720: check mode ok.
lan8720: set 10base-t full duplex.
lan8720: check mode ok.
lan8720: set 100base-tx half duplex.
lan8720: check mode ok.
lan8720: set 100base-tx full duplex.
lan8720: check mode ok.
lan8720: set 100base-tx half duplex advertised.
lan8720: check mode ok.
lan8720: set repeater mode.
lan8720: check mode ok.
lan8720: set power down mode.
lan8720: check mode ok.
lan8720: set all capable.
lan8720: check mode ok.
lan8720: lan8720_set_phy_address/lan8720_get_phy_address test.
lan8720: set phy address 0x01.
lan8720: check phy address ok.
lan8720: lan8720_get_symbol_error_counter test.
lan8720: symbol error counter is 0.
lan8720: lan8720_set_auto_mdix/lan8720_get_auto_mdix test.
lan8720: enable auto mdix.
lan8720: check auto mdix ok.
lan8720: disable auto mdix.
lan8720: check auto mdix ok.
lan8720: lan8720_set_manual_channel_select/lan8720_get_manual_channel_select test.
lan8720: set tx transmits, rx receives.
lan8720: check manual channel select ok.
lan8720: set tx receives, rx transmits.
lan8720: check manual channel select ok.
lan8720: lan8720_set_sqe_test_off/lan8720_get_sqe_test_off test.
lan8720: enable sqe test off.
lan8720: check sqe test off ok.
lan8720: disable sqe test off.
lan8720: check sqe test off ok.
lan8720: lan8720_get_polarity test.
lan8720: check polarity normal polarity.
lan8720: lan8720_get_interrupt_flag test.
lan8720: check energy on generated interrupt flag false.
lan8720: check auto negotiation complete interrupt flag false.
lan8720: check remote fault detected interrupt flag false.
lan8720: check link down interrupt flag true.
lan8720: check auto negotiation lp acknowledge interrupt flag false.
lan8720: check parallel detection fault interrupt flag false.
lan8720: check auto negotiation page received interrupt flag false.
lan8720: lan8720_set_interrupt_mask/lan8720_get_interrupt_mask test.
lan8720: enable energy on generated.
lan8720: check interrupt mask ok.
lan8720: disable energy on generated.
lan8720: check interrupt mask ok.
lan8720: enable auto negotiation complete.
lan8720: check interrupt mask ok.
lan8720: disable auto negotiation complete.
lan8720: check interrupt mask ok.
lan8720: enable remote fault detected.
lan8720: check interrupt mask ok.
lan8720: disable remote fault detected.
lan8720: check interrupt mask ok.
lan8720: enable link down.
lan8720: check interrupt mask ok.
lan8720: disable link down.
lan8720: check interrupt mask ok.
lan8720: enable auto negotiation lp acknowledge.
lan8720: check interrupt mask ok.
lan8720: disable auto negotiation lp acknowledge.
lan8720: check interrupt mask ok.
lan8720: enable parallel detection fault.
lan8720: check interrupt mask ok.
lan8720: disable parallel detection fault.
lan8720: check interrupt mask ok.
lan8720: enable auto negotiation page received.
lan8720: check interrupt mask ok.
lan8720: disable auto negotiation page received.
lan8720: check interrupt mask ok.
lan8720: lan8720_get_auto_negotiation_done test.
lan8720: check auto negotiation done false.
lan8720: lan8720_get_speed_indication test.
lan8720: check speed indication 0x06.
lan8720: lan8720_set_soft_reset/lan8720_get_soft_reset test.
lan8720: enable soft reset.
lan8720: check soft reset ok.
lan8720: finish register test.
```

```shell
lan8720 -e net --operate=init --addr=1

start dhcp.
state: looking for dhcp server ...
ip address assigned by a dhcp server: 192.168.1.103
```

```shell
lan8720 -e net --operate=dns --addr=1 --name=www.libdriver.com 

www.libdriver.com dns: 39.101.212.84
```

```shell
lan8720 -h

Usage:
  lan8720 (-i | --information)
  lan8720 (-h | --help)
  lan8720 (-p | --port)
  lan8720 (-t reg | --test=reg) [--addr=<num>]
  lan8720 (-e net | --example=net) (--operate=<init | dns>) [--addr=<num>] [--name=<domain>]

Options:
      --addr=<num>                  Set the chip address number.([default: 1])
  -e <net>, --example=<net>         Run the driver example.
  -h, --help                        Show the help.
  -i, --information                 Show the chip information.
      --name=<domain>               Set domain name.([default: www.bing.com])
      --operate=<init | dns>        Set operate, init is init the net and dns is running the dns.
  -p, --port                        Display the pins used by this device to connect the chip.
  -t <reg>, --test=<reg>            Run the driver test.
```
