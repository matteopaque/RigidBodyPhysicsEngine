//
// Created by Clef on 1/5/26.
//

#include "Timer.h"


TaskID Timer::addTask(double LengthInSeconds, std::function<bool(int)> callback)
{
    TaskID id = lastID;
    lastID++;
    Task newTask = {LengthInSeconds, 0, 0, callback};
    tasks[id] = std::move(newTask);
    return id;
}

std::vector<std::pair<TaskID, bool>> Timer::update(double deltaTime)
{
    std::vector<std::pair<TaskID, bool>> results;
    std::vector <TaskID> toErase;
    for (auto & taskAndId : tasks )
    {
        const auto id = taskAndId.first;
        Task &task = taskAndId.second;
        task.taskCurrentTime += deltaTime;
        if (task.taskCurrentTime>= task.taskTimeLength)
        {
            const bool shouldRepeat = task.taskCallback(task.TimesCalledBefore);
            task.TimesCalledBefore++;
            if (shouldRepeat)
                task.taskCurrentTime -= task.taskTimeLength;
            else
                toErase.push_back(id);
            results.emplace_back(id, shouldRepeat);
        }
    }
    for (auto idToErase : toErase)
    {
        tasks.erase(idToErase);
    }
    return results;
}
