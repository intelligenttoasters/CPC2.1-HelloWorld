/*
 * spi.h
 *
 * Created: 4/12/2016 8:36:51 AM
 *  Author: paul
 */ 


#ifndef SPI1_H_
#define SPI1_H_

void spi_handle(void);
void spi_init(void);
Ctrl_status spi_test_unit_ready_0(void);
Ctrl_status spi_read_capacity_0(U32 *);
bool spi_wr_protect_0(void);
Ctrl_status spi_mem_2_ram_0 (U32 addr, void *ram);
Ctrl_status spi_ram_2_mem_0 (U32 addr, const void *ram);

#endif /* INCFILE1_H_ */