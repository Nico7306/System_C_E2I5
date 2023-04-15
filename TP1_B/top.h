#ifndef top_h
#define top_h

#include "bitgen.h"
#include "bit2byte.h"
#include "consumer.h"
#include "fifo.h"

/* declare the top module, containing all other submodules */

class top : public sc_module
{
public:
	top(sc_module_name name);				// constructor

private:	
	
	/* declare the submodules */
	FIFO					fifo1;			
	BitGen					bitGen1;		
	Bit2Byte				bit2Byte1;		
	Consumer				consumer1;		

public:
	/* declare the signals linking BitGen and Bit2Byte */
	sc_signal<bool>			rdy;			
	sc_signal<bool>			ack;
	sc_signal<bool>			data;

};
#endif
