#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <conio.h>
#include <iomanip>
#include <limits>
#include <queue>
#include <sstream>

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
        string pw_hash;

        string first_name;
        string last_name;
        time_t birthdate;

        string contact_number;
        string email_address;

};


class AccountManager {
    const string ACC_DB_FILE = "ACCOUNT_DB";

    hash<string> hasher;
    map<string, Account> accounts;

    public:
        AccountManager() {
            load_accounts();
        }

        ~AccountManager() {
            save_accounts();
        }

        void add_account(Account acc) {
            accounts.insert( { acc.username, acc } );
        }

        void remove_account(Account acc) {
            accounts.erase(acc.username);
        }

        Account retrieve_account(string username) {
            return accounts[username];
        }

        Account create_account(
            string username, string password,
            string first_name, string last_name,
            string contact_number, string email_address
        ) {
            string pw_hash = hash_password(password);
            Account acc;

            acc.username = username;
            acc.pw_hash = pw_hash;

            acc.first_name = first_name;
            acc.last_name = last_name;
            acc.contact_number = contact_number;
            acc.email_address = email_address;

            add_account(acc);
            return acc;
        }

        void load_accounts() {
            ifstream acc_db;
            acc_db.open(ACC_DB_FILE);

            while (acc_db.peek() != EOF) {
                Account acc;

                string temp;
                getline(acc_db, temp);

                getline(acc_db, acc.username);
                getline(acc_db, acc.pw_hash);

                getline(acc_db, acc.first_name);
                getline(acc_db, acc.last_name);
                getline(acc_db, acc.contact_number);
                getline(acc_db, acc.email_address);

                add_account(acc);
            }

            acc_db.close();
        }

        void save_accounts() {
            ofstream acc_db;
            acc_db.open(ACC_DB_FILE);

            for (auto account : accounts) {
                Account acc = account.second;

                acc_db << acc.username << endl;
                acc_db << acc.pw_hash << endl;

                acc_db << acc.first_name << endl;
                acc_db << acc.last_name << endl;
                acc_db << acc.contact_number << endl;
                acc_db << acc.email_address << endl;
            }

            acc_db.close();
        }

        bool verify_login(string username, string password) {
            return accounts[username].pw_hash == hash_password(password);
        }

        static string hash_password(string password) {
            hash<string> hasher;
            return to_string(hasher(password));
        }
};

class Task {
private:
    string title, category, dueDate;
    bool completed;

public:
    Task(string t, string c, string d) : title(t), category(c), dueDate(d) {}

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
};

class LinkedList {
public:
    struct Node {
        Task* task;
        Node* next;
    };

    Node* head;
    int count, pendingTaskCount;

public:
    LinkedList() : head(nullptr), count(0), pendingTaskCount(0) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp->task;
            delete temp;
            count--;
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
        updateTaskStatus();
    }

    void updateTaskStatus() {
        Node* current = head;
        int pendingTaskCount = 0;

        while (current != nullptr) {
            Task* task = current->task;
            if (!task->isCompleted() && !task->isExpired()) {
                pendingTaskCount++;
            }
            current = current->next;
        }

        this->pendingTaskCount = pendingTaskCount;
    }

    void displayTasks() const {
        Node* current = head;
        while (current != nullptr) {
            Task* task = current->task;
            cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;
            current = current->next;
        }
    }

    int getCount() const { return count; }

    bool isEmpty() const { return count == 0; }
};

class TaskManager {
public:
    void TaskHeader(int pendingTaskCount) {
        system("cls");
        cout << "===================================" << endl;
        cout << "\t   TASK MENU" << endl;
        cout << "===================================" << endl;
        cout << "[1] Create Tasks" << endl;
        cout << "[2] Search Task" << endl;
        cout << "[3] Update Task" << endl;
        cout << "[4] View Tasks [" << pendingTaskCount << "]" << endl;
        cout << "[5] About" << endl;
        cout << "[6] Exit" << endl;
    }

    int getUserChoice() {
        int choice;
        cin >> choice;
        return choice;
    }

    void MainTasks() {
        LinkedList tasks;
        int pendingTaskCount;

        while(true) {
            TaskHeader(tasks.pendingTaskCount);
            char choice = getch();

            switch(choice) {
                case '1': // Create tasks
                    CreateTask(tasks);
                    break;
                case '2': // Search task
                    searchTask(tasks);
                    break;
                case '3': // Update task
                    break;
                case '4': // View tasks
                    break;
                case '5': // About 
                    break;
                case '6':
                    cout << "Exiting program..." << endl;
                    break;
                default:
                    cout << "Invalid choice. Please try again" << endl;
            }
        }
    }


