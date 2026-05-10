#include "Prescription.h"

// empty constructor
Prescription::Prescription() {
    prescriptionId = 0;
    appointmentId = 0;
    patientId = 0;
    doctorId = 0;
    date = "";
    medicines = "";
    notes = "";
}

int Prescription::getid() {
    return prescriptionId; // This points getid() to your existing ID
}

Prescription::Prescription(int prescriptionId, int appointmentId, int patientId, int doctorId, string date, string medicines, string notes) {
    this->prescriptionId = prescriptionId;
    this->appointmentId = appointmentId;
    this->patientId = patientId;
    this->doctorId = doctorId;
    this->date = date;
    this->medicines = medicines;
    this->notes = notes;
}


int Prescription::getPrescriptionId() {
    return prescriptionId;
}

int Prescription::getAppointmentId() {
    return appointmentId;
}

int Prescription::getPatientId() {
    return patientId;
}

int Prescription::getDoctorId() {
    return doctorId;
}

string Prescription::getDate() {
    return date;
}

string Prescription::getMedicines() {
    return medicines;
}

string Prescription::getNotes() {
    return notes;
}


void Prescription::setPrescriptionId(int id) {
    prescriptionId = id;
}

void Prescription::setAppointmentId(int id) {
    appointmentId = id;
}

void Prescription::setPatientId(int id) {
    patientId = id;
}

void Prescription::setDoctorId(int id) {
    doctorId = id;
}

void Prescription::setDate(string date) {
    this->date = date;
}

void Prescription::setMedicines(string medicines) {
    this->medicines = medicines;
}

void Prescription::setNotes(string notes) {
    this->notes = notes;
}


ostream& operator<<(ostream& out, Prescription& p) {
    out << "Prescription ID: " << p.prescriptionId << endl;
    out << "Appointment ID: " << p.appointmentId << endl;
    out << "Patient ID: " << p.patientId << endl;
    out << "Doctor ID: " << p.doctorId << endl;
    out << "Date: " << p.date << endl;
    out << "Medicines: " << p.medicines << endl;
    out << "Notes: " << p.notes << endl;
    return out;
}