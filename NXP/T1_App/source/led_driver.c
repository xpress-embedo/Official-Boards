/*
 * led_driver.c
 *
 *  Created on: 07-Jun-2025
 *      Author: xpress_embedo
 */

#include "led_driver.h"
#include "fsl_clock.h"
#include "fsl_flexio.h"
#include "fsl_flexio_spi.h"

/*--------------------------------- Macros -----------------------------------*/
// Transfer size is two bytes as we need to transfer data from COL06 to COL21,
// i.e. 16-bits, which can be done in 2 bytes
#define LED_DRIVER_TRANSFER_SIZE                    (2u)
#define LED_DRIVER_BAUD_RATE                        (500000u)
#define LED_DRIVER_ADDRESS                          (FLEXIO)
#define LED_DRIVER_CLOCK_NAME                       (kCLOCK_Flexio0)
#define LED_DRIVER_CLOCK_SOURCE                     (kCLOCK_IpSrcFircAsync)

#define LED_DRIVER_SPI_SDO_PIN                      (4u)    // this is FlexIO pin
#define LED_DRIVER_SPI_CLK_PIN                      (0u)    // this is FlexIO pin
// Not sure what to do with SDI pin, as in HW it is not configured
#define LED_DRIVER_SPI_SDI_PIN                      (3u)    // (0u)
// The chip select seems not controlled from FlexIO module and is controlled using normal GPIO logic
// Actually this is not Chip Select, this is basically Latch Enable pin of Shift Register
#define LED_DRIVER_SPI_CS_PIN                       (3u)    // (0u)
/* note: PTD5, PTD6, PTD7 are free pins and not connected to anything, PTD5 is
 * also a FlexIO pin i.e FLEXIO-D3 pin, hence I am using this pin for SDI and CS
 * this is a workaround, if I keep using 0 for SDI and Chip Select it is corrupting
 * the clock and the data on logic analyzer doesn't seem nice.
 * I have also made a forum post on NXP community, will wait for the correct
 * solution to solve this problem.
 */
 
 
/*--------------------------- Private Variables ------------------------------*/
static FLEXIO_SPI_Type spi_device;
static flexio_spi_master_config_t master_config;

/*------------------------- Private Function Prototypes ----------------------*/

/*-------------------------- Public Function Definitions ---------------------*/
void LedDriver_Init( void )
{
  uint32_t clock;

  /* Set PCC FLEXIO selection */
  CLOCK_SetIpSrc( LED_DRIVER_CLOCK_NAME, LED_DRIVER_CLOCK_SOURCE);

  /* Get default master configuration */
  FLEXIO_SPI_MasterGetDefaultConfig( &master_config );
  master_config.baudRate_Bps = LED_DRIVER_BAUD_RATE;
  /* Default bit count at 16. */
  master_config.dataMode = kFLEXIO_SPI_16BitMode;

  /* FLEXIO SPI Device Configuration */
  spi_device.flexioBase = LED_DRIVER_ADDRESS;
  spi_device.SCKPinIndex = LED_DRIVER_SPI_CLK_PIN;
  spi_device.SDOPinIndex = LED_DRIVER_SPI_SDO_PIN;
  // not sure about chip select and SDI pins
  /*
  * NOTE: there is strange behavior, on my custom board the CS and SDI is not
  * available, so by default they are set to "0" as not configured.
  * But the problem is clock is also "0", and this was causing some problem
  * and the clock was stretching and causing incorrect detection of data on
  * logic analyzer, for now this is fixed by specifying some dummy pins
  */
  spi_device.CSnPinIndex = LED_DRIVER_SPI_CS_PIN;
  spi_device.SDIPinIndex = LED_DRIVER_SPI_SDI_PIN;
  spi_device.shifterIndex[0]  = 0u;
  spi_device.shifterIndex[1]  = 1u;
  spi_device.timerIndex[0]    = 0u;
  spi_device.timerIndex[1]    = 1u;

  clock = CLOCK_GetIpFreq(LED_DRIVER_CLOCK_NAME);
  /* Initialize FlexIO SPI Master */
  FLEXIO_SPI_MasterInit(&spi_device, &master_config, clock);
}

// This function will send two bytes of data COL06 to COL21
void LedDriver_Write( uint16_t data_to_transmit )
{
  flexio_spi_transfer_t master_transfer;
  uint8_t tx_data[2] = { (data_to_transmit & 0xFF), ((data_to_transmit >>8) & 0xFF) };

  master_transfer.txData    = tx_data;
  master_transfer.rxData    = NULL;
  master_transfer.dataSize  = LED_DRIVER_TRANSFER_SIZE;
  master_transfer.flags     = kFLEXIO_SPI_16bitMsb;

  FLEXIO_SPI_MasterTransferBlocking( &spi_device, &master_transfer );
}

/*
Notes STP16CPC26 Low Voltage 16-bit Constant Current LED Sink Driver
  * 16 Constant Current output channels -> HD_COL6 to HD_COL21
  * Adjustable output current through external resistor
  * Maximum Clock Frequency 30 MHz
  * The device contains a 16-bit shift register and data latches, which convert
    serial input data into parallel output format. In the output stage sixteen
    regulated current generators provide 5 mA to 90 mA constant current to drive
    LEDs.
  * LED brightness can be adjusted from 0% to 100% via Output Enable pin.

  * Pin Description:
  * 01    :=  GND     :=  Ground Terminal
  * 02    :=  SDI     :=  Serial Data Input Terminal => HMI_HD_MOSI (PTD2)
  * 03    :=  CLK     :=  Clock Input Terminal => HMI_HD_CLK (PTD0)
  * 04    :=  LE      :=  Latch Input Terminal => HMI_HD_CS (PTA12)
  * 5-20  :=  OUTO-15 :=  Output Terminal
  * 21    :=  OE      :=  Input Terminal for Output Enable (active low) => HMI_HD_BLANK (PTA3)
  * 22    :=  SDO     :=  Serial Data Out Terminal
  * 23    :=  REXT    :=  Input Terminal for External Resistance for Constant Current Programming
  * 24    :=  VDD     :=  Supply Voltage Terminal

  * Apart from this we have one HMI_HD_EN (PTC14) pin which is used to generate
  * supply for the Shift Register IC, if this is not enabled, then no signal will
  * work properly.

  * Important information related to timing:
  * - LE and OE are level sensitive and not synchronized with CLK signal.
  * - When LE is at low level, the latch circuit holds previous data.
  * - If LE is high level, data present in the shift register are latched.
  * - When OE is at low level, the status of the OUTO to OUT15 depends on the data
      in the latch circuits.
  * - When OE is at high level, all outputs are switched off independently on the
      data stored in the latch circuits.
  * - Every rising edge of the CLK signal, a new data on the SDI pin is sampled.
  * - The data is loaded into the shift register, whereas a bit is shifted out 
      from SDO

  * T1 Board Clock Information
  * FAST IRC        ->    48 MHz
  * System:
  * Bus Clock       ->    24 MHz
  * Core Clock      ->    48 MHz
  * Flash Clock     ->    24 MHz
  * System Clock    ->    48 MHz
  * Peripheral:
  * FLEXIO Clock    ->    12 MHz because FIRC DIV2 is 12 MHz because divide by 4
*/


