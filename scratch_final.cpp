<<<<<<< HEAD
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
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

class State {
    public:
        AccountManager acc_manager;

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

            if (acc_manager.verify_login(username, password)) {
                Account acc = acc_manager.retrieve_account(username);
            } else {
                cout << "\nUsername or password is incorrect.\n\n";
                system("pause");
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
        }   
};

int main() {
    State state;

    state.homepage();

    return 0;
=======
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
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

class State {
    public:
        AccountManager acc_manager;

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

            if (acc_manager.verify_login(username, password)) {
                Account acc = acc_manager.retrieve_account(username);
            } else {
                cout << "\nUsername or password is incorrect.\n\n";
                system("pause");
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
        }   
};

int main() {
    State state;

    state.homepage();

    return 0;
>>>>>>> 7667cb26151fba176586344964be0c649c14c74f
}