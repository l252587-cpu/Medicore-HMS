#ifndef PRESCRIPTION_H
#define PRESCRIPTION_H

#include <iostream>
#include <string>
using namespace std;

class Prescription {
private:
    int prescriptionId;
    int appointmentId;
    int patientId;
    int doctorId;
    string date;
    string medicines;
    string notes;

public:
  
    Prescription();
    Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, string date, string medicines, string notes);

    int getid();
    int getPrescriptionId();
    int getAppointmentId();
    int getPatientId();
    int getDoctorId();
    string getDate();
    string getMedicines();
    string getNotes();

   
    void setPrescriptionId(int id);
    void setAppointmentId(int id);
    void setPatientId(int id);
    void setDoctorId(int id);
    void setDate(string date);
    void setMedicines(string medicines);
    void setNotes(string notes);

   
    friend ostream& operator<<(ostream& out, Prescription& p);
};

#endif
