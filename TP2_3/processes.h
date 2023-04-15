#include "os.h"


#include <string>


//functions implementing the simulated processes. 
//Parameters are:
//- Task*: a pointer to the process Task object, used for OS method and attribute reference
//- void*: a generic argument for process customization
//Return value is a boolean. If false, the process is stopped. Otherwise, the process is restarted immediately 
bool proc1 (Task*,void*);
bool proc2 (Task*,void*);
bool proc3 (Task*,void*);


//Structure used for transfering an  integer parameter through void pointer (without worrying the compiler)
struct IntHandler{
	IntHandler(int i):i(i)
	{}

	int i;
};

struct Proc4ArgumentHandler{
    Proc4ArgumentHandler(int a, int b, std::string s) : _a(a),  _b(b), _s(s)
    {}
    int _a, _b;
    std::string _s;
    
};


//Utility macro for tasks readability
#define CONSUME(t)  (task->m_os->Consume(task, t )); 
