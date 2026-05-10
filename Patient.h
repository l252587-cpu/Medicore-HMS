#ifndef PATIENT_H
#define PATIENT_H
#include "Person.h"
#include "Exceptions.h"
#include "Validator.h"
#include <iostream>
#include <string>
using namespace std;

// forward declarations
class doctor;
class Appointment;
class Bill;
class Prescription;
template <class T> class Storage;
class FileHandler;

class patient : public person {
private:
    int age;
    string gender;
    string contact;
    float balance;

public:
    // constructors
    patient();
    patient(int id, string name, string password, int age, string gender, string contact, float balance);

    // getters
    int getage() const;
    string getgender() const;
    string getcontact() const;
    float getbalance() const;

    // setters
    void setbalance(float amount);

    // menu functions
    void displayMenu();
    void display() const;

    void bookAppointment(Storage<doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, FileHandler& fh);
    void cancelAppointment(Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<doctor>& doctors, FileHandler& fh);
    void viewAppointments(Storage<Appointment>& appointments, Storage<doctor>& doctors);
    void viewMedicalRecords(Storage<Prescription>& prescriptions, Storage<doctor>& doctors);
    void viewBills(Storage<Bill>& bills);
    void payBill(Storage<Bill>& bills, FileHandler& fh);
    void topUpBalance(FileHandler& fh);

    // operator overloads
    void operator+=(float amount);
    void operator-=(float amount);
    bool operator==(patient& other);
    friend ostream& operator<<(ostream& out, patient& p);
};

#endif