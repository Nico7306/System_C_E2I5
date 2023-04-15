
#include "cpu.h"
#include "timer.h"

using namespace sc_core;

class Top: public sc_module{
public:
	SC_HAS_PROCESS(Top);

	Top(sc_module_name name);

	void driver();

	CPU cpu1,cpu2;


	sc_signal<bool> clock;

	sc_signal<bool> cpu1_interrupt;
	sc_signal<char> cpu1_task;

	sc_signal<bool> cpu2_interrupt;
	sc_signal<char> cpu2_task;

	//Trace signals for cpu tasks' state 
	sc_vector< sc_signal<char> > cpu1_tasks_state;
	sc_vector< sc_signal<char> > cpu2_tasks_state;

};

	
