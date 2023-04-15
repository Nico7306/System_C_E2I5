#include <os.h>
#include <cpu.h>

/* TaskQueue class definitions */

TaskQueue::TaskQueue ()
{
	m_head = 0;
}

TaskQueue::~TaskQueue ()
{
	TaskQueueItem		*toDel;

	while (m_head)
	{
		toDel = m_head;
		m_head = m_head->m_next;

		delete toDel;
	}
}

/* add a task by priority */
void TaskQueue::Add (Task *t)
{
	TaskQueueItem		*tqi = new TaskQueueItem (t);
	TaskQueueItem		**p = &m_head;
	
	while (*p != 0 && t->m_priority >= (*p)->m_task->m_priority)
		p = &(*p)->m_next;

	tqi->m_next = *p;
	*p = tqi;
}

/* add a task queue by priority */
void TaskQueue::Add (TaskQueue *q)
{
	TaskQueueItem		*p = q->m_head;

	while (p)
	{
		Add (p->m_task);
		
		p = p->m_next;
	}
}

/* add a task at the end of the queue */

void TaskQueue::AddAtEnd (Task *t)
{
	TaskQueueItem		*tqi = new TaskQueueItem (t);
	TaskQueueItem		**p = &m_head;

	while (*p != 0)
		p = &(*p)->m_next;

	tqi->m_next = *p;
	*p = tqi;
}

/* remove the first task from the queue */

Task *TaskQueue::RemoveFirst ()
{
	if (m_head == 0)
		return 0;

	Task				*ret;
	TaskQueueItem		*toDel;
	ret = m_head->m_task;
	toDel = m_head;
	m_head = m_head->m_next;

	delete toDel;

	return ret;
}

/* remove a specified task from the queue */

void TaskQueue::Remove (Task *t)
{
	TaskQueueItem		**p = &m_head, *toDel;

	while (*p != 0)
	{
		if ((*p)->m_task == t)
		{
			toDel = *p;
			*p = (*p)->m_next;

			delete toDel;

			break;
		}
		p = &(*p)->m_next;
	}
}

/* return the head task */

Task *TaskQueue::First ()
{
	if (m_head == 0)
		return 0;

	return m_head->m_task;
}


/* Task class definitions */
int Task::count=0;

//Utility function for wrapping processes' functions
void launcher(task_h h, Task* task, void* arg)
{
	while( h(task,arg))
	{
		task->m_os->TaskYield();
 	}
	task->m_os->TaskExit();
}


Task::Task (OS *os, task_h h, void *arg,const char *name,int index, int priority, bool resetTask) 
	: m_context (launcher, h, this,(void*) arg, resetTask)
{
	m_os = os;
	m_uid = ++count;
	m_index=index;
	m_name = new char[strlen (name) + 1];
	strcpy (m_name, name);
	m_priority = priority;
	
	m_time = 0;	// time portion remained from a time slice
}

void Task::setState(TaskState state)
{
	m_state=state;
}

Task::~Task ()
{
	if (m_name)
		delete [] m_name;
}


/* OS class definitions */
OS::OS (unsigned long idCPU, CPU* cpu) : m_thisContext (NULL)
{
	m_idCPU = idCPU;
	m_cpu = cpu;

	m_currentTask = task_null;
	m_timeChange = 10;
}

OS::~OS ()
{
	// delete all its tasks
	
	std::vector <Task*>::iterator		it;

	it = m_tasks.begin ();
	while (it != m_tasks.end())
	{
		delete (*it);
		it++;
	}
}


void OS::RegisterTask (task_h h, void* arg, const char *name, int priority, bool resetTask)
{
	int index=m_tasks.size();
	assert(index<MAX_TASK_PER_CPU);

	Task	*task = new Task (this , h, arg, name,index, priority,resetTask);

	m_tasks.push_back (task);

	if(resetTask)
		m_thisContext=&(task->m_context); //This is the task to launch on reset 
	else
		m_readyList.AddAtEnd (task); //This is an applicative task, add this to scheduler list
}

void OS::TaskYield ()
{
	// interrupt the execution of the current (active) task

	if (!m_currentTask)
		return;

	m_currentTask->setState(TASK_READY);

	m_readyList.AddAtEnd (m_currentTask);

	m_kernel.SwitchContext (&m_currentTask->m_context, m_thisContext); 
}

