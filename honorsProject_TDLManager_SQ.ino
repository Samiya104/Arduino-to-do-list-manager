// To-Do list Manager Project
// Hardware used: Arduino Uno, Elegoo 2.8" TFT Display
// Input Mode: Serial Monitor
// Output Mode: Elegoo 2.8" TFT Display

// include the necessary libraries
#include <Elegoo_TFTLCD.h>

// Define colors for the TFT screen
#define WHITE 0xFFFF
#define BLACK 0x0000
#define YELLOW 0xFFFE0
#define LAVENDER 0xEDFF

// Define the maximum number of tasks
const int MAX_TASKS = 10;

// Task structure
struct Task {
  String description;
  String dueDate;
  int priority;
  bool completed;
};

// Array to store tasks
Task tasks[MAX_TASKS];

// Counter for the number of tasks
int taskCount = 0;

// TFT display setup
Elegoo_TFTLCD tft(A3, A2, A1, A0, A4);

void setup() {
  tft.begin(0x9341);
  tft.setRotation(3);
  tft.fillScreen(LAVENDER);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);

  Serial.begin(9600); // Start serial communication

  // Show the menu
  showMenu();
}

void loop() {

  String inputOption;
  do {
    // Wait for user input
    while (Serial.available() == 0) {}

    // Read user input
    inputOption = Serial.readStringUntil('\n');
    inputOption.trim(); // Remove any leading/trailing whitespace

  } while (inputOption.length() == 0); // Ignore empty input lines
    int option = inputOption.toInt();

  // Process the input option
  if (option == 1) {
    // Create Task
    delay(1000);
    createTask();
  } 
  else if (option == 2) {
    // View Tasks
    delay(1000);
    viewTasks();
  } 
  else if (option == 3) {
    // Edit Task
    delay(1000);
    editTask();
  } 
  else if (option == 4) {
    // Delete Task
    delay(1000);
    deleteTask();
  } 
  else if (option == 5) {
    // Exit
    tft.fillScreen(LAVENDER);
    tft.setTextColor(BLACK);
    tft.setCursor(10,10);
    tft.println("Thank you! Bye.\n\tYou can now turn off this device");
    delay(1000);
    return;
  } 
  else {
    // Invalid option
      tft.fillScreen(LAVENDER);
      tft.setTextColor(BLACK);
      tft.setCursor(10,10);
      tft.println("Invalid option. Please try again.");
      delay(1000); // Wait for 1 second
  }
  showMenu();
  // Clear the Serial buffer
  while (Serial.available() > 0) {
    Serial.read();
  }
}

void showMenu() {
  // Clear the TFT screen
  tft.fillScreen(BLACK);
  tft.setTextSize(3);

  // Set the cursor position and text color for the menu title
  tft.fillRect(0,0,375,35, LAVENDER);
  tft.setTextColor(BLACK);
  tft.setCursor(0,10);
  tft.println("Todo List Menu");
  //set text color for menu options and display them
  tft.setTextColor(WHITE);
  tft.setCursor(0, 50);
  tft.setTextSize(2);
  tft.println("1. Create Task");
  tft.println("2. View Tasks");
  tft.println("3. Edit Task");
  tft.println("4. Delete Task");
  tft.println("5. Exit");
  tft.println("Enter your choice:");
}

//initiate the function to create tasks
void createTask() {
  // clear the screen
  tft.fillScreen(LAVENDER);
  tft.setTextColor(BLACK);
  // Create a new task and add it to the tasks array
  if (taskCount < MAX_TASKS) {
    Task newTask;
    tft.fillScreen(WHITE);
    tft.setTextColor(BLACK);
    tft.setCursor(20, 20);
    tft.println("Enter task description:");
    while (Serial.available() == 0) {}
    newTask.description = Serial.readString();
    tft.println(newTask.description); // Display user input on TFT

    tft.println("Enter task due date(YYYY-MM-DD):");
    while (Serial.available() == 0) {}
    newTask.dueDate = Serial.readString();
    tft.println(newTask.dueDate); // Display user input on TFT

    tft.println("Enter task priority:");
    while (Serial.available() == 0) {}
    newTask.priority = Serial.parseInt();
    tft.println(newTask.priority); // Display user input on TFT

    newTask.completed = false;
    tasks[taskCount++] = newTask;
  }
}

// Function to sort tasks by priority
int comparePriority(const void* a, const void* b) {
  const Task* taskA = reinterpret_cast<const Task*>(a);
  const Task* taskB = reinterpret_cast<const Task*>(b);
  return taskA->priority - taskB->priority;
}

// Function to sort tasks by due date (lexicographically)
int compareDueDate(const void* a, const void* b) {
  const Task* taskA = reinterpret_cast<const Task*>(a);
  const Task* taskB = reinterpret_cast<const Task*>(b);
  return taskA->dueDate.compareTo(taskB->dueDate);
}

