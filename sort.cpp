#include "sort.h"

bool isLessThan(const std::tm &lhs, const std::tm &rhs) {
  std::time_t lhs_time = std::mktime(const_cast<std::tm *>(&lhs));
  std::time_t rhs_time = std::mktime(const_cast<std::tm *>(&rhs));
  return lhs_time < rhs_time;
}

bool compareByPriority(const Task &a, const Task &b) {
  if (a.priority == b.priority) {
    return isLessThan(a.due_date, b.due_date);
  }
  if (a.priority == "high")
    return true;
  if (b.priority == "high")
    return false;
  if (a.priority == "medium")
    return true;
  return false;
}

bool compareByDueDate(const Task &a, const Task &b) {
  return isLessThan(a.due_date, b.due_date);
}

void sortTasksByPriority(std::vector<Task> &tasks) {
  std::sort(tasks.begin(), tasks.end(), compareByPriority);
}

void sortTasksByDueDate(std::vector<Task> &tasks) {
  std::sort(tasks.begin(), tasks.end(), compareByDueDate);
}
