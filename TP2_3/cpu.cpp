#include "cpu.h"

std::vector<CPU*>	CPU::s_cpus;	// CPUs in the system

/* CPU class definition */


bool ScheduleWrapper(Task* task, void* arg)
{
	//call the os scheduler on start-up 
	task->m_os->Schedule();
	return false;
}

CPU::CPU (sc_module_name name, unsigned long cpuID) : sc_module (name), tasks_state("tasks_state",MAX_TASK_PER_CPU)
{
	/* constructor: create the CPU and its OS */
		m_id = cpuID;
		m_os = new OS (cpuID, this);
		AddInitialTask(ScheduleWrapper,NULL,"scheduler",true);


		s_cpus.push_back (this);
}

CPU::~CPU()
{
	if (m_os)
		delete m_os;
}

void CPU::AddInitialTask (task_h taskProc, void* p, std::string name, bool resetTask)
{
	/* register the task through the OS of the CPU */
	m_os->RegisterTask (taskProc, p, name.c_str(), 4, resetTask);
}