void OS::TaskBlock ( sc_event_or_list& blocking_list)
{
	// interrupt the execution of the current (active) task

	if (!m_currentTask)
		return;

	Task* blockTask=m_currentTask;
	
	blockTask->m_context.m_blocking_list=blocking_list;
	blockTask->setState(TASK_BLOCKED);
	
	m_kernel.BlockContext(&(blockTask->m_context), m_thisContext); 

	//Once blocking event has been triggered, change task's state and add it back to ready list...
 
	blockTask->setState(TASK_READY);

	m_readyList.AddAtEnd (blockTask);

	//... and wait for the scheduler to launch it
	m_kernel.SwitchContext (&blockTask->m_context,NULL);
}


void OS::TaskExit ()
{
	// erase the task from the task vector and end its execution

	if (!m_currentTask)
		return;

	std::vector <Task *>::iterator		it;
	it = m_tasks.begin ();
	while (it != m_tasks.end ())
	{
		if (*it == m_currentTask)
		{
			m_tasks.erase (it);
			break;
		}
		it++;
	}

	m_currentTask->setState(TASK_DESTROYED); 
	m_kernel.SwitchContext (&m_currentTask->m_context, m_thisContext);
}
Task* OS::SelectNextTask()
{
	//Simply return the first task in the ready tasks queue 
	return 	m_readyList.RemoveFirst ();
}


void OS::Schedule ()
{
	/* perform the task schedule - the first ready task becomes the active one
		Then, if finished, it is deleted
	*/

	while(1)
	{
		while ((m_currentTask = SelectNextTask()) != task_null)
		{
			m_cpu->current_task=TASK_CONTEXT_SWITCH;

			for(unsigned int i=0; i< m_timeChange; i++)
				wait(m_cpu->clock.posedge_event());
			

			m_currentTask->m_time = 0;
			m_currentTask->setState(TASK_ACTIVE);
			m_cpu->current_task=(char)m_currentTask->m_index;

			m_kernel.SwitchContext (m_thisContext, &m_currentTask->m_context);

			if (m_currentTask && m_currentTask->getState() == TASK_DESTROYED)
			{
				delete m_currentTask;
			}
		}
		
		// if there is no ready task, wait...
		m_cpu->current_task=TASK_NO_TASK;
		wait(m_cpu->clock.posedge_event());

	}
}

void OS::Consume (Task *task, unsigned long time)
{
	/* model the execution in time of tasks 
	time is the requested execution time  
	*/

	long			t = task->m_time + time;	// new necessary execution time since the task is active

	for( ; t>=0; t--)
	{
		wait (m_cpu->clock.posedge_event());		// consume one clock cycle
		task->m_time = t;			// update the time consumed by the task
		
		if(m_cpu->interrupt)
			TaskYield();
	}

	task->m_time = t;			// update the time consumed by the task

}

long OS::ChanIn (int idxChannel, char *buf, int &bufSize)
{
	//To be completed
	
	return 0;
}


long OS::ChanOut (int idxChannel, char *buf, int bufSize)
{
	//Introduce a reference to 'simplify' the code
	Channel& myChannel= m_channels[idxChannel];

	if(myChannel.isSet())
	{

		//Check buffer sizes
		unsigned int size=bufSize;
		if(bufSize > myChannel.getBufferSize())
		{
			cerr<<" Could not transfer all data because ChanOut buffer size is too large!"<<endl;
			size=myChannel.getBufferSize();
		}

		//I arrived second at the rendez-vous, do the transfer
		memcpy(myChannel.getBuffer(),buf,size);
		
		//Reset the channel for future usage
		myChannel.reset();

		myChannel.m_event.notify();

	}else{
		//I arrived first at the rendez-vous, set the channel
		myChannel.set(buf, bufSize);

		sc_event_or_list blockingEvent(myChannel.m_event);
		TaskBlock(blockingEvent);
	}

	return 0;
}

long OS::AltIn (int nChannels, int *channels, char *buf, int &bufSize, int &fromChannel)
{
	//To be completed
	return 0;
}

void OS::UpdateTrace()
{
	for(unsigned int i=0;i<m_tasks.size();i++)
		m_cpu->tasks_state[i].write(m_tasks[i]->getState());
}


