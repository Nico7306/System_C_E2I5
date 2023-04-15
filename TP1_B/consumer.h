#ifndef consumer_h
#define consumer_h

#include "read_if.h"

/* declare the Consumer module */

class Consumer : public sc_module
{
public:
	SC_HAS_PROCESS(Consumer);	
	Consumer (sc_module_name name);

private:
	void thConsume ();

public:
	sc_port<read_if> in;		// the in port
};

#endif
