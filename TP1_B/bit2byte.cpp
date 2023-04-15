#include "systemc.h"
#include "bit2byte.h"

/* define the Bit2Byte module */

/* constructor */

Bit2Byte::Bit2Byte(sc_module_name name) : sc_module (name)
{
	/* thRead is a thread sensitive to the positive edge of rdy */

	SC_THREAD(thRead);
	sensitive << rdy.pos ();
	
	/* thProduce is a thread */

	SC_THREAD(thProduce);

	data = 0;
	idx = 0;

	bByteReady = false;
}

/* function implementing the thRead thread, which reads bits using ports: data_in, rdy and ack */

void Bit2Byte::thRead ()
{
	bool		tbit;							// temporary variable to store the read bit

	ack = false;								// initialize ack
	
	while (1)
	{
		if (rdy == false)
			wait ();							// wait for ready

		tbit = data_in;							// read the bit; store it in tbit 
		
		wait (2, SC_NS);
		ack = true;								// activate ack
		
		if (rdy == true)						// after reading the bit,
			wait (rdy.negedge_event ());		//   wait for the protocol to finish: negative edge of rdy
		
		wait (2, SC_NS);
		ack = false;							// deactivate ack
		
		if (idx >= 8)							// idx = 8: the last byte was not sent yet, 9: it was sent 
		{
			if (idx == 8)						// if there are already 8 read bits (this would be the 9th), 
				wait (byteFree);				//   wait for the byte to be sent	
			idx = 0;							// reset the number of read bits
			data = 0;							// reset the actual byte
		}

		data = data | (tbit ? 1 : 0) << idx;	// put the new read bit in the byte
		idx ++;									// increment the number of read bits
		
		if (idx == 8)							// if this is the 8th read bit, notify a byte is ready to be sent
		{
			bByteReady = true;
			byteReady.notify ();
		}
	}
}

/* function implementing the thProduce thread, which writes bytes to the out port */

void Bit2Byte::thProduce ()
{
	char			tdata;

	while (1)
	{
		if (bByteReady == false)				// wait until there is a complete byte ready to be sent
			wait (byteReady);
		
		printf ("[%lu] Bit2Byte::thProduce: %X\n", (unsigned long) sc_simulation_time (), (unsigned int) data & 0xFF);
	
		tdata = data;							// put the byte in tdata
		bByteReady = false;						// reset bByteReady

		// wait (100, SC_NS);

		idx++;									// increment the number of read bits => idx = 9
		byteFree.notify ();						// notify the sending of the last byte

		out->write (tdata);						// write the last byte to the out port
	}
}
