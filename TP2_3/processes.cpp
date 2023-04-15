#include "processes.h"



/* file containing the functions implementing the processes (also called tasks) */

bool proc1 (Task* task, void* p)
{

	IntHandler* par=(IntHandler*)p;
	Proc4ArgumentHandler *args=new Proc4ArgumentHandler(4,5,"Test")

	cout << task->m_name << " "<< par->i << endl;

	CONSUME(par->i * 100);

	task->m_os->RegisterTask(proc5,(void*)args),"cpu1-proc5",4,false);
	return true;
}

bool proc2 (Task* task,void* p)
{
	IntHandler* par=(IntHandler*)p;

	cout << task->m_name << " "<< par->i << endl;

	CONSUME(par->i * 100);
	return true;
}

bool proc3 (Task* task,void* p)
{
	IntHandler* par=(IntHandler*)p;

	cout << task->m_name << " "<< par->i << endl;

	CONSUME(par->i * 100);

	return true;
}


bool proc4 (Task* task,void* p)
{
	Proc4ArgumentHandler *args = (Proc4ArgumentHandler *) p;
	std::cout << args->_a << " + " << args->_b << " = " << args->_a + args->_b << ", " << args->_s << std::endl;
	CONSUME(50);	
	return true;
}

bool proc5 (Task* task,void* p)
{
	Proc4ArgumentHandler *args=(Proc4ArgumentHandler*)p;
	std::cout << par->_a << " + " << par->_b << " = " << par->_a + par->_b << ", " << par->_s << std::endl;
	CONSUME(50);
	delete par;
	//Do not restart the process
	return false;
}

