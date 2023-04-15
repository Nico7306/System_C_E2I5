#ifndef bitgen_h
#define bitgen_h

/* declare the BitGen module */

class BitGen : public sc_module
{
public: 
	SC_HAS_PROCESS(BitGen);
	BitGen(sc_module_name name);
	void thProduce();

	/* to be completed - exercise 2 - add the ports */
	sc_out<bool>				data_out;
	sc_out<bool>				rdy;
	sc_in<bool>					ack;
};

#endif



