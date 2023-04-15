#ifndef _OS_H_
#define _OS_H_

#include <systemc.h>
#include <kernel.h>
#include <channel.h>

#define task_null (Task *)0

//Constants for task tracing
#define TASK_CONTEXT_SWITCH 100
#define TASK_NO_TASK 200

//Defines the numberr of communications channel per OS
#define MAX_CHANNEL 4

//Enumeration representing the different states of tasks
enum TaskState { TASK_ACTIVE='a',
		 TASK_BLOCKED='b',
		 TASK_DESTROYED='d',
		 TASK_READY='r'
		};

class CPU;
class Task;
class OS;

/* Task class */


class Task
{
public:
	// constructor (os, handle to the function implementing the task, arguments, name, priority)
	Task (OS *os, task_h h, void *arg,const char *name, int index, int priority, bool resetTask); 
	
	void setState(TaskState state);
	TaskState getState(){return m_state;}
	
	~Task ();

public:
	OS						*m_os;
	TaskContext					m_context;
	int						m_uid; //Unique identifier to tasks
	int						m_index;//Index in the executing processor
	char						*m_name;
	int						m_priority;
	unsigned long					m_time;

private:

	TaskState				m_state;
	static int				count;		// counts the tasks created in the system; used for task unique IDs
};

/* structure used to link a task in a task queue */

struct TaskQueueItem
{
public:
	TaskQueueItem (Task *t) {m_task = t; m_next = 0;}

	TaskQueueItem				*m_next;
	Task					*m_task;
};

/* class implementing a task queue */

class TaskQueue
{
public:
	TaskQueue ();
	~TaskQueue ();

public:
	void Add (Task *t);
	void Add (TaskQueue *q);
	void AddAtEnd (Task *t);
	Task *RemoveFirst ();
	void Remove (Task *t);
	Task *First ();

public:
	TaskQueueItem				*m_head;
};

/* OS class */

class OS
{
public:
	OS (unsigned long idCPU, CPU* cpu);
	~OS ();

public:
	void RegisterTask (task_h h, void* arg, const char *name, int priority, bool resetTask);// register a task for this OS
	void TaskYield ();							// task yields the processor
	void TaskBlock (sc_event_or_list& blocking_list);			// -TP3- task is blocked (and yields the processor) 
	void TaskExit ();							// task exits
	Task* SelectNextTask ();						// select which task to execute next
	void Schedule ();							// the task schedule function
	void Consume (Task *task, unsigned long time);				// time execution of a given task


	long ChanIn (int idxChannel, char *buf, int &bufSize);			// -TP3- Called by the receiver on Single Emitter Single Receptor communication
	long ChanOut (int idxChannel, char *buf, int bufSize);			// -TP3- Called by the sender in both SESR and PESR communication schemes	
	//Called by the receiver on Plural Emitter Single Receptor communication
	//The argument 'channels' is a table of size 'nCHannels' including the indexes of all channels to listen to.   
	long AltIn (int nChannels, int *channels, char *buf, int &bufSize, int &fromChannel); //-TP3-

	void UpdateTrace();
public:
	Kernel						m_kernel;
	unsigned long					m_idCPU;
	CPU						*m_cpu;
	unsigned long					m_timeChange;		// necessary time to change the context
	 
	std::vector <Task*>				m_tasks;		// all the tasks registered by this OS
	TaskQueue					m_readyList;		// the ready task list
	Task						*m_currentTask;		// the active task
	TaskContext					*m_thisContext;		// the OS context


	Channel					m_channels[MAX_CHANNEL]; // -TP3- Channels for inter-tasks communication

};

//global functions
void Consume (unsigned long time);						// time execution of the active task
void TaskExit ();								// global function for task exit

#endif
