#include "Doctor.h"
#include "Patient.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include <ctime>

// constructors
doctor::doctor() :  person() {
    specialization = "";
    contact = "";
    fee = 0.0;
}

doctor::doctor(int id, string name, string password, string specialization, string contact, float fee)
    : person(id, name, password) {
    this->specialization = specialization;
    this->contact = contact;
    this->fee = fee;
}

// getters
string doctor::getspec() const { return specialization; }
string doctor::getcontact() const { return contact; }
float doctor::getfee() const { return fee; }

// setters
void doctor::setspecialization(string s) { specialization = s; }
void doctor::setcontact(string c) { contact = c; }
void doctor::setfee(float f) { fee = f; }

// operator overloads
bool doctor::operator==(doctor& other) {
    return this->ID == other.ID;
}

ostream& operator<<(ostream& out, doctor& d) {
    out << "ID: " << d.ID << endl;
    out << "Name: " << d.name << endl;
    out << "Specialization: " << d.specialization << endl;
    out << "Contact: " << d.contact << endl;
    out << "Fee: PKR " << d.fee << endl;
    return out;
}

// menus
void doctor::displayMenu() {
    cout << "\nWelcome, Dr. " << name << endl;
    cout << "Specialization: " << specialization << endl;
    cout << "===============================" << endl;
    cout << "1. View Today's Appointments" << endl;
    cout << "2. Mark Appointment Complete" << endl;
    cout << "3. Mark Appointment No-Show" << endl;
    cout << "4. Write Prescription" << endl;
    cout << "5. View Patient Medical History" << endl;
    cout << "6. Logout" << endl;
}

void doctor::display() const {
    cout << "ID: " << ID
        << " | Name: " << name
        << " | Spec: " << specialization
        << " | Fee: PKR " << fee << endl;
}

// helper to get today's date
string getDoctorTodayDate() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    string day = (now.tm_mday < 10 ? "0" : "") + to_string(now.tm_mday);
    string month = ((now.tm_mon + 1) < 10 ? "0" : "") + to_string(now.tm_mon + 1);
    string year = to_string(now.tm_year + 1900);
    return day + "-" + month + "-" + year;
}

// ==================== VIEW TODAY'S APPOINTMENTS ====================

void doctor::viewTodaysAppointments(Storage<Appointment>& appointments, Storage<patient>& patients) {
    string today = getDoctorTodayDate();
    cout << "\n--- Today's Appointments (" << today << ") ---" << endl;

    Appointment todayApps[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == ID && a.getDate() == today) {
            todayApps[count++] = a;
        }
    }

    if (count == 0) {
        cout << "No appointments scheduled for today." << endl;
        return;
    }

    // bubble sort by time slot ascending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (todayApps[j].getTimeSlot() > todayApps[j + 1].getTimeSlot()) {
                Appointment temp = todayApps[j];
                todayApps[j] = todayApps[j + 1];
                todayApps[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        string patName = "Unknown";
        for (int j = 0; j < patients.size(); j++) {
            if (patients.getAll()[j].getid() == todayApps[i].getPatientId()) {
                patName = patients.getAll()[j].getname();
                break;
            }
        }
        cout << "ID: " << todayApps[i].getAppointmentId()
            << " | Patient: " << patName
            << " | Slot: " << todayApps[i].getTimeSlot()
            << " | Status: " << todayApps[i].getStatus() << endl;
    }
}

// ==================== MARK COMPLETE ====================

void doctor::markAppointmentComplete(Storage<Appointment>& appointments, Storage<patient>& patients, FileHandler& fh) {
    string today = getDoctorTodayDate();
    cout << "\n--- Today's Pending Appointments ---" << endl;
    bool found = false;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == ID && a.getDate() == today && a.getStatus() == "pending") {
            string patName = "Unknown";
            for (int j = 0; j < patients.size(); j++) {
                if (patients.getAll()[j].getid() == a.getPatientId()) {
                    patName = patients.getAll()[j].getname();
                    break;
                }
            }
            cout << "ID: " << a.getAppointmentId()
                << " | Patient: " << patName
                << " | Slot: " << a.getTimeSlot() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No pending appointments today." << endl;
        return;
    }

    int appId;
    cout << "Enter Appointment ID: ";
    cin >> appId;

    Appointment* app = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getAppointmentId() == appId &&
            appointments.getAll()[i].getDoctorId() == ID &&
            appointments.getAll()[i].getDate() == today &&
            appointments.getAll()[i].getStatus() == "pending") {
            app = &appointments.getAll()[i];
            break;
        }
    }

    if (app == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    app->setStatus("completed");
    fh.updateAppointment(*app);
    cout << "Appointment marked as completed." << endl;
}

// ==================== MARK NO-SHOW ====================

