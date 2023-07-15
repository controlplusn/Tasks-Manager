#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

class Account {
public:
    string username;
    string password;

    Account() {}
    
    Account(string username, string password) : username(username), password(password) {}
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
            string username, password;
            ss >> username >> password;
            Account acc(username, password);
            accounts.insert({username, acc});
        }

        acc_db.close();
    }

    bool verify_login(string username, string password) {
        auto account = accounts.find(username);
        if (account != accounts.end()) {
            return account->second.password == password;
        }
        return false;
    }

    bool create_account(string username, string password) {
        if (accounts.find(username) == accounts.end()) {
            Account acc(username, password);
            accounts.insert({username, acc});
            save_accounts();
            return true;
        }
        return false;
    }

    void save_accounts() {
        ofstream acc_db(ACC_DB_FILE);
        for (const auto& account : accounts) {
            acc_db << account.second.username << " " << account.second.password << endl;
        }
        acc_db.close();
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
    void TaskHeader() {
        system("cls");
        cout << "===================================" << endl;
        cout << "\t   TASK MENU" << endl;
        cout << "===================================" << endl;
        cout << "[1] Create Tasks" << endl;
        cout << "[2] Search Task" << endl;
        cout << "[3] Update Task" << endl;
        cout << "[4] View Tasks" << endl;
        cout << "[5] About" << endl;
        cout << "[6] Exit" << endl;
    }
};

// Function Prototype
int choice(int);

int main() {
    AccountManager acc_manager;
    int option;

    choice(option);

    return 0;
}

// Login persistence
int choice(int a) {
    AccountManager acc_manager;
    TaskManager task_manager;
    int choice;
    
    do {
        cout << "***************PROFILE***************" << endl;
        cout << "[1] Login" << endl;
        cout << "[2] Sign up" << endl; 
        cout << "[3] Exit" << endl; 
        cout << "**********************************" << endl;
        
        cout << "Enter option: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1: {
                system("cls");
                string username, password;

                cout << "Username: ";
                getline(cin, username);

                cout << "Password: ";
                getline(cin, password);

                if (acc_manager.verify_login(username, password)) {
                    // cout << "Login Successful!" << endl;
                    task_manager.TaskHeader();
                    break;
                } else {
                    cout << "Invalid username or password." << endl;
                }
                break;
            }
            case 2: {
                system("cls");
                string username, password;

                cout << "Enter a new username: ";
                getline(cin, username);

                cout << "Enter a new password: ";
                getline(cin, password);

                if (acc_manager.create_account(username, password)) {
                    cout << "Account created successfully!" << endl;
                } else {
                    cout << "Username already exists. Please choose a different username." << endl;
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
        }
    } while (choice != 3);
    
    return choice;
}
