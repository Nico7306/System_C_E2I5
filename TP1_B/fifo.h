#ifndef fifo_h
#define fifo_h

#include "write_if.h"
#include "read_if.h"

/* declare the FIFO channel, implementing the two interfaces, write_if and read_if */

class FIFO : public sc_channel, public write_if, public read_if
{
public:
	/* constructor */
	FIFO (sc_module_name name);

public:
	/* read_if implementation: declarations */
	virtual void read(char &c);
	virtual int num_available();

	/* write_if implementation: declarations */
	virtual void write (char c);
	virtual void reset();

private:
	enum e					{max = 5};			// the size of the array containing the FIFO elements
	char					data[max];			// the array containing the FIFO elements
	int						num_elements;		// the actual number of elements in the FIFO
	int						first;				// the position in the array of the first element
	sc_event				write_event;		// the write event
	sc_event				read_event;			// the read event
};

#endif
