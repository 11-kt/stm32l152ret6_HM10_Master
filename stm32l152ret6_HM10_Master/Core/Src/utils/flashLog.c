/*
 * flashLog.c
 *
 *  Created on: Mar 21, 2024
 *      Author: Andrianov Vitaly
 */

#include "utils/flashLog.h"

uint32_t DATA_EEPROM_START_ADDR = (uint32_t) 0x08080000;	// start data flash addr
uint32_t DATA_EEPROM_END_ADDR	= (uint32_t) 0x08080FFF;	// end data flash addr
char timeStr[10];
RTC_DateTypeDef sDate = {0};
RTC_TimeTypeDef sTime = {0};
/**
  * @brief  Erase all data flash cells from DATA_EEPROM_START_ADDR to DATA_EEPROM_END_ADDR
  * @retval void
 */
void eraseDataFlash() {
    HAL_StatusTypeDef status;

    /* Calculate the number of pages to erase */
    uint32_t pageCount = ((DATA_EEPROM_END_ADDR - DATA_EEPROM_START_ADDR));

    /* Unlock the FLASH and erase the entire data flash */
    status = HAL_FLASHEx_DATAEEPROM_Unlock();
    if (status != HAL_OK) {
        /* Error handling */
        return;
    }

    /* Erase each page of the data flash */
    for (uint32_t page = 0; page < pageCount; page++) {
        status = HAL_FLASHEx_DATAEEPROM_Erase(0x00u, DATA_EEPROM_START_ADDR + (page));
        if (status != HAL_OK) {
            /* Error handling */
            return;
        }
    }

    status = HAL_FLASHEx_DATAEEPROM_Lock();
    if (status != HAL_OK) {
        /* Error handling */
        return;
    }
}

/**
  * @brief  Write string to current cell (addr)
  * @param  str - string value
  * @retval void
 */
void writeStringToDataFlash(const char *str) {
    HAL_StatusTypeDef status;

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
    status = HAL_FLASHEx_DATAEEPROM_Unlock();
    if (status != HAL_OK) {
        /* Error handling */
        return;
    }

    sprintf(timeStr, "%02d:%02d", Get_RTC_Minutes(), Get_RTC_Seconds());
    /* Write the string time to the data flash */
	for (size_t i = 0; i < len; i++) {
		/* Write data to current cell */
		status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_START_ADDR + i, timeStr[i]);
		if (status != HAL_OK) {
			/* Error handling */
			return;
		}
	}
    /* Next cell addr */
    DATA_EEPROM_START_ADDR += 0x8;

    /* Write the string to the data flash */
    for (size_t i = 0; i < len; i++) {
    	/* Write data to current cell */
        status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_START_ADDR + i, str[i]);
        if (status != HAL_OK) {
            /* Error handling */
            return;
        }
    }

    /* Write null terminator */
    status = HAL_FLASHEx_DATAEEPROM_Program(FLASH_TYPEPROGRAMDATA_BYTE, DATA_EEPROM_START_ADDR + len, '\0');
    if (status != HAL_OK) {
        /* Error handling */
        return;
    }

    status = HAL_FLASHEx_DATAEEPROM_Lock();
    if (status != HAL_OK) {
        /* Error handling */
        return;
    }
    /* Next cell addr */
    DATA_EEPROM_START_ADDR += 0x8;
}

uint8_t Get_RTC_Minutes(void) {
    return (uint8_t)((RTC->TR & RTC_TR_MNU_Msk) >> RTC_TR_MNU_Pos);
}

// Function to get seconds from RTC
uint8_t Get_RTC_Seconds(void) {
    return (uint8_t)((RTC->TR & RTC_TR_ST_Msk) >> RTC_TR_ST_Pos);
}
