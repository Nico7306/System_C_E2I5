#include "systemc.h"
#include "bitgen.h"

/* define the BitGen module */

/* constructor */

BitGen::BitGen (sc_module_name name) : sc_module (name)
{
	SC_THREAD (thProduce);
}


/* definition of the thread thProduce */
void BitGen::thProduce()
{
	char c;
	ifstream dataFile("a.txt");

	while (dataFile.read(&c, sizeof(char))) {
		for (unsigned char i = 0; i < 8; i++){
			data_out = (c>>i) & 1;
			wait(2, SC_NS);

			rdy = true;
			wait(2, SC_NS);

			if (ack == false) {
				wait(2);
			}
			wait(2, SC_NS);
			
			rdy = false;
			wait(2, SC_NS);
		}
	}
}