void doctor::markAppointmentNoShow(Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<patient>& patients, FileHandler& fh) {
    string today = getDoctorTodayDate();
    cout << "\n--- Today's Pending Appointments ---" << endl;
    bool found = false;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getDoctorId() == ID && a.getDate() == today && a.getStatus() == "pending") {
            string patName = "Unknown";
            for (int j = 0; j < patients.size(); j++) {
                if (patients.getAll()[j].getid() == a.getPatientId()) {
                    patName = patients.getAll()[j].getname();
                    break;
                }
            }
            cout << "ID: " << a.getAppointmentId()
                << " | Patient: " << patName
                << " | Slot: " << a.getTimeSlot() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No pending appointments today." << endl;
        return;
    }

    int appId;
    cout << "Enter Appointment ID: ";
    cin >> appId;

    Appointment* app = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getAppointmentId() == appId &&
            appointments.getAll()[i].getDoctorId() == ID &&
            appointments.getAll()[i].getDate() == today &&
            appointments.getAll()[i].getStatus() == "pending") {
            app = &appointments.getAll()[i];
            break;
        }
    }

    if (app == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    app->setStatus("noshow");
    fh.updateAppointment(*app);

    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getAppointmentId() == appId) {
            bills.getAll()[i].setStatus("cancelled");
            fh.updateBill(bills.getAll()[i]);
            break;
        }
    }

    cout << "Appointment marked as no-show." << endl;
}

// ==================== WRITE PRESCRIPTION ====================

void doctor::writePrescription(Storage<Appointment>& appointments, Storage<Prescription>& prescriptions, FileHandler& fh) {
    int appId;
    cout << "Enter Appointment ID: ";
    cin >> appId;

    Appointment* app = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getAppointmentId() == appId &&
            appointments.getAll()[i].getDoctorId() == ID &&
            appointments.getAll()[i].getStatus() == "completed") {
            app = &appointments.getAll()[i];
            break;
        }
    }

    if (app == nullptr) {
        cout << "Invalid appointment ID or not completed." << endl;
        return;
    }

    // check prescription already exists
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getAppointmentId() == appId) {
            cout << "Prescription already written for this appointment." << endl;
            return;
        }
    }

    string medicines, notes;
    cout << "Enter medicines (e.g. Paracetamol 500mg;Amoxicillin 250mg): ";
    cin.ignore();
    getline(cin, medicines);
    cout << "Enter notes: ";
    getline(cin, notes);

    // generate prescription ID
    int newPresId = 1;
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getPrescriptionId() >= newPresId) {
            newPresId = prescriptions.getAll()[i].getPrescriptionId() + 1;
        }
    }

    string today = getDoctorTodayDate();
    Prescription newPres(newPresId, appId, app->getPatientId(), ID, today, medicines, notes);
    prescriptions.add(newPres);
    fh.savePrescription(newPres);
    cout << "Prescription saved." << endl;
}

// ==================== VIEW PATIENT MEDICAL HISTORY ====================

void doctor::viewPatientMedicalHistory(Storage<patient>& patients, Storage<Appointment>& appointments, Storage<Prescription>& prescriptions) {
    int patId;
    cout << "Enter Patient ID: ";
    cin >> patId;

    // check patient exists
    patient* pat = nullptr;
    for (int i = 0; i < patients.size(); i++) {
        if (patients.getAll()[i].getid() == patId) {
            pat = &patients.getAll()[i];
            break;
        }
    }

    if (pat == nullptr) {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    // check completed appointment with this doctor
    bool hasCompleted = false;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getPatientId() == patId &&
            appointments.getAll()[i].getDoctorId() == ID &&
            appointments.getAll()[i].getStatus() == "completed") {
            hasCompleted = true;
            break;
        }
    }

    if (!hasCompleted) {
        cout << "Access denied. You can only view records of your own patients." << endl;
        return;
    }

    // collect prescriptions
    Prescription myPres[100];
    int count = 0;
    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getPatientId() == patId &&
            prescriptions.getAll()[i].getDoctorId() == ID) {
            myPres[count++] = prescriptions.getAll()[i];
        }
    }

    if (count == 0) {
        cout << "No prescriptions found." << endl;
        return;
    }

    // bubble sort by date descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (myPres[j].getDate() < myPres[j + 1].getDate()) {
                Prescription temp = myPres[j];
                myPres[j] = myPres[j + 1];
                myPres[j + 1] = temp;
            }
        }
    }

    cout << "\n--- Medical History for Patient ID: " << patId << " ---" << endl;
    for (int i = 0; i < count; i++) {
        cout << "Date: " << myPres[i].getDate()
            << " | Medicines: " << myPres[i].getMedicines()
            << " | Notes: " << myPres[i].getNotes() << endl;
    }
}