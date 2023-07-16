#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <queue>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include <limits>
#include <string>
#include <map>
#include <functional>
#include <conio.h>

#define BACKSPACE 8
#define ENTER 13
#define ARROW_UP 72
#define ARROW_DOWN 80
#define ARROW_LEFT 75
#define ARROW_RIGHT 77

using namespace std;

class Account {
public:
    string username;
    string password_hash;

    Account() {}
    
    Account(string username, string password_hash) : username(username), password_hash(password_hash) {}
};

class AccountManager {
    const string ACC_DB_FILE = "accounts.txt";
    map<string, Account> accounts;

public:
    AccountManager() {
        load_accounts();
    }

    void load_accounts() {
        ifstream acc_db(ACC_DB_FILE);
        if (!acc_db) {
            ofstream new_file(ACC_DB_FILE);
            new_file.close();
            return;
        }

        string line;
        while (getline(acc_db, line)) {
            stringstream ss(line);
            string username, password_hash;
            ss >> username >> password_hash;
            Account acc(username, password_hash);
            accounts.insert({username, acc});
        }

        acc_db.close();
    }

    bool verify_login(string username, string password) {
        auto account = accounts.find(username);
        if (account != accounts.end()) {
            string hashed_password = hash_password(password);
            return account->second.password_hash == hashed_password;
        }
        return false;
    }

    bool create_account(string username, string password) {
        if (accounts.find(username) == accounts.end()) {
            string hashed_password = hash_password(password);
            Account acc(username, hashed_password);
            accounts.insert({username, acc});
            save_accounts();
            return true;
        }
        return false;
    }

    void save_accounts() {
        ofstream acc_db(ACC_DB_FILE);
        for (const auto& account : accounts) {
            acc_db << account.second.username << " " << account.second.password_hash << endl;
        }
        acc_db.close();
    }

    string hash_password(const string& password) {
        hash<string> hasher;
        return to_string(hasher(password));
    }
};

class Task {
private:
    string title, category, dueDate;
    bool completed;

public:
    Task(string t, string c, string d) : title(t), category(c), dueDate(d), completed(false) {}

    string getTitle() const { return title; }
    string getCategory() const { return category; }
    string getDueDate() const { return dueDate; }

    bool isExpired() const {
        time_t now = time(nullptr);
        tm* localTime = localtime(&now);
        int currentYear = localTime->tm_year % 100;
        int currentMonth = localTime->tm_mon + 1;
        int currentDay = localTime->tm_mday;

        istringstream iss(dueDate);
        int month, day, year;
        char delimiter;
        if (iss >> month >> delimiter >> day >> delimiter >> year) {
            if (year < currentYear || (year == currentYear && month < currentMonth) ||
                (year == currentYear && month == currentMonth && day < currentDay)) {
                return true;
            }
        }
        return false;
    }

    bool isCompleted() const { return completed; }
    void setCompleted(bool value) { completed = value; }
};

class LinkedList {
public:
    struct Node {
        Task* task;
        Node* next;
    };

