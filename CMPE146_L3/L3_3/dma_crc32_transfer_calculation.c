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
 * MSP432 DMA - CRC32 calculation using DMA
 *
 * Description: This code examples shows how to use the DMA module on MSP432 
 * to feed data into the CRC32 module for a CRC32 signature calculation. This
 * use case is particularly useful when the user wants to calculate the CRC32
 * signature of a large data array (such as a firmware image) but still wants
 * to maximize power consumption. After the DMA transfer is setup, a software
 * initiation occurs and the MSP432 device is put to sleep. Once the transfer
 * completes, the DMA interrupt occurs and the CRC32 result is placed into
 * a local variable for the user to examine.
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
/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>

#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#define CRC32_SEED 0xFFFFFFFF

/* Statics */
static volatile uint32_t crcSignature;

volatile int dma_done;

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(controlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif
uint8_t controlTable[1024];

/* Extern */
//extern uint8_t data_array[];

uint8_t data_array[1024];
int size_array[] = {2, 4, 16, 32, 64, 128, 256, 786, 1024};

int main(void)
{
    printf("\n");
    /* Setting up timer */
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);

    /* Halting Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring DMA module */
    MAP_DMA_enableModule();
    MAP_DMA_setControlBase(controlTable);

    /* Setting Control Indexes. In this case we will set the source of the
     * DMA transfer to our random data array and the destination to the
     * CRC32 data in register address*/
    int i;
    for (i = 0; i < 9; i++)
    {
        printf("\n\nCurrent Block size: %d\n", size_array[i]);

        /* CRC without DMA */
        MAP_CRC32_setSeed(CRC32_SEED, CRC32_MODE);
        int ii;
        uint32_t t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
        for (ii = 0; ii < size_array[i]; ii++)
        {
            MAP_CRC32_set8BitData(data_array[ii], CRC32_MODE);
        }
        uint32_t hwCRC = MAP_CRC32_getResult(CRC32_MODE);
        uint32_t t1 = MAP_Timer32_getValue(TIMER32_0_BASE);
        printf("hwCRC =%08x\n", hwCRC);
        uint32_t totalhw = t0 - t1;
        uint32_t freq = MAP_CS_getMCLK() / 1000000;
        uint32_t ushw = totalhw / freq;
        printf("time (us): %d\n", ushw);
        printf("counter: %d freq: %dMHz\n", totalhw, freq);

        MAP_DMA_setChannelControl(UDMA_PRI_SELECT,
                    UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1024);

        MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT,
                    UDMA_MODE_AUTO, data_array,
                    (void*) (&CRC32->DI32), size_array[i]);

        /* Assigning/Enabling Interrupts */
        MAP_DMA_assignInterrupt(DMA_INT1, 0);
        MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
        MAP_Interrupt_enableMaster();

        /* Enabling DMA Channel 0 */
        MAP_DMA_enableChannel(0);

        /* Reinitialize CRC32 seed */
        MAP_CRC32_setSeed(CRC32_SEED, CRC32_MODE);

        /* Set flag to 0 */
        dma_done = 0;

        t0 = MAP_Timer32_getValue(TIMER32_0_BASE);
        /* Forcing a software transfer on DMA Channel 0 */
        MAP_DMA_requestSoftwareTransfer(0);

        while(!dma_done)
        {
        }
        t1 = MAP_Timer32_getValue(TIMER32_0_BASE);

        printf("dmaCRC=%08x\n", crcSignature);
        uint32_t totaldma = t0 - t1;
        freq = MAP_CS_getMCLK() / 1000000;
        uint32_t usdma = totaldma / freq;
        printf("time (us): %d\n", usdma);
        printf("counter: %d freq: %dMHz\n", totaldma, freq);
        float speedup = (float)ushw / (float)usdma;
        printf("speedup: %f bytes\n", speedup);
    }
}

/* Completion interrupt for DMA */
void DMA_INT1_IRQHandler(void)
{
    MAP_DMA_disableChannel(0);
    crcSignature = MAP_CRC32_getResult(CRC32_MODE);

    dma_done = 1;
}
