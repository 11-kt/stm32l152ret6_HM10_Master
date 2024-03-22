/*
 * flashLog.c
 *
 *  Created on: Mar 21, 2024
 *      Author: Andrianov Vitaly
 */

#include "utils/flashLog.h"

uint32_t DATA_EEPROM_START_ADDR = (uint32_t) 0x08080000;	// start data flash addr
uint32_t DATA_EEPROM_END_ADDR	= (uint32_t) 0x08080FFF;	// end data flash addr

/**
  * @brief  Erase all data flash cells from DATA_EEPROM_START_ADDR to DATA_EEPROM_END_ADDR
  * @retval void
 */
void eraseDataFlash() {
    /* Calculate the number of pages to erase */
    uint32_t pageCount = ((DATA_EEPROM_END_ADDR - DATA_EEPROM_START_ADDR));
    /* Unlock the FLASH and erase the entire data flash */
    HAL_FLASHEx_DATAEEPROM_Unlock();
    /* Erase each page of the data flash */
    for (uint32_t page = 0; page < pageCount; page++) {
        HAL_FLASHEx_DATAEEPROM_Erase(0x00u, DATA_EEPROM_START_ADDR + (page));
    }
    HAL_FLASHEx_DATAEEPROM_Lock();
}

/**
  * @brief  Write string to current cell (addr)
  * @param  str - string value
  * @retval void
 */
void writeStringToDataFlash(const char *str) {
	/* Check if the address is within the data flash range */
    if (DATA_EEPROM_START_ADDR < DATA_EEPROM_BASE || DATA_EEPROM_START_ADDR > DATA_EEPROM_END_ADDR)
        return;
    /* Calculate the length of the string */
    size_t len = strlen(str);
    /* Ensure the string fits within the data flash */
    if (len + 1 > (DATA_EEPROM_END_ADDR - DATA_EEPROM_START_ADDR)) {
    	/* String too long for the allocated space */
        return;
    }
    /* Erase the data flash page before writing */
    HAL_FLASHEx_DATAEEPROM_Unlock();
    /* Write the string to the data flash */
    for (size_t i = 0; i < len; i++) {
    	/* Write data to current cell */
        HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_START_ADDR + i, str[i]);
    }

    /* Write null terminator */
    HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_START_ADDR + len, '\0');
    HAL_FLASHEx_DATAEEPROM_Lock();
    /* Next cell addr */
    DATA_EEPROM_START_ADDR += 0x8;
}
