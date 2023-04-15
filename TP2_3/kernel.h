#ifndef KERNEL_H_
#define KERNEL_H_

#define SC_INCLUDE_DYNAMIC_PROCESSES
#include <systemc.h>


class Task;

//define the global trace file
extern sc_trace_file *g_trace_file;


//Define a type "pointer to a function returning a boolean and taking two arguments respectively of type OS* and void*"
//Within the OS, the function shall return true for the task to be restarted immediately, or false for the task to be deleted.  
typedef bool (*task_h) (Task*,void *);

//Define type for internal task launch
typedef void (*launcher_h) (task_h,Task*,void *);

class TaskContext
{
public:
	TaskContext (launcher_h launcher,task_h h,Task* task,void *arg, bool resetTask);
public:
	sc_process_handle				m_handle;
	sc_event_or_list				m_blocking_list;
};

class Kernel
{
public:
	void SwitchContext (TaskContext	*oldContext, TaskContext *newContext);
	void BlockContext  (TaskContext	*oldContext, TaskContext *newContext);
};

#endif
