#ifndef read_if_h
#define read_if_h

/* declare the read interface */

class read_if : virtual public sc_interface
{
public:
	virtual void read (char &) = 0;		// a method to read an element of type char
	virtual int num_available () = 0;	// a method to get the number of available elements
};

#endif
