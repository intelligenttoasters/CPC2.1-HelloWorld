/*
 * main.c
 *
 * Boot code for CModem transmission.
 *
 * Part of the CPC2 project: http://intelligenttoasters.blog
 *
 * Copyright (C)2016  Intelligent.Toasters@gmail.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you can find a copy here:
 * https://www.gnu.org/licenses/gpl-3.0.en.html
 *
 */
#include "stdio.h"
#include "stdint.h"
#include "cmodem.h"

int main(int argc, char **argv)
{
    // Check we have a file
    if( argc != 2 )
    {
            fprintf(stderr, "Usage: %s <file>\n", argv[0] );
            return -1;
    }

    // Send the file to the TTY
    uint8_t r = send( argv[1] );

    if( r!=0 ) fprintf(stderr,"Error: %d\n", r);

    return 0;
}
