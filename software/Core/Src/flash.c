/*
 * Copyright 2020 c1ewd
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "flash.h"

volatile struct sSettings settings, settingsBackup;

/**
 * Erase page in internal flash
 * @param pageAddress address of memory page to erase
 */
void flashErase(unsigned int pageAddress) {
  while (FLASH->SR & FLASH_SR_BSY);
  
  if((FLASH->CR & FLASH_CR_LOCK)!=0) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }
  
  if (FLASH->SR & FLASH_SR_EOP) {
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR |= FLASH_CR_PER;
  FLASH->AR = pageAddress;
  FLASH->CR |= FLASH_CR_STRT;
  while (!(FLASH->SR & FLASH_SR_EOP));
  FLASH->SR = FLASH_SR_EOP;
  FLASH->CR &= ~FLASH_CR_PER;
}

/**
 * Write struct to internal flash
 * @param data pointer to settings struct
 * @param address address of memory page to write
 * @param count size of settings struct
 */
void flashWrite(unsigned char* data, unsigned int address, unsigned int count) {
  unsigned int i;

  while (FLASH->SR & FLASH_SR_BSY);

  if((FLASH->CR & FLASH_CR_LOCK)!=0) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }
  
  if (FLASH->SR & FLASH_SR_EOP) {
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR |= FLASH_CR_PG;

  for (i = 0; i < count; i += 2) {
    *(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
    while (!(FLASH->SR & FLASH_SR_EOP));
    FLASH->SR = FLASH_SR_EOP;
  }

  FLASH->CR &= ~(FLASH_CR_PG);
}

/**
 * Read struct from internal flash
 * @param data pointer to settings struct
 * @param address address of memory page to read from
 * @param count size of settings struct
 */
void flashRead(unsigned char* data, unsigned int address, unsigned int count) {
  unsigned int i;
  
  while (FLASH->SR & FLASH_SR_BSY);
/*
  if((FLASH->CR & FLASH_CR_LOCK)!=0) {
    FLASH->KEYR = FLASH_KEY1;
    FLASH->KEYR = FLASH_KEY2;
  }
  
  FLASH->CR |= FLASH_CR_PG;
*/
  FLASH->OBR &= ~(FLASH_OBR_RDPRT1 | FLASH_OBR_RDPRT2);
  //попробывать
  // data[i] = *(volatile unsigned char*)(address + i);
  // data[i+1] = *(volatile unsigned char*)(address + i + 1);
//  FLASH_BASE
//  data[0] = *(unsigned char*)(address);
  
  for (i = 0; i < count; i += 2) {
    //*(volatile unsigned short*)(address + i) = (((unsigned short)data[i + 1]) << 8) + data[i];
//    data[i + 1] = (unsigned char)(((*(volatile unsigned short*)(address + i))) >> 8);
//    data[i] = (unsigned char)*(volatile unsigned short*)(address + i) & 0x00ff;
//    data[i] = (unsigned char)*(volatile unsigned short*)(address + i);

//    data[i] = *(volatile unsigned char*)(address + i);
//    data[i + 1] = *(volatile unsigned char*)(address + i + 1);    

//    data[i] = 0xff;
//    data[i + 1] = 0xff;

    data[i] = *(volatile unsigned char*)(address + i);
    data[i + 1] = *(volatile unsigned char*)(address + i + 1);
    
//    while (!(FLASH->SR & FLASH_SR_EOP));
//    FLASH->SR = FLASH_SR_EOP;
  }
  
//  FLASH->CR &= ~(FLASH_CR_PG);
}

/**
 * Initial settings struct in internal flash
 */
void flashFirst(void) {

  settings.temperatureLow = 20.5;
  settings.temperatureHight = 40.2;
  settings.humidityLow = 40.6;
  settings.humidityHight = 60.3;
  
  flashErase(MEMORY_PAGE);
  flashWrite((unsigned char *)&settings, MEMORY_PAGE, sizeof(settings));

}