    Node* head;
    int count, pendingTaskCount;
    vector<Task*> completedTasks;

public:
    LinkedList() : head(nullptr), count(0), pendingTaskCount(0) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp->task;
            delete temp;
        }
    }

    void addTask(Task* newTask) {
        Node* newNode = new Node;
        newNode->task = newTask;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        count++;

        if (!newTask->isCompleted() && !newTask->isExpired()) {
            pendingTaskCount++; // Increment pendingTaskCount
        }
        updateTaskStatus();
    }

    void updateTaskStatus() {
        Node* current = head;
        pendingTaskCount = 0;
        completedTasks.clear();

        while (current != nullptr) {
            Task* task = current->task;
            if (!task->isCompleted() && !task->isExpired()) {
                pendingTaskCount++;
            } else if (task->isCompleted()) {
                completedTasks.push_back(task);
            }
            current = current->next;
        }
        sortByDueDate(completedTasks);
    }

    void sortByDueDate(vector<Task*>& tasks) const{
        sort(tasks.begin(), tasks.end(), [](Task* a, Task* b) {
            return a->getDueDate() < b->getDueDate();
        });
    }
    void printLine(const string& text, int maxLength) const{
        cout << "| " << text;
        int padding = maxLength - text.length();
        cout << string(padding, ' ') << " |" << endl;
    }

    void printTopBorder(const string& title, int length) const{
        int titleLength = title.length();
        int sideLength = (length - titleLength - 2) / 2; 
        string border(sideLength, '=');
        cout << border << " [" << title << "] " << border << endl;
    }

    void printBottomBorder(int length) const{
        string border(length, '=');
        cout << border << endl;
    }

    void displayTasks(const vector<Task*>& tasks) const {
        vector<Task*> sortedTasks = tasks; // Create a copy of the tasks vector
        sortByDueDate(sortedTasks); // Sort the copy by due date
        
        int maxTitleLength = 0;
        int maxCategoryLength = 0;
        int maxDueDateLength = 0;

        for (Task* task : sortedTasks) {
            maxTitleLength = max(maxTitleLength, static_cast<int>(task->getTitle().length()));
            maxDueDateLength = max(maxDueDateLength, static_cast<int>(task->getDueDate().length()));
            maxCategoryLength = max(maxCategoryLength, static_cast<int>(task->getCategory().length()));
        }
        int maxLength = max(maxTitleLength + maxDueDateLength + 6, maxCategoryLength + 2);    printTopBorder("TASKS", maxLength + 2);

        for (Task* task : sortedTasks) {
            string category = task->getCategory();
            string title = task->getTitle();
            string dueDate = task->getDueDate();
            printLine(category + ": ", maxLength);
            printLine("[" + dueDate + "] - " + title, maxLength);
        }
        printBottomBorder(maxLength + 4);
        }

    vector<Task*> getCompletedTasks() const {
        return completedTasks;
    }

    Task* removeTask(Task* taskToRemove) {
        if (head == nullptr)
            return nullptr;

        if (head->task == taskToRemove) {
            Node* temp = head;
            head = head->next;
            Task* removedTask = temp->task;
            delete temp;
            count--;
            return removedTask;
            updateTaskStatus(); // Update pendingTaskCount
        }

        Node* current = head->next;
        Node* prev = head;
        while (current != nullptr) {
            if (current->task == taskToRemove) {
                prev->next = current->next;
                Task* removedTask = current->task;
                delete current;
                count--;
                updateTaskStatus(); // Update pendingTaskCount
                return removedTask;
            }
            prev = current;
            current = current->next;
        }

        return nullptr;
    }
    int getCount() const { return count; }

    bool isEmpty() const { return count == 0; }
};

void displayMenu(int pendingTaskCount) {
    LinkedList l;
    int maxLength = 30;

    l.printTopBorder("MENU", maxLength + 2);
    l.printLine("1. Create a new list", maxLength);
    l.printLine("2. Search tasks", maxLength);
    l.printLine("3. Update tasks", maxLength);
    l.printLine("4. View tasks [" + to_string(pendingTaskCount) + "]", maxLength);
    l.printLine("5. About", maxLength);
    l.printLine("6. Check Reminders", maxLength);
    l.printLine("7. Log out", maxLength);

    l.printBottomBorder(maxLength + 4);
    cout << "Your choice: ";
}


// Function Prototype
int choice(int);
string get_hidden();
void MainTask();
int getUserChoice();
void createTask(LinkedList& tasks);
bool isValidDate(const string& date);
bool isDatePassed(const string& date);
string getCurrentDate();
vector<Task*> searchTasks(const LinkedList& tasks, int searchChoice, const string& searchTerm);
void searchTask(const LinkedList& tasks);
void updateTask(LinkedList& tasks);
void viewTasks(const LinkedList& tasks);
void about();


int main() {
    AccountManager acc_manager;
    int option;

    choice(option);

    return 0;
}

