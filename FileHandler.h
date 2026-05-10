#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "HospitalException.h"
#include "FileNotFoundException.h"
#include "InsufficientFundsException.h"
#include "InvalidInputException.h"
#include "SlotUnavailableException.h"
using namespace std;

class FileHandler {
private:
    void splitLine(string line, string parts[], int& count);
    int stringToInt(string s);
    float stringToFloat(string s);

public:
    // loading
    void loadPatients(Storage<patient>& patients);
    void loadDoctors(Storage<doctor>& doctors);
    void loadAdmin(Admin& admin);
    void loadAppointments(Storage<Appointment>& appointments);
    void loadBills(Storage<Bill>& bills);
    void loadPrescriptions(Storage<Prescription>& prescriptions);

    // saving
    void savePatient(patient& p);
    void saveDoctor(doctor& d);
    void saveAppointment(Appointment& a);
    void saveBill(Bill& b);
    void savePrescription(Prescription& p);

    // updating
    void updatePatient(patient& p);
    void updateAppointment(Appointment& a);
    void updateBill(Bill& b);

    // deleting
    void deletePatient(int id);
    void deleteDoctor(int id);

    // special
    void logSecurityEvent(string log);
    void saveDischargedPatient(patient& p);
};

#endif