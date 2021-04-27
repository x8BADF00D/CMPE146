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
/******************************************************************************
 * MSP432 Empty Project
 *
 * Description: An empty project that uses DriverLib
 *
 *                MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 * Author: 
*******************************************************************************/
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define BAUD_RATE 1200

void delay_us(uint32_t count);
void delay_ms(uint32_t count);
void txdel();
void send_message(char* msg);

int main(void)
 {
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    // Set P1.3 output, set idle state (high)
    GPIO_setAsOutputPin(GPIO_PORT_P1,GPIO_PIN3);
    GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN3);

    // Set up Timer32
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);

    delay_ms(100);

    // sending message
    printf("sending...\n");
    send_message("The quick brown fox jumps over the lazy dog.\r\n");
    printf("sent!\n");

    while(1)
    {

    }
}

// Delay for some milliseconds
// Function provided by Professor Wo-Tak Wu, edited by myself
void delay_us(uint32_t count)
{
    uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
//    uint32_t clock_rate = MAP_CS_getMCLK();         // Number of ticks per second, by definition
    uint32_t t_final = t0 - (3 * count);
    while (1)
    {
        if (MAP_Timer32_getValue(TIMER32_0_BASE) < t_final)         // Timer counts down
        {
            break;
        }
    }
}

// Delay for some milliseconds
// Function provided by Professor Wo-Tak Wu, edited by myself
void delay_ms(uint32_t count)
{
    uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
    uint32_t clock_rate = MAP_CS_getMCLK();         // Number of ticks per second, by definition
    uint32_t t_final = t0 - (uint32_t)((uint64_t)clock_rate * count / 1000);
    while (1)
    {
        if (MAP_Timer32_getValue(TIMER32_0_BASE) < t_final)         // Timer counts down
        {
            break;
        }
    }
}

void txdel()
{
    uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
    uint32_t t_final = t0 - 2500;
    while (1)
    {
        if (MAP_Timer32_getValue(TIMER32_0_BASE) < t_final)         // Timer counts down
        {
            break;
        }
    }
}

void send_message(char* msg)
{
    uint32_t del = 1000000/BAUD_RATE;
    int i = 0;
    int message_size = strlen(msg);

    /* Sending message  */
    while (i < message_size)
    {
        /* Start bit */
        GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN3);
        delay_us(del);

        int j = 0;

        while(j < 8)
        {
            if(msg[i] & (1<<j))
            {
                GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN3);
            }
            else
            {
                GPIO_setOutputLowOnPin(GPIO_PORT_P1,GPIO_PIN3);
            }
            delay_us(del);
            j++;
        }
        i++;

        GPIO_setOutputHighOnPin(GPIO_PORT_P1,GPIO_PIN3);
        delay_us(del * 2);
    }
}
