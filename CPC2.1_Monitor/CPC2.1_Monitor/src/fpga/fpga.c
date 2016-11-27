/*
 * fpga.c
 *
 * Created: 19/11/2016 3:45:42 PM
 *  Author: paul
 *    Desc: This library will program the FPGA from bitstreams passed to it either via STDIO(USB) or via a data request callback
 */ 

#include "string.h"
#include "delay.h"
#include "cmodem/cmodem.h"
#include "fpga/fpga.h"
#define XOFF 19
#define XON 17

// Module variables
extern char status_msg[40];

bool toggle_nconfig(void)
{
	ioport_set_pin_level(NCONFIG, false);
	ioport_set_pin_level(NCONFIG, true);
	return ioport_get_pin_level(NSTATUS);
}

static inline void dclk(unsigned char data) // Want this to be inline
{
	ioport_set_port_level(FPGA_PORT, CONFIG_DATA_MASK, (data << 18) );
	// Send clock high then low
	ioport_set_pin_level(DCLK, true); 
	ioport_set_pin_level(DCLK, false);
	// Send again, because we expect a compressed bitstream, which had a dclk-to-data = 2
	ioport_set_pin_level(DCLK, true); 
	ioport_set_pin_level(DCLK, false);
}

void init_fpga_ports(void)
{
	// Set the initial conditions
	dclk(0);
	
	// Hold nConfig low and keep it there
	ioport_set_pin_level(NCONFIG, false);

	// Set the pull ups
	ioport_set_pin_mode(NSTATUS, IOPORT_MODE_PULLUP);
	ioport_set_pin_mode(CONF_DONE, IOPORT_MODE_PULLUP);

	// Set the pin directions
	ioport_set_pin_dir(DCLK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(CONF_DONE, IOPORT_DIR_INPUT);
	ioport_set_port_dir(FPGA_PORT, CONFIG_DATA_MASK, IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(NSTATUS, IOPORT_DIR_INPUT);
	ioport_set_pin_dir(NCONFIG, IOPORT_DIR_OUTPUT);
}

void pgm_fpga_from_usb(void)
{
	U8 *buffer;
	int32_t result;
	U32 total = 0;
	bool nstatus;
	
	// Clear the message buffer
	memset( status_msg, 0, 40 );
	
	// Minimum nConfig pulse width is 2uS
	ioport_set_pin_level(NCONFIG, false);
	delay_us(2);		
	ioport_set_pin_level(NCONFIG, true);
	
	// Wait for nSTATUS to go high
	while( ( ioport_get_pin_level(NSTATUS) ) == 0 ) nop();
	
	// The specification calls for a 2uS delay after nStatus goes high
	delay_us(2);		

	// Read some data until an error or done
	while( ( result = receive(&buffer, status_msg) ) > 0 )
	{
		// Accumulate the total for reporting
		total += result;
		
		// Transmit data to FPGA
		for( int cntr=0; cntr<result; cntr++ )
			dclk(buffer[cntr]);	// Transmit the data
	}
	
	// Spec requires two additional DCLK after config sent
	dclk(0); dclk(0);
	
	// Check if there was an error in the USB transmission
	if( result < 0 )
		sprintf(status_msg,"Error receiving file after %ld bytes",total);
	else {
		// If not, then check the config done pin is released
		// Check the config done pin
		if(ioport_get_pin_level(CONF_DONE) == false)
		{
			nstatus = ioport_get_pin_level(NSTATUS);
			sprintf(status_msg,"Failed to configure after receiving %ld bytes, status=%d",total,nstatus);
		}
		else
		// Show what's received
			sprintf(status_msg,"Received %ld bytes, and CONF_DONE released",total);
	}
}