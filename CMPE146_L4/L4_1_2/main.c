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
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

char* string1 = "12345";
char string2[] = "12345";
const char string3[1024 * 150] = {"12345"};

uint32_t memAddr(char* addr);
bool flashUnpro(char* addr);

void main(void)
{
    /* Stop Watchdog  */
    //MAP_WDT_A_holdTimer();

    printf("\n");
    printf("string1: %s\n", string1);
    printf("addr: %x\n", &string1);
    printf("string2: %s\n", string2);
    printf("addr: %x\n", &string2);
    printf("string3: %s\n", string3);
    printf("addr: %x\n", &string3);
    string2[0] = '0';
    string2[1] = '4';



    if (flashUnpro(&string3) == false)
    {
        printf("unprotect operation failed!\n");
    }
    if (ROM_FlashCtl_programMemory(string2, string3, 2) == false)
    {
        printf("program str2 to str3 failed.\n");
    }
    if (ROM_FlashCtl_programMemory(string2, string1, 2) == false)
    {
        printf("program str2 to str1 failed.\n");
    }


//    FlashCtl_programMemory(string2, string1, 2);
//    FlashCtl_programMemory(string2, string3, 2);

    /*
    char *ptr = 0x21000;
    memOp(&string1);
    memOp(&string2);
    memOp(ptr);
    */

    //ROM_FlashCtl_programMemory(string2, string1, 2);
    //ROM_FlashCtl_programMemory(string2, string3, 2);

    printf("string1: %s\n", string1);
    printf("string2: %s\n", string2);
    printf("string3: %s\n", string3);
}

bool flashUnpro(char* addr)
{
    printf("\nstarting flashUnpro()\n");
    int address = (int)addr;
    int base = ((address >=0x20000) && (address < 0x40000)) ? (address - 0x20000) : address;
    uint32_t sector = (address == base) ? address >> 12 : (address - base) >> 12;
    if ((address < 0x40000) && (address >= 0x2000))
    {
        printf("Bank 1, sector %d : addr: %x\n", sector, address);
        return ROM_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, sector);
    }
    else if ((address < 0x20000) && (address >= 0x0))
    {
        printf("Bank 0, sector %d : addr: %x\n", sector, address);
        return ROM_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK0, sector);
    }
    printf("%x is not a Flash address.\n", address);
    return false;
}

uint32_t memAddr(char* addr)
{
    printf("passed adr: %x\n", addr);
    int address = (int)addr;
    printf("address as int: %x\n", address);
    int base = ((address >=0x20000) && (address < 0x40000)) ? (address - 0x20000) : address;
    printf("base address: %x\n", base);

    // sector formula found in MSP432 Flash operations manual
    // shift right 12x is same as dividing by 4096, but quicker operation
    uint32_t sector = (address == base) ? address >> 12 : (address - base) >> 12;
    printf("sector: %d\n", sector);

    return sector;
}