// Login persistence
int choice(int a) {
    AccountManager acc_manager;
    int choice;
    
    do {
        cout << "***************PROFILE***************" << endl;
        cout << "[1] Login" << endl;
        cout << "[2] Sign up" << endl; 
        cout << "[3] Exit" << endl; 
        cout << "**********************************" << endl;
        
        cout << "Enter option: ";
        cin >> choice;
        
        // Check if the input is valid (non-integer input)
        if (cin.fail()) {
            system("cls");
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        // Clear any remaining characters in the input buffer
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch(choice) {
            case 1: {
                system("cls");
                string username, password;

                cout << "Username: ";
                getline(cin, username);

                cout << "Password: ";
                password = get_hidden();

                if (acc_manager.verify_login(username, password)) {
                    system("cls");
                    MainTask();
                    break;
                } else {
                    system("cls");
                    cout << "Invalid username or password." << endl;
                    system("pause");
                }
                break;
            }
            case 2: {
                system("cls");
                string username, password;

                do {
                    cout << "Enter username (must be at least 6 characters long): ";
                    getline(cin, username);
                } while (username.length() < 6);

                do {
                    cout << "Enter password: ";
                    password = get_hidden();
                } while (password.length() < 8);

                if (acc_manager.create_account(username, password)) {
                    system("cls");
                    cout << "Account created successfully!" << endl;
                    system("pause");
                } else {
                    system("cls");
                    cout << "Username already exists. Please choose a different username." << endl;
                    system("pause");
                }
                break;
            }
            case 3: {
                system("cls");
                cout << "Exiting..." << endl;
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                continue;
        }
    } while (choice != 3);
    
    return choice;
}

string get_hidden() {
    string line = "";
    char key_pressed;

    while((key_pressed = getch()) != ENTER) {
        if (key_pressed == BACKSPACE) {
            if (line.length() > 0)
                cout << "\b \b";

            line = line.substr(0, line.length() - 1);
        }
        else {
            line += key_pressed;
            cout << "*";
        }
    }
    cout << endl;

    return line;
}

// ========================== TASK MANAGER ==========================
void MainTask() {
    LinkedList tasks;
    int choice, pendingTaskCount;

    do {
        displayMenu(tasks.pendingTaskCount);
        choice = getUserChoice();

        switch (choice) {
            case 1:
                system("cls");
                createTask(tasks);
                break;
            case 2:
                system("cls");
                searchTask(tasks);
                break;
            case 3:
                system("cls");
                updateTask(tasks);
                break;
            case 4:
                system("cls");
                viewTasks(tasks);
                break;
            case 5:
                system("cls");
                about();
                break;
            case 6:
                cout << "Exiting... Thank you!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // // Clear any error flags and ignore any remaining input in the buffer
                break;
        }
        cout << endl;
    } while (choice != 6);
}

int getUserChoice() {
    int choice;
    while (true) {
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid integer." << endl;
        } else if (choice < 1 || choice > 6) {
            cout << "Invalid choice. Please enter a number between 1 and 6." << endl;
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return choice;
        }
    }
}

string getCurrentDate() {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);

    int currentYear = localTime->tm_year % 100;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    string currentDate = to_string(currentMonth) + "/" + to_string(currentDay) + "/" + to_string(currentYear);
    return currentDate;
}

