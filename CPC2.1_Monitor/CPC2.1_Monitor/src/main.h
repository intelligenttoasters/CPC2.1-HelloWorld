/*
 * main.h
 *
 * Created: 3/12/2016 10:05:29 AM
 *  Author: paul
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "fpga/fpga.h"
#include "usb/usb_handler.h"
#include "cmodem/cmodem.h"
#include "spi/spi.h"
#include "flash/flash.h"
#include "string.h"
#include "menu/menu.h"

// Required prototypes
void ftl_init(void);

#define stdio_ready() udi_cdc_is_rx_ready()

struct global_vars 
{
	// Global message buffer
	char returnMessage[80];

	// Flash Spare space
	uint8_t flash_buffer[PAGE_SIZE];
	uint8_t flash_spare[SPARE_SIZE];
};

#ifdef GLOBAL_SET_STRUCT
	struct global_vars globals;
#else
	extern struct global_vars globals;
#endif

#endif /* INCFILE1_H_ */