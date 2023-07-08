#include <iostream>
#include <string>
#include <queue>

using namespace std;

void printLine(const string& text, int maxLength) {
    cout << "| " << text;
    int padding = maxLength - text.length();
    cout << string(padding, ' ') << " |" << endl;
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
        menuItems.pop();
    }

    printBottomBorder(maxLength + 4);
}

int main() {
    printMenu();
    cout << "Enter your choice: ";

    return 0;
}
