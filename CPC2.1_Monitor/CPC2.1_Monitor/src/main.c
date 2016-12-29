/**
 * main.c
 *
 * Main boot routine
 *
 */
#define GLOBAL_SET_STRUCT	// Sets the global variables here

#include <asf.h>
#include "main.h"
#include <string.h>

// External functions
void process_ui(void);

int main (void)
{
	while( true ) 
	{
		// Set up the board
		board_init();

		// Load FPGA image from flash
		load_flash_image();

		// Show the menu
		menu();
		
		// Disable the USB
		udc_stop();
		reset_fpga();
	}

	uint8_t data, data2[256], *ptr;

	// Test the MRAM Storage access
	Ctrl_status r = mem_test_unit_ready(LUN_ID_MRAM_MEM);
	U32 size; U8 buffer[SECTOR_SIZE * 16];
	r = mem_read_capacity(LUN_ID_MRAM_MEM,&size);
	/*
	memset( buffer, 0, SECTOR_SIZE * 16);
	strncpy( buffer, "Hello world from the MRAM!", 27);
	r = ram_2_memory(LUN_ID_MRAM_MEM, 0, &buffer);
	*/
	memset( buffer, 0, SECTOR_SIZE * 16);
	r = memory_2_ram(LUN_ID_MRAM_MEM, 0, &buffer);
	
	
	// Flash prep test
	/*
	extern struct nand_flash_ecc nf_ecc;
	uint8_t spare[64];
	for( int blk=0; blk<8192; blk++) {
//		for(int pg=0; pg<64; pg++) {
			nand_flash_raw_read_page(&nf_ecc, blk,0,buffer,spare);
			if( spare[0] != 0xff )
			{
				printf("Not blank!");
			}
		}
	int x = 1;
	if( x == 0 )
	nand_flash_raw_erase_block(&nf_ecc,4827);
	// End flash prep test
	*/
	
	// Test the flash storage access
	r = mem_test_unit_ready(LUN_ID_FLASH_MEM);
	r = mem_read_capacity(LUN_ID_FLASH_MEM,&size);
	
	memset( buffer, 0, SECTOR_SIZE * 16);
	r = memory_2_ram(LUN_ID_FLASH_MEM, 0, buffer);
	
	memset( buffer, 0, BLOCK_SIZE );
	memcpy(buffer, "Hello NAND Flash world!!!", 25);

	int x = 1;	
	if( x == 0 )
		r = ram_2_memory(LUN_ID_FLASH_MEM, 0, buffer);

	memset( buffer, 0, SECTOR_SIZE * 16);
	r = memory_2_ram(LUN_ID_FLASH_MEM, 0, buffer);
	
	// Send something to FPGA
	struct spi_device device = {.id = 1};
	ptr = data2;
	spi_select_device(SPI, &device);
	spi_read_packet(SPI, data2, 256 );
	spi_deselect_device(SPI, &device);
	
	// Main multi task loop
	while(1) {
		process_ui();
	}
}