void createTask(LinkedList& tasks) {
    LinkedList l;
    string title, category, dueDate;

    // Prompt user until a valid title is entered (not empty)
    do {
        cout << "Enter task title: ";
        getline(cin, title);
    } while (title.empty());

    // Prompt user until a valid category is entered (not empty)
    do {
        cout << "Enter task category: ";
        getline(cin, category);
    } while (category.empty());

    string currentDate = getCurrentDate();
    cout << "Current Date: " << currentDate << endl;

    bool validDate = false;
    while (!validDate) {
        cout << "Enter due date [MM/DD/YY]: ";
        getline(cin, dueDate);

        if (!isValidDate(dueDate)) {
            cout << "Date not valid. Please enter a date in the format MM/DD/YY." << endl;
        } else if (isDatePassed(dueDate)) {
            cout << "Date not valid. The date has passed the current date." << endl;
            cout << "Current Date: " << currentDate << endl;
        } else {
            validDate = true;  // Set validDate to true
        }

        if (validDate) {  // Add the task only when validDate is true
            Task* newTask = new Task(title, category, dueDate);
            tasks.addTask(newTask);
            int maxTitleLength = 0;
            int maxCategoryLength = 0;
            int maxDueDateLength = 0;

            maxTitleLength = max(maxTitleLength, static_cast<int>(title.length()));
            maxDueDateLength = max(maxDueDateLength, static_cast<int>(dueDate.length()));
            maxCategoryLength = max(maxCategoryLength, static_cast<int>(category.length()));

            
            int maxLength = max(maxTitleLength + maxDueDateLength + 6, maxCategoryLength + 2);
            l.printBottomBorder(maxLength + 4);
            l.printLine(category + ": ", maxLength);
            l.printLine("[" + dueDate + "]  - "+ title, maxLength);
            l.printBottomBorder(maxLength + 4);
            cout << "Task added successfully!" << endl;
        }
    }
}

vector<Task*> searchTasks(const LinkedList& tasks, int searchChoice, const string& searchTerm) {
    vector<Task*> matchingTasks;
    LinkedList::Node* current = tasks.head;
    while (current != nullptr) {
        Task* task = current->task;

        switch (searchChoice) {
            case 1: // Title
                if (task->getTitle() == searchTerm)
                    matchingTasks.push_back(task);
                break;
            case 2: // Category
                if (task->getCategory() == searchTerm)
                    matchingTasks.push_back(task);
                break;
            case 3: // Date
                if (task->getDueDate() == searchTerm)
                    matchingTasks.push_back(task);
                break;
            default:
                break;
        }
        current = current->next;
    }
    return matchingTasks;
}

