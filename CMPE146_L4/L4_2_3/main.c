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

#define TIMER_MAX 0x000fffff

void delay_ms(uint32_t count);

int main(void)
{
    /* Stop Watchdog  */
    MAP_WDT_A_holdTimer();

    printf("\nMCLK: %u\n", MAP_CS_getMCLK());
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);

    /* 10000 */
    int t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(10000);
    int t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    double total = (t0 - t1)/3000;
    printf("10000: %f ms\n", total);

    /* 5000 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(5000);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("5000: %f ms\n", total);

    /* 2000 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(2000);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("2000: %f ms\n", total);

    /* 1000 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(1000);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("1000: %f ms\n", total);

    /* 10 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(10);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("10: %f ms\n", total);

    /* 5 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(5);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("5: %f ms\n", total);

    /* 2 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(2);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("2: %f ms\n", total);

    /* 1 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(1);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("1: %f ms\n", total);

    /* 0 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(0);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("0: %f ms\n", total);

    /* 10000 */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    delay_ms(10000);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    total = (t0 - t1)/3000;
    printf("10000: %f ms\n", total);
}

void delay_ms(uint32_t count)
{
    uint32_t done = (MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX) - ((count * 3000)); // the final value of timer
    uint32_t factor = 0;
    uint32_t current = MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX;
    uint32_t last = current;
    while (current > done)
    {
        current = (MAP_Timer32_getValue(TIMER32_0_BASE) & TIMER_MAX) - (factor * TIMER_MAX);
        if (last < current) factor++;
    }
}