    // Function for create tasks
    void CreateTask(LinkedList& tasks) {
        system("cls");
        cout << "Enter Task: ";
        string title;
        getline(cin, title);

        cout << "Category: "; // Note - What categories?
        string category;
        getline(cin, category);

        string dueDate;
        bool validDate = false;
        while (!validDate) {
            cout << "[MM/DD/YY] Due date: ";
            getline(cin, dueDate);

            if (!isValidDate(dueDate)) {
                cout << "Date not valid. Please enter a date in the format MM/DD/YY." << endl;
            } else if (isDatePassed(dueDate)) {
                cout << "Date not valid. The date has passed the current date." << endl;
            } else {
                Task* newTask = new Task(title, category, dueDate);
                tasks.addTask(newTask);
                cout << "Task added successfully!" << endl;
                validDate = true;
            }
        }
    }

    // Function for search task
    void searchTask(const LinkedList& tasks) {
        system("cls");
        cout << "*************** Search By ***************" << endl;
        cout << "[1] Title" << endl;
        cout << "[2] Category" << endl;
        cout << "[3] Date" << endl;
        cout << "Your choice: ";
        int searchChoice;
        cin >> searchChoice;

        switch (searchChoice) {
            case 1: {
                system("cls");
                cout << "Title: ";
                string title;
                getline(cin, title);

                LinkedList::Node* current = tasks.head;
                bool found = false;
                while (current != nullptr) {
                    Task* task = current->task;
                    if (task->getTitle() == title) {
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
                    if (task->getCategory() == category) {
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
                    if (task->getDueDate() == date) {
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
                }
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    // Function for updating a task
    void updateTask(LinkedList& tasks) {
        system("cls");
        cout << "*************** Update By ***************" << endl;
        cout << "[1] Title" << endl;
        cout << "[2] Category" << endl;
        cout << "[3] Date" << endl;
        cout << "[4] Back" << endl;
        cout << "Your choice: ";
        int searchChoice;
        int pendingTaskCount = 0;
        cin >> searchChoice;
        // cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (searchChoice) {
            case 1: {
                system("cls");
                cout << "Title: ";
                string title;
                getline(cin, title);

                LinkedList::Node* current = tasks.head;
                bool found = false;

                while (current != nullptr) {
                    Task* task = current->task;
                    if (task->getTitle() == title) {
                        if (!found) {
                            cout << "Task/s found:" << endl;
                            found = true;
                        }
                        cout << task->getCategory() << ":" << endl;
                        cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;

                        cout << "1. Mark all completed" << endl;
                        cout << "2. Delete all" << endl;
                        cout << "3. Back" << endl;
                        cout << "Your choice: ";
                        int updateChoice;
                        cin >> updateChoice;
                    

                        switch (updateChoice) {
                            case 1: {
                                system("cls");
                                cout << "Marking all tasks as completed..." << endl;
                                LinkedList::Node* current = tasks.head;
                                while (current != nullptr) {
                                    Task* task = current->task;
                                    // Mark the task as completed
                                    // ...
                                    current = current->next;
                                }
                                cout << "All tasks marked as completed!" << endl;
                                break;
                            }
                            case 2: {
                                system("cls");
                                cout << "This action cannot be undone. Do you want to continue? [Y/N] ";
                                char confirm;
                                cin >> confirm;
                                
                                if (confirm == 'Y' || confirm == 'y') {
                                    cout << "Deleting all tasks..." << endl;
                                    LinkedList::Node* current = tasks.head;
                                    while (current != nullptr) {
                                        LinkedList::Node* temp = current;
                                        current = current->next;
                                        Task* task = temp->task;
                                        // Delete the task
                                        // ...
                                        delete task;
                                        delete temp;
                                    }
                                    tasks.head = nullptr;
                                    tasks.count = 0;
                                    cout << "All tasks deleted!" << endl;
                                }
                                break;
                            }
                            case 3:
                                system("cls");
                                // Go back to the main menu
                                break;
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                                break;
                        }
                        tasks.updateTaskStatus();
                    }
                    current = current->next;
                }

                if (!found) {
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
                    if (task->getCategory() == category) {
                        if (!found) {
                            cout << "Task/s found:" << endl;
                            found = true;
                        }
                        cout << task->getCategory() << ":" << endl;
                        cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;

                        cout << "1. Mark all completed" << endl;
                        cout << "2. Delete all" << endl;
                        cout << "3. Back" << endl;
                        cout << "Your choice: ";
                        int updateChoice;
                        cin >> updateChoice;
                        // cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        switch (updateChoice) {
                            case 1:
                                system("cls");
                                // Mark all completed code here
                                // ...
                                break;
                            case 2: {
                                system("cls");
                                cout << "This action cannot be undone. Do you want to continue? [Y/N] ";
                                char confirm;
                                cin >> confirm;
                                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                if (confirm == 'Y' || confirm == 'y') {
                                    // Delete all code here
                                    // ...
                                }
                                break;
                            }
                            case 3:
                                system("cls");
                                // Go back to main menu
                                break;
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                                break;
                        }
                    }
                    current = current->next;
                }

                if (!found) {
                    cout << "Task/s not found." << endl;
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
                    if (task->getDueDate() == date) {
                        if (!found) {
                            cout << "Task/s found:" << endl;
                            found = true;
                        }
                        cout << task->getCategory() << ":" << endl;
                        cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;

                        cout << "1. Mark all completed" << endl;
                        cout << "2. Delete all" << endl;
                        cout << "3. Back" << endl;
                        cout << "Your choice: ";
                        int updateChoice;
                        cin >> updateChoice;
                        // cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        switch (updateChoice) {
                            case 1:
                            system("cls");
                                // Mark all completed code here
                                // ...
                                break;
                            case 2: {
                                system("cls");
                                cout << "This action cannot be undone. Do you want to continue? [Y/N] ";
                                char confirm;
                                cin >> confirm;
                                // cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                if (confirm == 'Y' || confirm == 'y') {
                                    // Delete all code here
                                    // ...
                                }
                                break;
                            }
                            case 3:
                                system("cls");
                                // Go back to main menu
                                break;
                            default:
                                cout << "Invalid choice. Please try again." << endl;
                                break;
                        }
                    }
                    current = current->next;
                }

                if (!found) {
                    cout << "Task/s not found." << endl;
                }
                break;
            }
            case 4:
                system("cls");
                // Go back to main menu
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    // Function for viewing the tasks
    void viewTasks(const LinkedList& tasks) {
        system("cls");

        LinkedList::Node* current = tasks.head;
        bool foundPendingTasks = false;
        bool foundCompletedTasks = false;
        bool foundExpiredTasks = false;
        int pendingTaskCount = 0;

        cout << "[1] View Pending Tasks" << endl;
        cout << "[2] View Completed Tasks" << endl;
        cout << "[3] View Expired Tasks" << endl;
        cout << "Your choice: ";
        int viewChoice;
        cin >> viewChoice;

        switch (viewChoice) {
            case 1: {
                system("cls");
                cout << "Pending Tasks:" << endl;
                LinkedList::Node* current = tasks.head;
                bool foundPendingTasks = false;
                while (current != nullptr) {
                    Task* task = current->task;
                    // Check if the task is pending
                    // ...
                    if (!task->isCompleted() && !task->isExpired()) {
                        cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;
                        foundPendingTasks = true;
                        pendingTaskCount++;
                    }
                    current = current->next;
                }
                if (!foundPendingTasks) {
                    cout << "No pending tasks found." << endl;
                }
                break;
            }
            case 2: {
                system("cls");
                cout << "Completed Tasks:" << endl;
                LinkedList::Node* current = tasks.head;
                bool foundCompletedTasks = false;
                while (current != nullptr) {
                    Task* task = current->task;
                    // Check if the task is completed
                    // ...
                    if (task->isCompleted()) {
                        cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;
                        foundCompletedTasks = true;
                    }
                    current = current->next;
                }
                if (!foundCompletedTasks) {
                    cout << "No completed tasks found." << endl;
                }
                break;
            }
            case 3:
                system("cls");
                cout << "Expired Tasks:" << endl;
                // Display expired tasks code here
                while (current != nullptr) {
                    Task* task = current->task;
                    // Check if the task is expired
                    // ...
                    if (task->isExpired()) {
                        cout << "[" << task->getDueDate() << "] - " << task->getTitle() << endl;
                        foundExpiredTasks = true;
                    }
                    current = current->next;
                }
                if (!foundExpiredTasks) {
                    cout << "No expired tasks found." << endl;
                }
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    }

    // Function for about
    void about() {
        cout << "(information and instructions of this program)" << endl;
    }


    // Date Declaration
    bool isValidDate(const string& date) {
        istringstream iss(date);
        int month, day, year;
        char delimiter;
        if (iss >> month >> delimiter >> day >> delimiter >> year) {
            if (month >= 1 && month <= 12 && day >= 1 && day <= 31 && year >= 0 && year <= 99) {
                return true;
            }
        }
        return false;
    }

    bool isDatePassed(const string& date) {
        istringstream iss(date);
        int month, day, year;
        char delimiter;
        if (iss >> month >> delimiter >> day >> delimiter >> year) {
            // Get the current date
            time_t now = time(nullptr);
            tm* localTime = localtime(&now);
            int currentYear = localTime->tm_year % 100;
            int currentMonth = localTime->tm_mon + 1;
            int currentDay = localTime->tm_mday;

            if (year < currentYear || (year == currentYear && month < currentMonth) ||
                (year == currentYear && month == currentMonth && day < currentDay)) {
                return true;
            }
        }
        return false;
    }
};


class State {
    public:
        AccountManager acc_manager;
        TaskManager task_manager;

        void homepage() {
            system("cls");

            cout << "***************PROFILE***************" << endl;
            cout << "[1] Sign up" << endl;
            cout << "[2] Login" << endl; 
            cout << "[3] Exit" << endl; 
            cout << "**********************************" << endl;

            while(true) {
                char answer = getch();

                switch(answer) {
                    case '1':
                        sign_up();
                        break;
                    case '2':
                        login();
                        break;
                    default:
                        continue;
                }
            }
        }

        void login() {
            system("cls");

            cout << "Account Login\n";
            cout << "---------------------------------\n";
            
            cout << "Username: ";
            string username;
            getline(cin, username);

            cout << "Password: ";

            string password = get_line_as_hidden();
            
            while(true) {
                if (acc_manager.verify_login(username, password)) {
                Account acc = acc_manager.retrieve_account(username);
                system("cls");
                task_manager.MainTasks();
                } else {
                    cout << "\nUsername or password is incorrect.\n\n";
                    system("pause");
                    homepage();
                    break;
                }
            }
        }

        string get_line_as_hidden() {
            string line = "";
            char key_pressed;

            while ((key_pressed = getch()) != ENTER) {
                if (key_pressed == BACKSPACE) {
                    if (line.length() > 0)
                        cout << "\b \b";

                    line = line.substr(0, line.length() - 1);
                } else {
                    line += key_pressed;
                    cout << '*';
                }
            }

            cout << endl;
            
            return line;
        }

        void sign_up() {
            system("cls");

            cout << "Account Creation\n";
            cout << "---------------------------------\n";

            cout << "First Name: ";
            string first_name;
            getline(cin, first_name);

            cout << "Last Name: ";
            string last_name;
            getline(cin, last_name); 

            cout << "Contact No.: ";
            string contact_num;
            getline(cin, contact_num);

            cout << "Email Address: ";
            string email_address;
            getline(cin, email_address);


            cout << "Username: ";
            string username;
            getline(cin, username);

            string password, confirm_password;

            while (true) {    
                cout << "Password: ";
                password = get_line_as_hidden();

                cout << "Confirm Password: ";
                confirm_password = get_line_as_hidden();

                if (password == confirm_password)
                    break;
                
                cout << "\nPasswords do not match. Please re-input your password.\n\n";
                system("pause");
                system("cls");

                cout << "Account Creation\n";
                cout << "---------------------------------\n";
                cout << "First Name: " << first_name << endl;
                cout << "Last Name: " << last_name << endl;
                cout << "Contact No.: " << contact_num << endl;
                cout << "Email Address: " << email_address << endl;

                cout << "Username: " << username << endl;
            }

            acc_manager.create_account(
                username,
                password, 
                first_name, 
                last_name,
                contact_num,
                email_address
            );

            cout << "\nAccount created successfully.\n\n";
            system("pause");

            homepage();
        }   

};


int main() {
    State state;

    state.homepage();   

    return 0;
}