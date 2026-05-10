#ifndef DOCTOR_H
#define DOCTOR_H

#include "Person.h"
#include "Exceptions.h"
#include "Validator.h"
#include <iostream>
using namespace std;

// forward declarations
class patient;
class Appointment;
class Bill;
class Prescription;
template <class T> class Storage;
class FileHandler;

class doctor : public person {
private:
    string specialization;
    string contact;
    float fee;

public:
    // constructors
    doctor();
    doctor(int id, string name, string password, string specialization, string contact, float fee);

    // getters
    string getspec() const;
    string getcontact() const;
    float getfee() const;

    // setters
    void setspecialization(string specialization);
    void setcontact(string contact);
    void setfee(float fee);

    // operator overloads
    bool operator==(doctor& other);
    friend ostream& operator<<(ostream& out, doctor& d);

    // menu functions
    void displayMenu();
    void display() const;
    void viewTodaysAppointments(Storage<Appointment>& appointments, Storage<patient>& patients);
    void markAppointmentComplete(Storage<Appointment>& appointments, Storage<patient>& patients, FileHandler& fh);
    void markAppointmentNoShow(Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<patient>& patients, FileHandler& fh);
    void writePrescription(Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, FileHandler& fh);
    void viewPatientMedicalHistory(Storage<patient>& patients, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions);
};

#endif