#include "Bill.h"


Bill::Bill() {
    billId = 0;
    patientId = 0;
    appointmentId = 0;
    amount = 0.0;
    status = "";
    date = "";
}


Bill::Bill(int billId, int patientId, int appointmentId, float amount, string status, string date) {
    this->billId = billId;
    this->patientId = patientId;
    this->appointmentId = appointmentId;
    this->amount = amount;
    this->status = status;
    this->date = date;
}


int Bill::getBillId() {
    return billId;
}

int Bill::getPatientId() {
    return patientId;
}

int Bill::getAppointmentId() {
    return appointmentId;
}

float Bill::getAmount() {
    return amount;
}

string Bill::getStatus() {
    return status;
}

string Bill::getDate() {
    return date;
}


void Bill::setBillId(int id) {
    billId = id;
}

void Bill::setPatientId(int id) {
    patientId = id;
}

void Bill::setAppointmentId(int id) {
    appointmentId = id;
}

void Bill::setAmount(float amount) {
    this->amount = amount;
}

void Bill::setStatus(string status) {
    this->status = status;
}

void Bill::setDate(string date) {
    this->date = date;
}
int Bill::getid() {
    return billId; // This points getid() to your existing ID
}

ostream& operator<<(ostream& out, Bill& b) {
    out << "Bill ID: " << b.billId << endl;
    out << "Patient ID: " << b.patientId << endl;
    out << "Appointment ID: " << b.appointmentId << endl;
    out << "Amount: PKR " << b.amount << endl;
    out << "Status: " << b.status << endl;
    out << "Date: " << b.date << endl;
    return out;
}