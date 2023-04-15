#include "systemc.h"
#include "consumer.h"

/* define the Consumer module */

/* constructor */

Consumer::Consumer(sc_module_name name) 
{
	SC_THREAD(thConsume);				// thConsume is a thread
}

void Consumer::thConsume()
{
	char car;
	std::string full_data = "";

	while (true) {
		in->read(car);
		full_data += car;
		printf("[%lu] char read %c\n",(unsigned long)(sc_time_stamp().value()/1000),car
		);
		printf("### Data is: %s\n", full_data.c_str());
		wait(20, SC_NS);
		}
}
