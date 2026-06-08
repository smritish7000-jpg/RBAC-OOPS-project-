#include <iostream>
#include <fstream>
#include <string>
#include<vector>
#include <ctime>
using namespace std;

// BASE USER CLASS

class User {
protected:
    string username;
    string password;
    string role;

public:
    User(string u, string p, string r) {
        username = u;
        password = p;
        role = r;
    }

    virtual void accessResource() = 0;

    string getUsername() {
        return username;
    }

    string getPassword() {
        return password;
    }

    string getRole() {
        return role;
    }

    virtual ~User() {}
};

// ADMIN CLASS
class Admin : public User {
public:
    Admin(string u, string p)
        : User(u, p, "Admin") {}

    void accessResource() override {
        cout << "Admin Access Granted: Read/Write/Delete Allowed\n";
    }
};

//  NORMAL USER CLASS 

class NormalUser : public User {
public:
    NormalUser(string u, string p)
        : User(u, p, "User") {}

    void accessResource() override {
        cout << "User Access Granted: Read/Write Allowed\n";
    }
};

// GUEST CLASS

class Guest : public User {
public:
    Guest(string u, string p)
        : User(u, p, "Guest") {}

    void accessResource() override {
        cout << "Guest Access Granted: Read Only\n";
    }
};

//  AUTHENTICATION SYSTEM 

class AuthSystem {
public:

    void registerUser() {

        string username, password, role;

        cout << "\nEnter Username: ";
        cin >> username;

        cout << "Enter Password: ";
        cin >> password;

        cout << "Enter Role (Admin/User/Guest): ";
        cin >> role;

        ofstream file("users.txt", ios::app);

        file << username << " "
             << password << " "
             << role << endl;

        file.close();

        cout << "\nRegistration Successful!\n";
    }

    bool loginUser(string &loggedUser, string &loggedRole) {

        string username, password;
        string fileUser, filePass, fileRole;

        cout << "\nEnter Username: ";
        cin >> username;

        cout << "Enter Password: ";
        cin >> password;

        ifstream file("users.txt");

        while (file >> fileUser >> filePass >> fileRole) {

            if (username == fileUser &&
                password == filePass) {

                loggedUser = fileUser;
                loggedRole = fileRole;

                cout << "\nLogin Successful!\n";
                cout << "Welcome " << loggedUser << endl;
                cout << "Role: " << loggedRole << endl;

                file.close();
                return true;
            }
        }

        file.close();

        cout << "\nInvalid Username or Password!\n";
        return false;
    }
};

// ================= ACCESS LOGGING =================

void writeLog(string username, string action) {
    ofstream logFile("access_log.txt", ios::app);
    time_t now = time(0);
    logFile << username
            << " -> "
            << action
            << " -> "
            << ctime(&now);
    logFile.close();
}


// ================= RESOURCE ACCESS =================

void accessFile(string username, string role) {
    if (role == "Admin") {
        Admin obj(username, "123");
        obj.accessResource();
        writeLog(username, "Admin Accessed Resource");
    }
    else if (role == "User") {
        NormalUser obj(username, "123");
        obj.accessResource();
        writeLog(username, "User Accessed Resource");
    }
    else if (role == "Guest") {
        Guest obj(username, "123");
        obj.accessResource();
        writeLog(username, "Guest Accessed Resource");
    }
    else {
        cout << "Invalid Role\n";
    }
}


// ================= KEYLOGGER DETECTOR =================

void detectKeylogger() {
    ifstream file("keystrokes.txt");
    if (!file) {
        cout << "\nKeystroke file not found!\n";
        return;
    }
    string data;
    file >> data;
    file.close();
    int suspicious = 0;
    for (int i = 0; i < data.length() - 3; i++) {
        if (data[i] == data[i + 1] &&
            data[i] == data[i + 2] &&
            data[i] == data[i + 3]) {
            suspicious++;
        }
    }
    if (suspicious > 0) {
        cout << "\nWARNING: Suspicious Keylogger Pattern Detected!\n";
        ofstream report("report.txt", ios::app);
        report << "Suspicious activity detected in keystrokes.\n";
        report.close();
    }
    else {
        cout << "\nNo Suspicious Activity Found.\n";
    }
}


// ================= VIEW ACCESS LOG =================

void viewLogs() {

    ifstream file("access_log.txt");

    string line;

    cout << "\n  ACCESS LOGS  \n";

    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

// ================= MAIN FUNCTION =================

int main() {

    AuthSystem auth;

    int choice;

    string loggedUser;
    string loggedRole;

    bool isLoggedIn = false;

    do {

        cout << "\n========= RBAC SYSTEM =========\n";

        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Access Resource\n";
        cout << "4. Detect Keylogger\n";
        cout << "5. View Logs\n";
        cout << "6. Exit\n";

        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            auth.registerUser();
            break;

        case 2:
            isLoggedIn = auth.loginUser(loggedUser, loggedRole);
            break;

        case 3:

            if (isLoggedIn) {
                accessFile(loggedUser, loggedRole);
            }
            else {
                cout << "\nPlease Login First!\n";
            }

            break;

        case 4:
            detectKeylogger();
            break;

        case 5:
            viewLogs();
            break;

        case 6:
            cout << "\nProgram Exited.\n";
            break;

        default:
            cout << "\nInvalid Choice!\n";
        }

    } while (choice != 6);

    return 0;
}