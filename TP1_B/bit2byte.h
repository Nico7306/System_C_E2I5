#ifndef bit2byte_h
#define bit2byte_h

#include "write_if.h"

/* declare the Bit2Byte module */

class Bit2Byte : public sc_module
{
public:
	SC_HAS_PROCESS(Bit2Byte);	
	Bit2Byte (sc_module_name name);

private:
	/* declare the functions to read bits and, respectively, to produce bytes */

	void thRead ();
	void thProduce ();

public:
	/* declare the module ports */

	sc_in<bool>				data_in;
	sc_in<bool>				rdy;
	sc_out<bool>				ack;
	sc_port<write_if>			out;

private:
	char						data;				// attribute for storing the actual byte
	char						idx;				// index of the last read bit
	sc_event					byteReady;			// event to signalize that a byte is ready to be sent
	bool						bByteReady;			
	sc_event					byteFree;			// event to signalize that a byte was sent

};

#endif
