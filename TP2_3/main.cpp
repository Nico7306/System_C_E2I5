#include <string>
#include "top.h"

#include "processes.h"

int sc_main (int, char**)
{

	// create some CPUs and add some processes (tasks) on them

    // instantiate the global trace file
	g_trace_file = sc_create_vcd_trace_file("trace");         // create a trace file
	g_trace_file->set_time_unit(1,SC_NS);
        
	Top top("top");
	IntHandler par1(1);
  	IntHandler par2(2);
	IntHandler par3(3);
    top.cpu1.AddInitialTask (proc1, (void*) &par1, "cpu1 - proc1");
    top.cpu1.AddInitialTask (proc2, (void*) &par2, "cpu1 - proc2");
	top.cpu2.AddInitialTask (proc3, (void*)&par3, "cpu2 - proc3");


	sc_start (200000, SC_NS);

        sc_close_vcd_trace_file(g_trace_file);             

	return 0;
}
