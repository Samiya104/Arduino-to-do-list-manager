#ifndef TASK_H
#define TASK_H

#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <algorithm>

struct Task {
    int id;
    std::string name;
    std::string priority;
    std::tm due_date;
    bool completed;
    bool deleted;
};

bool operator==(const Task& lhs, const Task& rhs);

class TodoList {
public:
    void addTask(const Task& task);
    void deleteTask();
    void completeTask();
    void viewTasks();
    void viewCompletedTasks();
    void viewDeletedTasks();
    Task* findTaskById(int id, bool checkDeleted = false);
    void saveToFile();
    void loadFromFile();

private:
    std::vector<Task> active_tasks;
    std::vector<Task> completed_tasks;
    std::vector<Task> deleted_tasks;
};

#endif // TASK_H
