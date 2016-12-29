/*
 * spi.c - Handles requests through the SPI port
 *
 * Created: 2/12/2016 4:21:21 PM
 *  Author: paul
 */ 

#include "asf.h"
#include "main.h"

// Interrupt handler
ISR(SPI_Handler)
{
	int x = 1;
	for(x=1; x<2; x++) nop();
	// TODO: FInish this?
}

void spi_init()
{
	// Enable interrupts
	spi_enable_interrupt(SPI, SPI_IER_TXEMPTY);	// Or?  SPI_IER_ENDTX

	// Initialize MRAM at 40MHz
	struct spi_device device = {0};
	spi_master_init	( SPI );
	spi_master_setup_device(SPI, &device, SPI_MODE_0, 40000000, 0);
	device.id = 1;
	spi_master_setup_device(SPI, &device, SPI_MODE_0, 40000000, 0);
	spi_enable( SPI );

	// Write enable
	device.id = 0;
	spi_select_device(SPI, &device);
	spi_write_single ( SPI, 0x06 );
	spi_deselect_device(SPI, &device);

	// Write status to unprotect all banks
	spi_select_device(SPI, &device);
	spi_write_single ( SPI, 0x01 );
	spi_write_single ( SPI, 0xf3 );
	spi_deselect_device(SPI, &device);
}

Ctrl_status spi_test_unit_ready_0(void)
{
	return CTRL_GOOD;
}

Ctrl_status spi_read_capacity_0(U32 * size)
{
	// Size is fixed at 32KB
	*size = 64;
	return CTRL_GOOD;
}

bool spi_wr_protect_0(void)
{
	return false;
}

Ctrl_status spi_mem_2_ram_0 (U32 addr, void *ram)
{
	struct spi_device device = {
		.id = 0
	};

	// Address is in sectors, so multiply by sector size
	addr = addr * SECTOR_SIZE;
	
	// Read data bytes
	spi_select_device(SPI, &device);
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_single ( SPI, 0x03 );						// Read command
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_single ( SPI, (addr & 0xff00) >> 8 );		// Address-high
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_single ( SPI, (addr & 0xff) );			// Address-low
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_read_packet(SPI, ram, SECTOR_SIZE );			// Read a packet
	spi_deselect_device(SPI, &device);					// Deselect
	return CTRL_GOOD;
}

Ctrl_status spi_ram_2_mem_0 (U32 addr, const void *ram)
{
	struct spi_device device = {
		.id = 0
	};

	// Address is in sectors, so multiply by sector size
	addr = addr * SECTOR_SIZE;

	// Write data bytes
	spi_select_device(SPI, &device);
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_single ( SPI, 0x02 );						// Write command
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_single ( SPI, (addr & 0xff00) >> 8 );		// Address-high
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_single ( SPI, (addr & 0xff) );			// Address-low
	delay_us(1);										// Makes no sense, but OK it's needed
	spi_write_packet(SPI, ram, SECTOR_SIZE );			// Write a packet
	spi_deselect_device(SPI, &device);
	
	return CTRL_GOOD;	
}
