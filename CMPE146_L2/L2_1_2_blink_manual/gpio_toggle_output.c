/* --COPYRIGHT--,BSD
 * Copyright (c) 2017, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
/*******************************************************************************
 * MSP432 GPIO - Toggle Output High/Low
 *
 * Description: In this very simple example, the LED on P1.0 is configured as
 * an output using DriverLib's GPIO APIs. An infinite loop is then started
 * which will continuously toggle the GPIO and effectively blink the LED.
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST         P1.0  |---> P1.0 LED
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

//![Simple GPIO Config]
int main(void)
{
    volatile uint32_t ii;

    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring P2.0 - P2.2 as output */
    /* Port 2 output address 0x4000_4c00 + 0x003h */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0);   // Red
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1);   // Green
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2);   // Blue

    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);    // Set Red bit to off
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);    // Set Green bit to off
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2);    // Set Blue bit to off


    /* addr is 0x40004C3 */
    int *addr = (int*)0x40004c03;

    /* Print statement to view address and value of Port 2 Output */
    //printf("addr: 0x%08X data: 0x%08X\n", addr, *addr);

    while (1)
    {
        /* Delay Loop */
        for(ii=0;ii<50000;ii++)
        {
        }

        /* XOR to toggle bit #3 for blue element of LED2 */
        *addr = *addr ^ 4;

        /* Print statement below to view value toggling */
        //printf("data: %08X\n", *addr);

    }
}
//![Simple GPIO Config]


