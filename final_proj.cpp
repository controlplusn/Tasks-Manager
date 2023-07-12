#include <iostream>
#include <string>
#include <vector>
using namespace std;

struct Student {
    string name;
    string student_num;
    int age;
};

// =================================== HEADER ===================================
void profileMenu() {
    // Add user, choose user, display all user
    cout << "***************PROFILE***************" << endl;
    cout << "[1] Add user" << endl;
    cout << "[2] Display users" << endl; 
    cout << "[3] Choose user" << endl;
    cout << "[4] Exit" << endl; 
    cout << "**********************************" << endl;
}


void showMenu() {
    cout << "***************MENU***************" << endl;
    cout << "[1] Create tasks" << endl;
    cout << "[2] lists of tasks" << endl;
    cout << "[3] Search task" << endl;
    cout << "[4] Exit" << endl; 
}

// =================================== STUDENT PROFILE ===================================
class StudentProfile {
private:
    Student student; // Student is the struct, student is the object

public:
    // Constructor
    StudentProfile(string n, string sn, int a) {
        student.name = n;
        student.student_num = sn;
        student.age = a;
    }

    void displayProfile() {
        cout << "Name: " << student.name << endl;
        cout << "Student number: " << student.student_num << endl;
        cout << "Age: " << student.age << endl;
    }

    Student getStudent() {
        return student;
    }
};


int main() 
{
    // show menu, input user, create tasks, lists of tasks, search tasks
    char option;
    vector<StudentProfile> studentDisplay;

    do {
    profileMenu();
    cout << "Enter choice: ";
    cin >> option;
    cin.ignore();

    switch (option) {
        case '1': {
            system("cls");
            string name;
            string student_num;
            int age;

            cout << "=============== Enter student details ===============\n";
            cout << "Enter Name: ";
            cin >> name;
            getline(cin, name);
            cout << "Enter student number: ";
            cin >> student_num;
            cout << "Enter age: ";
            cin >> age;
            cin.ignore();

            StudentProfile user(name, student_num, age);
            studentDisplay.push_back(user); // add new student/ studentDisplay is the object for vector, user is the object for the costructor
            cout << "Student added successfully!\n";
            break;
        }
        case '2': {
            system("cls");
            cout << "\n=============== Enter student details ===============\n";
            for(auto& user : studentDisplay) {
                user.displayProfile();
                cout << "=====================================================\n";
            }
            break;
        }
        case '3': {
            break;
        }   

    }   
    } while(option != 4);
    
    
    return 0;
}