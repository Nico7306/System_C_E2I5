#include "systemc.h"
#include "top.h"

/* define the top module, containing all other submodules */
/* constructor of the top module, calling also the constructors of its submodules */

top::top (sc_module_name name) : sc_module (name), 	        
	fifo1 ("fifo1"), bitGen1 ("bitGen1"), bit2Byte1 ("bit2Byte1"), consumer1 ("consumer1")
{ 
	/* to be completed - exercise 2 - connect the BitGen module to the rest of the system, by its ports */
	/* Connecte le module BitGen */
	bitGen1.rdy(rdy);
	bitGen1.ack(ack);
	bitGen1.data_out(data);
	/* connect the Bit2Byte module with the rest of the system, by its ports */
	bit2Byte1.data_in (data);
	bit2Byte1.rdy (rdy);
	bit2Byte1.ack (ack);
	bit2Byte1.out (fifo1);
	/* connect the Consumer module with the rest of the system, by its ports */
	consumer1.in (fifo1);
}