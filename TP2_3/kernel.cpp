#include <kernel.h>
#include <os.h>


sc_trace_file *g_trace_file;

void TaskExit ();

/* TaskContext class definition */

TaskContext::TaskContext (launcher_h launch, task_h h, Task* task, void *arg, bool resetTask)
{
	if (h == NULL)
	{
		cerr<<"ERROR: You tried to create a task without defining a function pointer (NULL)! "<<endl;
		return;
	}

	//Create the systemC thread running the task	
	m_handle=sc_spawn(sc_bind(launch,h,task,arg));
	if(!resetTask)
		m_handle.suspend();
}

void Kernel::BlockContext(TaskContext	*oldContext, TaskContext *newContext)
{
	if(newContext!=NULL)
		newContext->m_handle.resume();

	//wait for the task context's SystemC event list 
	if(oldContext!=NULL)
		wait(oldContext->m_blocking_list);
}

/* Kernel class definition */

void Kernel::SwitchContext (TaskContext *oldContext, TaskContext *newContext)
{
	//Call to SystemC 2.3 dynamic thread methods for switching tasks
	if(newContext!=NULL)
		newContext->m_handle.resume();
	if(oldContext!=NULL)
		oldContext->m_handle.suspend();
}
