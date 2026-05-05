#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <algorithm>

using namespace std;


struct Appointment {
    string petName;
    string ownerContact;
    string date;   // format: YYYY-MM-DD
    string time;   // format: HH:MM
    string reason;
};


vector<Appointment>             appointments;  // holds all appointments
set<string>                     bookedSlots;   
map<string, vector<Appointment>> petHistory;  

const string FILENAME = "appointments.csv";

// file handling
void loadFromFile() {
    ifstream file(FILENAME);

    if (!file.is_open()) {
        return;
    }

    string line;
    getline(file, line); 

    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        Appointment a;

        getline(ss, a.petName,       ',');
        getline(ss, a.ownerContact,  ',');
        getline(ss, a.date,          ',');
        getline(ss, a.time,          ',');
        getline(ss, a.reason,        ',');

        appointments.push_back(a);
        bookedSlots.insert(a.date + "_" + a.time);
        petHistory[a.petName].push_back(a);
    }

    file.close();
    cout << "\n  [✔] Records loaded from file.\n";
}

void saveToFile() {
    ofstream file(FILENAME);

    file << "PetName,OwnerContact,Date,Time,Reason\n";

    // Write each appointment
    for (int i = 0; i < appointments.size(); i++) {
        file << appointments[i].petName      << ","
             << appointments[i].ownerContact << ","
             << appointments[i].date         << ","
             << appointments[i].time         << ","
             << appointments[i].reason       << "\n";
    }

    file.close();
}


bool compareByTime(Appointment a, Appointment b) {
    return a.time < b.time;
}

void printLine() {
    cout << "  " << string(45, '-') << "\n";
}

//  FEATURE 1 - Add New Appointment
//  Uses: set (conflict check), vector, map
void addAppointment() {
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║      ADD NEW APPOINTMENT         ║\n";
    cout << "  ╚══════════════════════════════════╝\n\n";

    Appointment a;

    cin.ignore();
    cout << "  Pet Name         : "; getline(cin, a.petName);
    cout << "  Owner Contact    : "; getline(cin, a.ownerContact);
    cout << "  Date (YYYY-MM-DD): "; getline(cin, a.date);
    cout << "  Time (HH:MM)     : "; getline(cin, a.time);
    cout << "  Reason for Visit : "; getline(cin, a.reason);

    string slot = a.date + "_" + a.time;

    if (bookedSlots.count(slot) > 0) {
        cout << "\n  [!] DOUBLE BOOKING DETECTED!\n";
        cout << "      The time slot " << a.time << " on " << a.date
             << " is already taken.\n";
        cout << "      Please choose a different time.\n";
        return;
    }

    appointments.push_back(a);
    bookedSlots.insert(slot);
    petHistory[a.petName].push_back(a);

    saveToFile();

    cout << "\n  [✔] Appointment saved successfully!\n";
}

//  FEATURE 2 - Search Pet Visit History
//  Uses: map
void searchPetHistory() {
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║      SEARCH PET HISTORY          ║\n";
    cout << "  ╚══════════════════════════════════╝\n\n";

    string name;
    cin.ignore();
    cout << "  Enter Pet Name: "; getline(cin, name);

    //Look up pet in map 
    if (petHistory.count(name) == 0) {
        cout << "\n  [!] No records found for \"" << name << "\".\n";
        return;
    }

    vector<Appointment> history = petHistory[name];

    cout << "\n  Visit History for: " << name << "\n";
    printLine();

    for (int i = 0; i < history.size(); i++) {
        cout << "  Visit #" << (i + 1) << ":\n";
        cout << "    Date    : " << history[i].date          << "\n";
        cout << "    Time    : " << history[i].time          << "\n";
        cout << "    Reason  : " << history[i].reason        << "\n";
        cout << "    Owner   : " << history[i].ownerContact  << "\n";
        if (i < history.size() - 1) printLine();
    }

    printLine();
    cout << "  Total Visits: " << history.size() << "\n";
}

//  FEATURE 3 - View Daily Schedule
//  Uses: vector, sort()
void viewDailySchedule() {
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║      DAILY SCHEDULE              ║\n";
    cout << "  ╚══════════════════════════════════╝\n\n";

    string date;
    cin.ignore();
    cout << "  Enter Date (YYYY-MM-DD): "; getline(cin, date);

    // Filter appointments for that day 
    vector<Appointment> daily;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments[i].date == date) {
            daily.push_back(appointments[i]);
        }
    }

    if (daily.empty()) {
        cout << "\n  [!] No appointments found for " << date << ".\n";
        return;
    }

    //Sort by time using sort()
    sort(daily.begin(), daily.end(), compareByTime);

    cout << "\n  Schedule for " << date << ":\n";
    printLine();

    for (int i = 0; i < daily.size(); i++) {
        cout << "  [" << daily[i].time << "]  "
             << daily[i].petName << "\n";
        cout << "           Owner  : " << daily[i].ownerContact << "\n";
        cout << "           Reason : " << daily[i].reason       << "\n";
        if (i < daily.size() - 1) printLine();
    }

    printLine();
    cout << "  Total Appointments: " << daily.size() << "\n";
}

//  VIEW ALL APPOINTMENTS
void viewAllAppointments() {
    cout << "\n  ╔══════════════════════════════════╗\n";
    cout << "  ║      ALL APPOINTMENTS            ║\n";
    cout << "  ╚══════════════════════════════════╝\n\n";

    if (appointments.empty()) {
        cout << "  No appointments recorded yet.\n";
        return;
    }

    for (int i = 0; i < appointments.size(); i++) {
        cout << "  #" << (i + 1) << "\n";
        cout << "    Pet Name : " << appointments[i].petName      << "\n";
        cout << "    Owner    : " << appointments[i].ownerContact  << "\n";
        cout << "    Date     : " << appointments[i].date          << "\n";
        cout << "    Time     : " << appointments[i].time          << "\n";
        cout << "    Reason   : " << appointments[i].reason        << "\n";
        printLine();
    }

    cout << "  Total Records: " << appointments.size() << "\n";
}

//  MAIN MENU
int main() {
    cout << "\n  ╔══════════════════════════════════════════╗\n";
    cout << "  ║   PET CLINIC APPOINTMENT MANAGER         ║\n";
    cout << "  ╚══════════════════════════════════════════╝\n";

    // Load data galing sa file
    loadFromFile();

    int choice;

    do {
        cout << "\n  ╔══════════════════════════════════╗\n";
        cout << "  ║            MAIN MENU             ║\n";
        cout << "  ╠══════════════════════════════════╣\n";
        cout << "  ║  [1] Add New Appointment         ║\n";
        cout << "  ║  [2] Search Pet History          ║\n";
        cout << "  ║  [3] View Daily Schedule         ║\n";
        cout << "  ║  [4] View All Appointments       ║\n";
        cout << "  ║  [0] Exit                        ║\n";
        cout << "  ╚══════════════════════════════════╝\n";
        cout << "  Choice: ";
        cin >> choice;

        if (choice == 1) {
            addAppointment();
        } else if (choice == 2) {
            searchPetHistory();
        } else if (choice == 3) {
            viewDailySchedule();
        } else if (choice == 4) {
            viewAllAppointments();
        } else if (choice == 0) {
            cout << "\n  Goodbye! Stay pawsitive. \n\n";
        } else {
            cout << "\n  [!] Invalid choice. Please enter 0-4.\n";
        }

    } while (choice != 0);

    return 0;
}