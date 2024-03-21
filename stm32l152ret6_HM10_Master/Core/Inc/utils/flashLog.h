/*
 * flashLog.h
 *
 *  Created on: Mar 21, 2024
 *      Author: Andrianov Vitaly
 */

#ifndef INC_UTILS_FLASHLOG_H_
#define INC_UTILS_FLASHLOG_H_

#include <string.h>
#include <stdint.h>
#include "stm32l1xx_hal.h"

//--------------------------------------------------------------------------------//
	#define DATA_EEPROM_PAGE_SIZE   (uint32_t) FLASH_PAGE_SIZE	// size of 1 page
	#define DATA_EEPROM_BASE		(uint32_t) FLASH_BASE		// flash base addr
//--------------------------------------------------------------------------------//

//--------------------------------------------------------------------------------//
	void eraseDataFlash();							// erase all data flash memory
	void writeStringToDataFlash(const char *str);	// write string to flash cell
//--------------------------------------------------------------------------------//

#endif /* INC_UTILS_FLASHLOG_H_ */
