#include <sstream>
#include "top.h"

	Top::Top(sc_module_name name):sc_module(name),cpu1("CPU1",1),cpu2("CPU2",2),cpu1_tasks_state("cpu1_tasks_state",MAX_TASK_PER_CPU),cpu2_tasks_state("cpu2_tasks_state",MAX_TASK_PER_CPU)
	{
		cpu1.clock(clock);
		cpu1.interrupt(cpu1_interrupt);
		cpu1.current_task(cpu1_task);

		cpu2.clock(clock);
		cpu2.interrupt(cpu2_interrupt);
		cpu2.current_task(cpu2_task);

		cpu1.tasks_state.bind(cpu1_tasks_state);
		cpu2.tasks_state.bind(cpu2_tasks_state);
		
		SC_THREAD(driver);

		//Prepare the trace
	    sc_trace(g_trace_file, clock, "clock");
	    sc_trace(g_trace_file, cpu1_interrupt, "cpu1_itr");  
		sc_trace(g_trace_file, cpu1_task, "cpu1_task");
		sc_trace(g_trace_file, cpu2_interrupt, "cpu2_itr");  
		sc_trace(g_trace_file, cpu2_task, "cpu2_task");

		std::string s2="cpu2_task";
		for(unsigned int i=0;i<MAX_TASK_PER_CPU;i++)
		{
			std::stringstream s1;
			s1<<"cpu1_task"<<i<<"_state";
			sc_trace(g_trace_file, cpu1_tasks_state[i], s1.str().c_str());
			std::stringstream s2;
			s2<<"cpu2_task"<<i<<"_state";
			sc_trace(g_trace_file, cpu2_tasks_state[i], s2.str().c_str());	
		}
	
		cpu1_interrupt=false;
		cpu2_interrupt=false;		

	}


	void Top::driver()
	{
		clock=true;
		while(1)
		{
			wait(5, SC_NS);
			clock=!clock;
			cpu1.m_os->UpdateTrace();
			cpu2.m_os->UpdateTrace();
		}
	}


	