//initiate the function to view tasks
void viewTasks() {
  //clear the screen
  tft.fillScreen(LAVENDER);
  tft.setTextColor(BLACK);
  tft.fillScreen(WHITE);
  tft.setCursor(0, 20);
  tft.setTextSize(2);

  tft.println("View tasks by:");
  tft.println("1. Priority");
  tft.println("2. Due Date");
  tft.println("Enter your choice (1 or 2):");

  // Wait for user input
  while (Serial.available() == 0) {}

  // Read and parse user choice
  int choice = Serial.parseInt();
  while (Serial.available() > 0) {
    Serial.read(); // Clear Serial buffer
  }

  // Sort tasks based on user's choice
  if (choice == 1) {
      tft.fillScreen(LAVENDER);
      tft.setTextColor(BLACK);
    qsort(tasks, taskCount, sizeof(Task), comparePriority);
  } else if (choice == 2) {
      tft.fillScreen(LAVENDER);
      tft.setTextColor(BLACK);
    qsort(tasks, taskCount, sizeof(Task), compareDueDate);
  } else {
      tft.fillScreen(LAVENDER);
      tft.setTextColor(BLACK);
    tft.println("Invalid choice. Press any key to return to menu.");
    while (Serial.available() == 0) {} // Wait for user input
    while (Serial.available() > 0) {
      Serial.read(); // Clear Serial buffer
    }
    return;
  }

  // Display sorted tasks
  tft.setCursor(0,5);
  tft.println("Tasks:");
  int yOffset = 25;
  for (int i = 0; i < taskCount; ++i) {
    Task task = tasks[i];
    tft.setCursor(20, yOffset);
    tft.print("Description: ");
    tft.println(task.description);
    tft.setCursor(20, yOffset + 20);
    tft.print("Due Date: ");
    tft.println(task.dueDate);
    tft.setCursor(20, yOffset + 40);
    tft.print("Priority: ");
    tft.println(task.priority);
    yOffset += 25; // Update Y-offset for next task
  }

  // Wait for user input to return to menu
  tft.println("Press any key to return to menu.");
  while (Serial.available() == 0) {} // Wait for user input
  while (Serial.available() > 0) {
    Serial.read(); // Clear Serial buffer
  }
}


void editTask() {
  tft.fillScreen(LAVENDER);
  tft.setTextColor(BLACK);
  // Display all tasks on the TFT screen
  int yOffset = 20;
  for (int i = 0; i < taskCount; ++i) {
    Task task = tasks[i];
    tft.setCursor(20, yOffset);
    tft.print(i + 1); // Display task index starting from 1
    tft.print(". ");
    tft.println(task.description);
    yOffset += 20; // Update Y-offset for next task
  }

  // Allow editing tasks
  tft.println("Enter task index to edit or -1 to return to menu:");
  while (Serial.available() == 0) {}
  int taskIndex = Serial.parseInt() - 1; // Convert to zero-based index
  while (Serial.available() > 0) {
    Serial.read(); // Clear Serial buffer
  }

  if (taskIndex == -2) {
    return; // Return to menu
  } else if (taskIndex >= 0 && taskIndex < taskCount) {
    // Edit the task
    tft.println("Enter new description:");
    while (Serial.available() == 0) {} // Wait for user input
    String newDescription = Serial.readStringUntil('\n');
    newDescription.trim(); // Remove any leading/trailing whitespace
    while (Serial.available() > 0) {
      Serial.read(); // Clear Serial buffer
    }

    tft.println("Enter new due date:");
    while (Serial.available() == 0) {} // Wait for user input
    String newDueDate = Serial.readStringUntil('\n');
    newDueDate.trim(); // Remove any leading/trailing whitespace
    while (Serial.available() > 0) {
      Serial.read(); // Clear Serial buffer
    }

    // Update the task
    tasks[taskIndex].description = newDescription;
    tasks[taskIndex].dueDate = newDueDate;
    tft.println("Task updated.");
  } 
  else {
    tft.println("Invalid task index.");
  }
  tft.println("Press any key to return to menu.");
  while (Serial.available() == 0) {} // Wait for user input
  while (Serial.available() > 0) {
    Serial.read(); // Clear Serial buffer
  }
}

void deleteTask() {
  tft.fillScreen(LAVENDER);
  tft.setCursor(20, 20);
  tft.setTextColor(BLACK);
  tft.println("Enter task index to delete:");
  while (Serial.available() == 0) {}
  int taskIndex = Serial.parseInt() - 1; // Convert to zero-based index

  if (taskIndex >= 0 && taskIndex < taskCount) {
    // Shift all tasks to the left to fill the gap left by the deleted task
    for (int i = taskIndex; i < taskCount - 1; ++i) {
      tasks[i] = tasks[i + 1];
    }
    taskCount--; // Decrement task count
    tft.println("Task deleted successfully.");
  }
  else {
    tft.println("Invalid task index.");
  }
  tft.println("Press any key to return to menu.");
  while (Serial.available() == 0) {} // Wait for user input
  while (Serial.available() > 0) {
    Serial.read(); // Clear Serial buffer
  }
}

//end