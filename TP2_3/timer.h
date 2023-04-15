
#include <systemc>

#define TIMER_PERIOD 1000

using namespace sc_core;


class Timer: public sc_module{
private:
	void main();
public:
	Timer(sc_module_name name);
	sc_out<bool>tick;
};
