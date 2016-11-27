/**
 * main.c
 *
 * Main boot routine
 *
 */

#include <asf.h>
#include <string.h>
#include "fpga/fpga.h"
#include "cmodem/cmodem.h"

char status_msg[40];
volatile enum uint8_t {waiting_for_file,ready,runtime,closed} 
			 port0_state = waiting_for_file;
extern void register_connect_callback(void *);
void cb(U8 port, bool state);

/* Handles port state connections and disconnections */
void cb(U8 port, bool state)
{
	if( port == 0 ) 
	{
		// If not yet processed a received file, then don't do anything
		if( port0_state == waiting_for_file ) return;
		
		// Otherwise
		if( state == true ) {
			port0_state =  runtime;
		}
		else {
			port0_state = closed;
		}
	}
}

int main (void)
{
	while(1) {
		// Set up the board
		board_init();

		// Initialize USB stdio
		register_connect_callback(cb);
		stdio_usb_init();

		// Initialize the flash
		struct nand_flash_raw nf_raw;
		memset(&nf_raw,0,sizeof(nf_raw));
		int r = nand_flash_raw_initialize(&nf_raw, 0, BOARD_NF_COMMAND_ADDR, BOARD_NF_ADDRESS_ADDR, BOARD_NF_DATA_ADDR);

		// Set the port state to ready
		port0_state = ready;

		// Get program
		pgm_fpga_from_usb();

		// Wait for a connection before anything else
		while( port0_state != runtime ) nop();

		// Print the welcome message
		printf("\e[2J\e[HConnected\n\rMsg: %s\n\rSystem clock is %ld hz, peripheral bus is %ld hz\n\r",status_msg, sysclk_get_cpu_hz(), sysclk_get_peripheral_bus_hz(0));

		// Echo every character
		U8 c = 0;
		while( 1 ) {
			c = getchar();
			if( c != 27 )
				putchar(c);
			else
				break;
		}
		
		// Reset the port
		udc_stop();		
	}
}
