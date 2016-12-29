/*
 * menu.c
 *
 * Created: 26/12/2016 7:44:51 PM
 *  Author: paul
 */ 

#include "main.h"

void monitor(void) {};

static const char *codes[] = {
	"\e[2J",
	"\e[%d;%dH",
	"\e[%dm",
	"\e[K",
	"\e[A",
	"\e[B",
	"\e[C",
	"\e[D",
	"\e[s",
	"\e[u"
};

#define MENU_ITEMS 6
static const char *menu_item[] = {
	" 1 - Upload FPGA image to device",
	" 2 - Upload FPGA image to device and connect to monitor port",
	" 3 - Upload FPGA image to flash",
	" 4 - Upload FPGA image to flash and reset",
	" 5 - Reset",
	" 6 - Clear FPGA image from flash"
};

static const char *messages[] = {
	"CPC2.0 Monitor V0.1 - Main Menu",
	"Options: ",
	"Select: ",
	"Waiting for program...",
	"Flash image reset",
	"Resetting device",
	"Sure? Y/N"
};

void menu()
{
	uint8_t cntr, option;

	while(true)
	{
		// Wait for connection
		while( !isPortOpen()) nop();
		
		// Reset attrs
		ATTR(NORM);
		GRA(CLS);

		// Print the last message
		LOCATE( 1, 20 );
		printf("Last message: \n\r%s", globals.returnMessage);

		// Print the title
		LOCATE( 2, 1 );
		M(0);	// Title

		LOCATE( 5, 3 );
		GRA(SAVE);
		M(1);	// Options
		GRA(REST);
		GRA(DOWN);
		GRA(DOWN);

		for( cntr=0; cntr<MENU_ITEMS; cntr++ )
		{
			GRA(SAVE);
			printf(menu_item[cntr]);
			GRA(REST);
			GRA(DOWN);
		}
		GRA(DOWN);
		GRA(DOWN);
		GRA(DOWN);
		GRA(SAVE);
		GRA(UP);		
		GRA(UP);
		
		M(2);	// Selection prompt

		// Get option (not null!)
		while( 1 ) {
			if (!isPortOpen()) break;
			if( stdio_ready() )
				if( (option = getchar()) != 0 ) 
					if( ( option >= '1' ) && ( option <= '6' ) ) break;
		}

		if (isPortOpen())
		{
			// Relocate cursor back
			GRA(REST);
			
			switch( option )
			{
				case '1':
					M(3);	// Waiting
					pgm_fpga_from_usb(globals.returnMessage);
					delay_ms(500);
					break;
				case '2':
					M(3);	// Waiting
					pgm_fpga_from_usb(globals.returnMessage);
					delay_ms(500);
					monitor();
					break;
				case '3':
					M(3);	// Waiting
					store_flash_image();
					break;
				case '4':
					M(3);	// Waiting
					store_flash_image();
					M(5);	// Resetting
					delay_s(1);
					return;	// Returning causes a reset
				case '5':
					M(5);	// Resetting
					return;
				case '6':
					M(6);
					if( getchar() != 'Y' ) 
					{
						sprintf(globals.returnMessage,"Aborted erase");
						break;
					}
					// Otherwise
					memset( globals.flash_buffer, 0, SECTOR_SIZE );
					ram_2_memory(LUN_ID_MRAM_MEM, 0, globals.flash_buffer);
					sprintf(globals.returnMessage, messages[4]);	// Reset flash
					break;
			}
		}
	}
}
