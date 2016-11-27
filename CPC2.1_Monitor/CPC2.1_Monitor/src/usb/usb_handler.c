/*
 * CFile1.c
 *
 * Created: 19/11/2016 8:11:29 PM
 *  Author: paul
 */ 

#include "asf.h"

static void (*connect_callback)(U8, U8) = NULL;

void register_connect_callback(void *callback)
{
	connect_callback = callback;
}

void cdc_connect_event(U8 port, bool state) 
{
	if( (*connect_callback != NULL ) ) connect_callback(port, state);
}
