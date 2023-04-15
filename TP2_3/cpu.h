#ifndef _CPU_H_
#define _CPU_H_

#include "os.h"

//Maximum number of tasks in one CPU
#define MAX_TASK_PER_CPU  11

//Maximum number of channels
#define MAX_CHANNEL  4

/* CPU class declaration */

class CPU : public sc_module
{
public:
	SC_HAS_PROCESS(CPU);

	CPU (sc_module_name name, unsigned long cpuID);
	virtual ~CPU();


	//SystemC Ports
	sc_in<bool> clock;
	sc_in<bool> interrupt;
	sc_out<char> current_task;
	sc_vector< sc_out<char> > tasks_state; //tasks state signals
	

public:
	void AddInitialTask (task_h taskProc, void* p, std::string name, bool resetTask=false);	// add task to CPU

	OS				*m_os;		// corresponding OS

	unsigned long			m_id;		// id

	static std::vector<CPU*>	s_cpus;	// CPUs in the system
};

#endif

