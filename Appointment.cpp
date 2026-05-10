#include "Appointment.h"

Appointment::Appointment() {
    appointmentId = 0;
    patientId = 0;
    doctorId = 0;
    date = "";
    timeslot = "";
    status = "";
}
Appointment::Appointment(int appointmentId, int patientId, int doctorId, string date, string timeSlot, string status) {
    this->appointmentId = appointmentId;
    this->patientId = patientId;
    this->doctorId = doctorId;
    this->date = date;
    this->timeslot = timeSlot;
    this->status = status;
}
int Appointment::getAppointmentId() {
    return appointmentId;
}
int Appointment::getid() {
    return appointmentId; // This points getid() to your existing ID
}

int Appointment::getPatientId() {
    return patientId;
}

int Appointment::getDoctorId() {
    return doctorId;
}

string Appointment::getDate() {
    return date;
}

string Appointment::getTimeSlot() {
    return timeslot;
}

string Appointment::getStatus() {
    return status;
}

// setters
void Appointment::setAppointmentId(int id) {
    appointmentId = id;
}

void Appointment::setPatientId(int id) {
    patientId = id;
}

void Appointment::setDoctorId(int id) {
    doctorId = id;
}

void Appointment::setDate(string date) {
    this->date = date;
}

void Appointment::setTimeSlot(string timeSlot) {
    this->timeslot = timeSlot;
}

void Appointment::setStatus(string status) {
    this->status = status;
}
bool Appointment::operator==(Appointment& other) {
    if (this->status == "cancelled" || other.status == "cancelled") {
        return false;
    }
    return (this->doctorId == other.doctorId &&
        this->date == other.date &&
        this->timeslot == other.timeslot);
}


ostream& operator<<(ostream& out, Appointment& a) {
    out << "Appointment ID: " << a.appointmentId << endl;
    out << "Patient ID: " << a.patientId << endl;
    out << "Doctor ID: " << a.doctorId << endl;
    out << "Date: " << a.date << endl;
    out << "Time Slot: " << a.timeslot << endl;
    out << "Status: " << a.status << endl;
    return out;
}