#include "timer.h"


void Timer::int main()
{
    bool state=true;

    while(true){
        tick=state;
        wait(1,SC_US);
        //toggle state
        state=!state;
    }
}
