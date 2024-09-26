#include "task.h"
#include "sort.h"
#include <fstream>
#include <iomanip>
#include <sstream>

bool operator==(const Task &lhs, const Task &rhs) { return lhs.id == rhs.id; }

void TodoList::addTask(const Task &task) {
  active_tasks.push_back(task);
  saveToFile();
}

void TodoList::deleteTask() {
  std::string inputLine;
  int id;
  std::cout << "Enter task ID: ";
  std::getline(std::cin, inputLine);
  try {
    id = std::stoi(inputLine);
  } catch (const std::invalid_argument &) {
    std::cout << "Invalid input. Please enter a numeric value for Task ID.\n";
    return;
  }
  Task *task = findTaskById(id);
  if (task == nullptr) {
    std::cout << "Task not found." << std::endl;
    return;
  }
  task->deleted = true;
  active_tasks.erase(
      std::remove(active_tasks.begin(), active_tasks.end(), *task),
      active_tasks.end());
  deleted_tasks.push_back(*task);
  saveToFile();
  std::cout << "Task deleted successfully." << std::endl;
}

void TodoList::completeTask() {
  std::string inputLine;
  int id;
  std::cout << "Enter task ID: ";
  std::getline(std::cin, inputLine);
  try {
    id = std::stoi(inputLine);
  } catch (const std::invalid_argument &) {
    std::cout << "Invalid input. Please enter a numeric value for Task ID.\n";
    return;
  }
  Task *task = findTaskById(id);
  if (task == nullptr) {
    std::cout << "Task not found." << std::endl;
    return;
  }
  task->completed = true;
  active_tasks.erase(
      std::remove(active_tasks.begin(), active_tasks.end(), *task),
      active_tasks.end());
  completed_tasks.push_back(*task);
  saveToFile();
  std::cout << "Task marked as completed successfully." << std::endl;
}

void TodoList::viewTasks() {
  if (active_tasks.empty()) {
    std::cout << "No ongoing tasks." << std::endl;
    return;
  }

  int option;
  std::string inputLine;
  std::cout << "View tasks by:" << std::endl;
  std::cout << "1. Priority" << std::endl;
  std::cout << "2. Due date" << std::endl;
  std::getline(std::cin, inputLine);
  try {
    option = std::stoi(inputLine);
  } catch (const std::invalid_argument &) {
    std::cout << "Invalid input. Please enter a numeric value for option \n";
    return;
  }

  if (option == 1) {
    sortTasksByPriority(active_tasks);
    std::cout << "Ongoing tasks by priority:" << std::endl;
  } else if (option == 2) {
    sortTasksByDueDate(active_tasks);
    std::cout << "Ongoing tasks by due date:" << std::endl;
  } else {
    std::cout << "Invalid option." << std::endl;
    return;
  }
  for (Task task : active_tasks) {
    // Format the due date for output
    char due_date_str[11];
    std::strftime(due_date_str, sizeof(due_date_str), "%Y-%m-%d",
                  &task.due_date);
    std::cout << task.name << " (priority: " << task.priority
              << ", due date: " << due_date_str << ")" << std::endl;
  }
}

void TodoList::viewCompletedTasks() {
  std::cout << "Completed tasks:" << std::endl;
  for (Task task : completed_tasks) {
    // Format the due date for output
    char due_date_str[11];
    std::strftime(due_date_str, sizeof(due_date_str), "%Y-%m-%d",
                  &task.due_date);
    std::cout << task.name << " (priority: " << task.priority
              << ", due date: " << due_date_str << ")" << std::endl;
  }
}

void TodoList::viewDeletedTasks() {
  std::cout << "Deleted tasks:" << std::endl;
  for (Task task : deleted_tasks) {
    // Format the due date for output
    char due_date_str[11];
    std::strftime(due_date_str, sizeof(due_date_str), "%Y-%m-%d",
                  &task.due_date);
    std::cout << task.name << " (priority: " << task.priority
              << ", due date: " << due_date_str << ")" << std::endl;
  }
}

Task *TodoList::findTaskById(int id, bool checkDeleted) {
  for (Task &task : active_tasks) {
    if (task.id == id) {
      return &task;
    }
  }
  if (checkDeleted) {
    for (Task &task : deleted_tasks) {
      if (task.id == id) {
        return &task;
      }
    }
  }
  return nullptr;
}

void TodoList::saveToFile() {
  std::ofstream ongoing_tasks_file("ongoing_tasks.txt");
  std::ofstream completed_tasks_file("completed_tasks.txt");
  std::ofstream deleted_tasks_file("deleted_tasks.txt");
  for (Task task : active_tasks) {
    ongoing_tasks_file << task.id << "," << task.name << "," << task.priority
                       << "," << std::put_time(&task.due_date, "%Y-%m-%d")
                       << "\n";
  }
  for (Task task : completed_tasks) {
    completed_tasks_file << task.id << "," << task.name << "," << task.priority
                         << "," << std::put_time(&task.due_date, "%Y-%m-%d")
                         << "\n";
  }
  for (Task task : deleted_tasks) {
    deleted_tasks_file << task.id << "," << task.name << "," << task.priority
                       << "," << std::put_time(&task.due_date, "%Y-%m-%d")
                       << "\n";
  }

  ongoing_tasks_file.close();
  completed_tasks_file.close();
  deleted_tasks_file.close();
}
void TodoList::loadFromFile() {
  active_tasks.clear();
  completed_tasks.clear();
  deleted_tasks.clear();

  std::ifstream ongoing_tasks_file("ongoing_tasks.txt");
  std::ifstream completed_tasks_file("completed_tasks.txt");
  std::ifstream deleted_tasks_file("deleted_tasks.txt");

  // Helper function to parse and load tasks from a file
  auto loadTasks = [](std::ifstream &file, bool completed, bool deleted,
                      std::vector<Task> &tasks) {
    if (!file.is_open()) {
      std::cerr << "File not found or could not be opened." << std::endl;
      return;
    }
    std::string line;
    while (std::getline(file, line)) {
      try {
        std::istringstream ss(line);
        Task task;
        std::string token;
        std::getline(ss, token, ',');
        task.id = std::stoi(token);
        std::getline(ss, task.name, ',');
        std::getline(ss, task.priority, ',');
        std::getline(ss, token, ',');
        std::istringstream ss_date(token);
        ss_date >> std::get_time(&task.due_date, "%Y-%m-%d");
        task.completed = completed;
        task.deleted = deleted;
        tasks.push_back(task);
      } catch (const std::exception &e) {
        std::cerr << "Failed to load task from line: " << line << std::endl;
        std::cerr << "Error: " << e.what() << std::endl;
        // Continue loading other tasks
      }
    }
  };

  // Load tasks from each file using the helper function
  loadTasks(ongoing_tasks_file, false, false, active_tasks);
  loadTasks(completed_tasks_file, true, false, completed_tasks);
}
