#ifndef TIMER_H
#define TIMER_H

#include <ctime>

using namespace std;

class Timer 
{
private:
    time_t start,end;
public:
    Timer() {
        start = 0;
        end = 0;
    }

    void set_Start(time_t time) {
        start = time;
    }

    void set_End(time_t time) {
        end = time; 
    }

    time_t timeDiff() {
        return difftime(end,start);
    }
};

#endif 