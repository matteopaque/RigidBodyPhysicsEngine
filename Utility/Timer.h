//
// Created by Clef on 1/5/26.
//

#ifndef OPENGLSETUP_TIMER_H
#define OPENGLSETUP_TIMER_H
#include <functional>
#include <map>

//should make another for lifetime

// takes in length and function and calls function after length.
// can remove task manually using id returned by add,
// and the timer only repeats if the function returns true
using TaskID = int;
class Timer
{
    TaskID lastID = 0;
    struct Task
    {
        double taskTimeLength;
        double taskCurrentTime;
        double TimesCalledBefore;
        std::function<bool(int)> taskCallback;
    };
    std::map<TaskID, Task> tasks;
public:
    TaskID addTask(double LengthInSeconds, std::function<bool(int)> callback);
    //returns vector of all the ids which have been run this time,
    // and a bool which is true if the function will repeat again,
    // and false if the task has ended
    std::vector<std::pair<TaskID, bool>> update(double deltaTime);

    void removeTask(TaskID toRemove)
    {
        tasks.erase(toRemove);
    }

};




#endif //OPENGLSETUP_TIMER_H
