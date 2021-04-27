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
 * MSP432 CRC32 - CRC32 Calculation
 *
 * Description: In this example, the CRC32 module is used to calculate a CRC32
 * checksum in CRC32 mode. This value is compared versus a software calculated
 * checksum. The idea here is to have the user use the debugger to step through
 * the code and observe the CRC calculation in the debugger. Note that this
 * code example was made to use the standard CRC32 polynomial value of
 * 0xCBF43926.
 *
 *              MSP432P401
 *             ------------------
 *         /|\|                  |
 *          | |                  |
 *          --|RST               |
 *            |                  |
 *            |                  |
 *            |                  |
 *            |                  |
 *
 ******************************************************************************/
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>
#include <stdlib.h>

#define CRC32_POLY              0xEDB88320
#define CRC32_INIT              0xFFFFFFFF

uint8_t myData[10240] = { 0 };
static uint32_t calculateCRC32(uint8_t* data, uint32_t length);
static uint32_t compute_simple_checksum(uint8_t* data, uint32_t length);

volatile uint32_t hwCalculatedCRC, swCalculatedCRC, myCalculatedCRC;

//![Simple CRC32 Example] 
int main(void)
{
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);

    uint32_t ii;
    uint16_t i = 0;
    while (i < 10240)
    {
        myData[i] = (rand() * 2) + i;
        i++;
    }

    /* Stop WDT */
    MAP_WDT_A_holdTimer();

    MAP_CRC32_setSeed(CRC32_INIT, CRC32_MODE);

    for (ii = 0; ii < 10240; ii++)
        MAP_CRC32_set8BitData(myData[ii], CRC32_MODE);

//    myData[20] += 1; // adding 1 to data #20 to make it odd
//    myData[21] -= 1; // subtracting 1 from data #21 to make it even

    /* Getting the result from the hardware module */
    uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
    hwCalculatedCRC = MAP_CRC32_getResultReversed(CRC32_MODE) ^ 0xFFFFFFFF;
    uint32_t t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
    uint32_t totalhw = (t0 - t1) * 333;

    /* Calculating the CRC32 checksum through software */
//    t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
//    swCalculatedCRC = calculateCRC32((uint8_t*) myData, 10240);
//    t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
//    uint32_t totalsw = (t0 - t1) * 333;

    /* Calculating the CRC32 checksum with my function */
    t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
    myCalculatedCRC = compute_simple_checksum((uint8_t*) myData, 10240);
    t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
    uint32_t totalmy = (t0 - t1) * 333;

//    float speed = (float)totalsw / (float)totalhw;

    printf("hw: %X time: %d ns\n", hwCalculatedCRC, totalhw);
//    printf("sw: %X time: %d ns\n", swCalculatedCRC, totalsw);
    printf("my: %X time: %d ns\n", myCalculatedCRC, totalmy);
//    printf("speedup sw to hw: %.2f\n", speed);

    /* Pause for the debugger */
    __no_operation();
}
//![Simple CRC32 Example] 

/* Simple Checksum function */
static uint32_t compute_simple_checksum(uint8_t* data, uint32_t length)
{
    uint32_t index = 0;
    uint32_t total = 0;
    while (index < length)
    {
        total += data[index];
        index++;
    }

    return ~total;
}

/* Standard software calculation of CRC32 */
static uint32_t calculateCRC32(uint8_t* data, uint32_t length)
{
    uint32_t ii, jj, byte, crc, mask;;

    crc = 0xFFFFFFFF;

    for(ii=0;ii<length;ii++)
    {
        byte = data[ii];
        crc = crc ^ byte;

        for (jj = 0; jj < 8; jj++)
        {
            mask = -(crc & 1);
            crc = (crc >> 1) ^ (CRC32_POLY & mask);
        }

    }

    return ~crc;
}
