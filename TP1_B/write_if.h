#ifndef write_if_h
#define write_if_h

/* declare the write interface */

class write_if : virtual public sc_interface
{
public:
	virtual void write (char) = 0;		// a method to write an element of type char
	virtual void reset () = 0;			// a method to reset the content
};

#endif