void searchTask(const LinkedList& tasks) {
    LinkedList l;
    int maxLength = 12;

    l.printBottomBorder(maxLength + 4);

    l.printLine("Search By:", maxLength);
    l.printLine("1. Title", maxLength);
    l.printLine("2. Category", maxLength);
    l.printLine("3. Due Date", maxLength);

    l.printBottomBorder(maxLength + 4);

    int searchChoice;
    cout << "Your choice: ";

    while (!(cin >> searchChoice) || searchChoice < 1 || searchChoice > 3) {
        // If input is not a valid integer or is out of range, show error and ask again
        cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
        // Clear any error flags and ignore any remaining input in the buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Your choice: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (searchChoice) {
        case 1: {
            system("cls");
            cout << "Title: ";
            string title;
            getline(cin, title);

            vector<Task*> matchingTasks = searchTasks(tasks, 1, title);
            if (!matchingTasks.empty()) {
                cout << "Task/s found:" << endl;
                tasks.displayTasks(matchingTasks);
            } else {
                cout << "Task/s not found." << endl;
            }
            break;
        }
        case 2: {
            system("cls");
            cout << "Category: ";
            string category;
            getline(cin, category);

            vector<Task*> matchingTasks = searchTasks(tasks, 2, category);
            if (!matchingTasks.empty()) {
                cout << "Task/s found:" << endl;
                tasks.displayTasks(matchingTasks);
            } else {
                cout << "Task/s not found." << endl;
            }
            break;
        }
        case 3: {
            system("cls");
            cout << "[MM/DD/YY] Date: ";
            string date;
            getline(cin, date);

            vector<Task*> matchingTasks = searchTasks(tasks, 3, date);
            if (!matchingTasks.empty()) {
                cout << "Task/s found:" << endl;
                tasks.displayTasks(matchingTasks);
            } else {
                cout << "Task/s not found." << endl;
            }
            break;
        }
        default:
            system("cls");
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}

void updateTask(LinkedList& tasks) {
    LinkedList l;
    if (tasks.pendingTaskCount == 0) {
        cout << "No pending tasks to update." << endl;
        return;
    }
    int searchmaxLength = 12;
    int maxLength = 25;

    l.printBottomBorder(searchmaxLength + 4);

    l.printLine("Search By:", searchmaxLength);
    l.printLine("1. Title", searchmaxLength);
    l.printLine("2. Category", searchmaxLength);
    l.printLine("3. Due Date", searchmaxLength);
    l.printLine("4. Back", searchmaxLength);

    l.printBottomBorder(searchmaxLength + 4);

    int searchChoice;
    cout << "Your choice: ";

    while (!(cin >> searchChoice) || searchChoice < 1 || searchChoice > 4) {
        // If input is not a valid integer or is out of range, show error and ask again
        cout << "Invalid choice. Please enter a number between 1 and 4." << endl;
        // Clear any error flags and ignore any remaining input in the buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Your choice: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (searchChoice) {
        case 1: {
            system("cls");
            cout << "Title: ";
            string title;
            getline(cin, title);

            vector<Task*> matchingTasks = searchTasks(tasks, 1, title);
            if (!matchingTasks.empty()) {
                cout << "Task/s found:" << endl;
                tasks.displayTasks(matchingTasks);

                l.printBottomBorder(maxLength + 4);

                l.printLine("1. Mark all completed", maxLength);
                l.printLine("2. Delete all", maxLength);
                l.printLine("3. Back", maxLength);

                l.printBottomBorder(maxLength + 4);

                int updateChoice;
                cin >> updateChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (updateChoice) {
                    case 1: {
                        system("cls");
                        for (Task* task : matchingTasks) {
                            if (!task->isCompleted()) {
                                task->setCompleted(true);
                                tasks.updateTaskStatus();
                            }
                        }
                        cout << "All matching tasks marked as completed and moved to the completed tasks list." << endl;
                        break;
                    }
                    case 2: {
                        system("cls");
                        for (Task* task : matchingTasks) {
                            tasks.removeTask(task);
                        }
                        cout << "All matching tasks deleted." << endl;
                        break;
                    }
                    case 3:
                        system("cls");
                        // Go back to the main menu
                        break;
                    default:
                        system("cls");
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
                tasks.updateTaskStatus();
            } else {
                cout << "Task/s not found." << endl;
            }
            break;
        }
        case 2: {
            system("cls");
            cout << "Category: ";
            string category;
            getline(cin, category);

            LinkedList::Node* current = tasks.head;
            bool found = false;
            while (current != nullptr) {
                Task* task = current->task;
                if (task->getCategory() == category && !task->isCompleted() && !task->isExpired()) {
                    if (!found) {
                        cout << "Task/s found:" << endl;
                        cout << task->getCategory() << ":" << endl;
                        found = true;
                    }
                    cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;
                }
                current = current->next;
            }

            if (!found) {
                cout << "Task/s not found." << endl;
            } else {
                l.printBottomBorder(maxLength + 4);

                l.printLine("1. Mark all completed", maxLength);
                l.printLine("2. Delete all", maxLength);
                l.printLine("3. Back", maxLength);

                l.printBottomBorder(maxLength + 4);

                cout << "Your choice: ";
                int updateChoice;
                cin >> updateChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (updateChoice) {
                    case 1: {
                        system("cls");
                        LinkedList::Node* current = tasks.head;
                        while (current != nullptr) {
                            Task* task = current->task;
                            if (task->getCategory() == category && !task->isCompleted()) {
                                task->setCompleted(true);
                            }
                            current = current->next;
                        }
                        cout << "All tasks under the category \"" << category << "\" marked as completed!" << endl;
                        break;
                    }
                    case 2: {
                        system("cls");
                        LinkedList::Node* current = tasks.head;
                        LinkedList::Node* prev = nullptr;
                        int deletedCount = 0;

                        while (current != nullptr) {
                            Task* task = current->task;
                            if (task->getCategory() == category) {
                                if (prev == nullptr) {
                                    tasks.head = current->next;
                                } else {
                                    prev->next = current->next;
                                }
                                delete current->task;
                                delete current;
                                current = prev != nullptr ? prev->next : tasks.head;
                                tasks.count--;
                                deletedCount++;
                            } else {
                                prev = current;
                                current = current->next;
                            }
                        }

                        cout << "Deleted " << deletedCount << " tasks under the category \"" << category << "\"!" << endl;
                        break;
                    }
                    case 3:
                        system("cls");
                        // Go back to the main menu
                        break;
                    default:
                        system("cls");
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
                tasks.updateTaskStatus();
            }
            break;
        }
        case 3: {
            system("cls");
            cout << "[MM/DD/YY] Date: ";
            string date;
            getline(cin, date);

            LinkedList::Node* current = tasks.head;
            bool found = false;
            while (current != nullptr) {
                Task* task = current->task;
                if (task->getDueDate() == date && !task->isCompleted() && !task->isExpired()) {
                    if (!found) {
                        cout << "Task/s found:" << endl;
                        found = true;
                    }
                    cout << task->getCategory() << ":" << endl;
                    cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;
                }
                current = current->next;
            }

            if (!found) {
                cout << "Task/s not found." << endl;
            } else {
                l.printBottomBorder(maxLength + 4);

                l.printLine("1. Mark all completed", maxLength);
                l.printLine("2. Delete all", maxLength);
                l.printLine("3. Back", maxLength);

                l.printBottomBorder(maxLength + 4);

                cout << "Your choice: ";
                int updateChoice;
                cin >> updateChoice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (updateChoice) {
                    case 1: {
                        system("cls");
                        LinkedList::Node* current = tasks.head;
                        while (current != nullptr) {
                            Task* task = current->task;
                            if (task->getDueDate() == date && !task->isCompleted()) {
                                task->setCompleted(true);
                                tasks.completedTasks.push_back(task);
                                tasks.removeTask(task);
                            }
                            current = current->next;
                        }
                        cout << "All tasks due on \"" << date << "\" marked as completed!" << endl;
                        break;
                    }
                    case 2: {
                        system("cls");
                        LinkedList::Node* current = tasks.head;
                        LinkedList::Node* prev = nullptr;
                        int deletedCount = 0;

                        while (current != nullptr) {
                            Task* task = current->task;
                            if (task->getDueDate() == date) {
                                if (prev == nullptr) {
                                    tasks.head = current->next;
                                } else {
                                    prev->next = current->next;
                                }
                                delete current->task;
                                delete current;
                                current = prev != nullptr ? prev->next : tasks.head;
                                tasks.count--;
                                deletedCount++;
                            } else {
                                prev = current;
                                current = current->next;
                            }
                        }

                        cout << "Deleted " << deletedCount << " tasks due on \"" << date << "\"!" << endl;
                        break;
                    }
                    case 3:
                        system("cls");
                        // Go back to the main menu
                        break;
                    default:
                        system("cls");
                        cout << "Invalid choice. Please try again." << endl;
                        break;
                }
                tasks.updateTaskStatus();
            }
            break;
        }
        default:
            system("cls");
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}

void viewTasks(const LinkedList& tasks) {
    LinkedList l;
    int maxTitleLength = 0;
    int maxCategoryLength = 0;
    int maxDueDateLength = 0;
    int maxLength = 30;
    l.printBottomBorder(maxLength + 4);

    l.printLine("1. View Pending Tasks", maxLength);
    l.printLine("2. View Completed Tasks", maxLength);
    l.printLine("3. View Expired Tasks", maxLength);

    l.printBottomBorder(maxLength + 4);

    int viewChoice;
    cout << "Your choice: ";

    while (!(cin >> viewChoice) || viewChoice < 1 || viewChoice > 3) {
        // If input is not a valid integer or is out of range, show error and ask again
        cout << "Invalid choice. Please enter a number between 1 and 3." << endl;
        // Clear any error flags and ignore any remaining input in the buffer
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Your choice: ";
    }

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    switch (viewChoice) {
        case 1: {
            system("cls");
            cout << "Pending Tasks:" << endl;
            LinkedList::Node* current = tasks.head;
            bool found = false;
            while (current != nullptr) {
                Task* task = current->task;
                if (!task->isCompleted() && !task->isExpired()) {
                    if (!found) {
                        found = true;
                    }
                    maxTitleLength = max(maxTitleLength, static_cast<int>(task->getTitle().length()));
                    maxCategoryLength = max(maxCategoryLength, static_cast<int>(task->getCategory().length()));
                    maxDueDateLength = max(maxDueDateLength, static_cast<int>(task->getDueDate().length()));
                }
                current = current->next;
            }
            int maxLength = max(maxTitleLength + maxDueDateLength + 6, maxCategoryLength + 2); // Add some padding
            l.printTopBorder("Pending Tasks", maxLength + 2);
            current = tasks.head;
            while (current != nullptr) {
                Task* task = current->task;
                if (!task->isCompleted() && !task->isExpired()) {
                    string title = task->getTitle();
                    string dueDate = task->getDueDate();
                    string category = task->getCategory();

                    l.printLine(category + ": ", maxLength);
                    l.printLine("[" + dueDate + "] - " + title, maxLength);
                }
                current = current->next;
            }
            l.printBottomBorder(maxLength + 4);

            if (!found) {
                cout << "No pending tasks." << endl;
            }
            break;
        }
        case 2: {
            system("cls");
            vector<Task*> completedTasks = tasks.getCompletedTasks();
            if (completedTasks.empty()) {
                cout << "No completed tasks." << endl;
            } else {
                tasks.displayTasks(completedTasks);
            }
            break;
        }
        case 3: {
            system("cls");
            cout << "Expired Tasks:" << endl;
            LinkedList::Node* current = tasks.head;
            vector<Task*> expiredTasks;
            bool found = false;
            while (current != nullptr) {
                Task* task = current->task;
                if (!task->isCompleted() && task->isExpired()) {
                    expiredTasks.push_back(task);
                    if (!found) {
                        found = true;
                    }
                }
                current = current->next;
            }

            if (!found) {
                cout << "No expired tasks." << endl;
            } else {

                // Calculate the maximum lengths for title and due date
                for (Task* task : expiredTasks) {
                    maxTitleLength = max(maxTitleLength, static_cast<int>(task->getTitle().length()));
                    maxCategoryLength = max(maxCategoryLength, static_cast<int>(task->getCategory().length()));
                    maxDueDateLength = max(maxDueDateLength, static_cast<int>(task->getDueDate().length()));
                }

                int maxLength = max(maxTitleLength + maxDueDateLength + 6, maxCategoryLength + 2); // Add some padding
                l.printTopBorder("EXPIRED TASKS", maxLength + 2);

                for (Task* task : expiredTasks) {
                    string category = task->getCategory();
                    string title = task->getTitle();
                    string dueDate = task->getDueDate();

                    l.printLine(category + ": ", maxLength);
                    l.printLine("[" + dueDate + "] - " + title, maxLength);
                }
                l.printBottomBorder(maxLength + 4);
            }
            break;
        }
        default:
            system("cls");
            cout << "Invalid choice. Please try again." << endl;
            break;
    }
}

void about() {
    LinkedList l;
    l.printBottomBorder(40);

    l.printLine("Search By:", 40);

    l.printBottomBorder(40);
}

// DATE AND TIME VALIDATION
bool isValidDate(const string& date) {
    istringstream iss(date);
    int month, day, year;
    char delimiter;
    return (iss >> month >> delimiter >> day >> delimiter >> year) ? true : false;
}

bool isDatePassed(const string& date) {
    time_t now = time(nullptr);
    tm* localTime = localtime(&now);
    int currentYear = localTime->tm_year % 100;
    int currentMonth = localTime->tm_mon + 1;
    int currentDay = localTime->tm_mday;

    istringstream iss(date);
    int month, day, year;
    char delimiter;
    if (iss >> month >> delimiter >> day >> delimiter >> year) {
        if (year < currentYear || (year == currentYear && month < currentMonth) ||
            (year == currentYear && month == currentMonth && day < currentDay)) {
            return true;
        }
    }
    return false;
}