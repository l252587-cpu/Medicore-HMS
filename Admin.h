#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include "Exceptions.h"
#include "Validator.h"
#include <iostream>
using namespace std;

// forward declarations
class patient;
class doctor;
class Appointment;
class Bill;
class Prescription;
template <class T> class Storage;
class FileHandler;

class Admin : public person {
public:
    // constructors
    Admin();
    Admin(int id, string name, string password);

    // menu functions
    void displayMenu();
    void display() const;
    void addDoctor(Storage<doctor>& doctors, FileHandler& fh);
    void removeDoctor(Storage<doctor>& doctors, Storage<Appointment>& appointments, FileHandler& fh);
    void viewAllPatients(Storage<patient>& patients, Storage<Bill>& bills);
    void viewAllDoctors(Storage<doctor>& doctors);
    void viewAllAppointments(Storage<Appointment>& appointments, Storage<patient>& patients, Storage<doctor>& doctors);
    void viewUnpaidBills(Storage<Bill>& bills, Storage<patient>& patients);
    void dischargePatient(Storage<patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, FileHandler& fh);
    void viewSecurityLog();
    void generateDailyReport(Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<patient>& patients, Storage<doctor>& doctors);
};

#endif