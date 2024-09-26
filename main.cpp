#include "sort.h"
#include "task.h"
#include <iomanip>
#include <iostream>
#include <sstream>


int main() {
  TodoList todoList;
  todoList.loadFromFile(); // Load tasks from files

  while (true) {
    std::cout << "\nTodo List Menu:\n";
    std::cout << "1. Add Task\n";
    std::cout << "2. View Ongoing Tasks\n";
    std::cout << "3. View Completed Tasks\n";
    std::cout << "4. View Deleted Tasks\n";
    std::cout << "5. Complete Task\n";
    std::cout << "6. Delete Task\n";
    std::cout << "7. Exit\n";
    std::cout << "Enter your choice: ";

    int choice;
    std::string inputLine;
    std::getline(std::cin, inputLine);
    try {
      choice = std::stoi(inputLine);
    } catch (const std::invalid_argument ) {
      std::cout << "Invalid input. Please enter a numeric value for choice.\n";
      continue;
    }

    if (choice == 1) {
      // Add Task
      Task newTask;
      std::cout << "Enter Task ID: ";
      std::getline(std::cin, inputLine);
      newTask.id = std::stoi(inputLine);

      std::cout << "Enter Task Name: ";
      std::getline(std::cin, newTask.name);

      std::cout << "Enter Task Priority (high, medium, low): ";
      std::getline(std::cin, newTask.priority);

      std::cout << "Enter Task Due Date (format: YYYY-MM-DD): ";
      std::getline(std::cin, inputLine);
      std::istringstream ss(inputLine);
      ss >> std::get_time(&newTask.due_date, "%Y-%m-%d");

      newTask.completed = false;
      newTask.deleted = false;
      todoList.addTask(newTask);
      std::cout << "Task added successfully.\n";
    } else if (choice == 2) {
      // View Ongoing Tasks
      todoList.viewTasks();
    } else if (choice == 3) {
      // View Completed Tasks
      todoList.viewCompletedTasks();
    } else if (choice == 4) {
      // View Deleted Tasks
      todoList.viewDeletedTasks();
    } else if (choice == 5) {
      // Complete Task
      todoList.completeTask();
    } else if (choice == 6) {
      // Delete Task
      todoList.deleteTask();
    } else if (choice == 7) {
      // Exit
      todoList.saveToFile(); // Save tasks to files
      break;
    } else {
      std::cout << "Invalid choice. Please enter a valid option.\n";
    }
  }

  return 0;
}
