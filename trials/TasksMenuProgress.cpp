#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <limits>
#include <queue>

using namespace std;

class TaskManager {
private:
    struct Node {
        string title;
        string description;
        string date;
        Node* next;
    };

    Node* head;

public:
    TaskManager() {
        head = nullptr;
    }

    void printLine(const string& text, int maxLength) {
        cout << "| " << text;
        int padding = maxLength - text.length();
        cout << string(padding, ' ') << " |";
    }

    void printTopBorder(const string& title, int length) {
        int titleLength = title.length();
        int sideLength = (length - titleLength - 2) / 2; 
        string border(sideLength, '=');
        cout << border << " [" << title << "] " << border << endl;
    }

    void printBottomBorder(int length) {
        string border(length, '=');
        cout << border << endl;
    }

    void printMenu() {
        queue<string> menuItems;
        int maxLength = 40;

        printTopBorder("MENU", maxLength + 2);

        menuItems.push("1. Create a new list");
        menuItems.push("2. Create tasks");
        menuItems.push("3. Display all tasks");
        menuItems.push("4. Search task");
        menuItems.push("5. Delete task");
        menuItems.push("6. Exit");

        while (!menuItems.empty()) {
            string item = menuItems.front();
            printLine(item, maxLength);
            cout << endl;
            menuItems.pop();
        }

        printBottomBorder(maxLength + 4);
    }

    void createList() {
        // wala pa
    }

    void createTask(string title, string date, string description) {
        Node* new_node = new Node;
        new_node->title = title;
        new_node->date = date;
        new_node->description = description;
        new_node->next = nullptr;

        if (head == nullptr) {
            head = new_node;
        } else {
            Node* temp = head;
            Node* prev = nullptr;

            while (temp != nullptr && temp->date < date) {
                prev = temp;
                temp = temp->next;
            }

            if (prev == nullptr) {
                new_node->next = head;
                head = new_node;
            } else {
                prev->next = new_node;
                new_node->next = temp;
            }
        }
    }

    void deleteTask(string title) {
        if (head == nullptr) {
            cout << "The list is empty." << endl;
            return;
        }

        Node* temp = head;
        Node* prev = nullptr;

        // Check if the first node matches the title
        if (temp != nullptr && temp->title == title) {
            head = temp->next;
            delete temp;
            cout << "Task deleted successfully!" << endl;
            return;
        }

        // Search for the task by title
        while (temp != nullptr && temp->title != title) {
            prev = temp;
            temp = temp->next;
        }

        // If task not found
        if (temp == nullptr) {
            cout << "Task not found!" << endl;
            return;
        }

        // Delete the task
        prev->next = temp->next;
        delete temp;
        cout << "Task deleted successfully!" << endl;
    }

    void displayAllTasks() {
        if (head == nullptr) {
            cout << "The list is empty." << endl;
            return;
        }
        cout << "Here are your tasks: " << endl;

        int maxTitleLength = 0;
        int maxDateLength = 0;
        int maxDescriptionLength = 0;

        Node* temp = head;
        while (temp != nullptr) {
            maxTitleLength = max(maxTitleLength, static_cast<int>(temp->title.length()));
            maxDateLength = max(maxDateLength, static_cast<int>(temp->date.length()));
            maxDescriptionLength = max(maxDescriptionLength, static_cast<int>(temp->description.length()));
            temp = temp->next;
        }

        int maxLength = max(maxTitleLength + maxDateLength + 4, maxDescriptionLength + 11);

        printTopBorder("TASKS", maxLength + 2);
        temp = head;
        while (temp != nullptr) {
            printLine("[" + temp->date + "] " + temp->title, maxLength);
            cout << endl;
            printLine("---------  " + temp->description, maxLength);
            cout << endl;
            temp = temp->next;
        }
        printBottomBorder(maxLength + 4);
    }

    bool isValidDate(const string& date) {
        if (date.length() != 8 || date[2] != '/' || date[5] != '/') {
            return false;
        }

        string monthStr = date.substr(0, 2);
        string dayStr = date.substr(3, 2);
        string yearStr = date.substr(6, 2);

        int month = stoi(monthStr);
        int day = stoi(dayStr);
        int year = stoi(yearStr);

        if (month >= 1 && month <= 12 && day >= 1 && day <= 31 && year >= 0 && year <= 99) {
            return true;
        }

        return false;
    }

    string getCurrentDate() {
        time_t now = time(nullptr);
        struct tm* timeinfo = localtime(&now);

        stringstream ss;
        ss << setw(2) << setfill('0') << (timeinfo->tm_mon + 1) << "/";
        ss << setw(2) << setfill('0') << timeinfo->tm_mday << "/";
        ss << setw(2) << setfill('0') << (timeinfo->tm_year % 100);
        return ss.str();
    }

    void searchTask(string title) {
        if (head == nullptr) {
            cout << "The list is empty." << endl;
            return;
        }

        Node* temp = head;

        // Search for the task by title
        while (temp != nullptr && temp->title != title) {
            temp = temp->next;
        }

        // If task not found
        if (temp == nullptr) {
            cout << "Task not found!" << endl;
            return;
        }
    }

    void run() {
        int choice;
        string title, date, description;
        bool validDate = false;
        string currentDate = getCurrentDate();

        while (true) {
            printMenu();
            cout << "Enter your choice: ";

            if (!(cin >> choice)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid choice. Try again." << endl;
                continue;
            }

            switch (choice) {
                case 1:
                    break;
                case 2:
                    cout << "Enter Title: ";
                    cin.ignore();
                    getline(cin, title);
                    cout << "Description: ";
                    getline(cin, description);

                    cout << "Current Date: " << currentDate << endl;

                    do {
                        cout << "[MM/DD/YY] Due Date: ";
                        getline(cin, date);

                        validDate = isValidDate(date);

                        if (!validDate) {
                            cout << "Invalid date format. Please enter a date in the format MM/DD/YY." << endl;
                        }
                    } while (!validDate);

                    createTask(title, date, description);
                    cout << "Task created successfully!" << endl;
                    displayAllTasks();
                    break;
                case 3:
                    displayAllTasks();
                    break;
                case 4:
                    cout << "Enter title to search: ";
                    cin.ignore();
                    getline(cin, title);
                    searchTask(title);
                    break;
                case 5:
                    int position;
                    cout << "Search for the task to be deleted: ";
                    cin.ignore();
                    getline(cin, title);
                    deleteTask(title);
                    break;
                case 6:
                    cout << "Exiting the program..." << endl;
                    return;
                default:
                    cout << "Invalid choice. Try again." << endl;
            }
            cout << endl;
        }
    }
};

int main() {
    TaskManager taskManager;
    taskManager.run();
    return 0;
}
