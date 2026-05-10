#include <iostream>
#include <fstream>
#include "Patient.h"
#include "Doctor.h"
#include "Admin.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include "Validator.h"
#include "Exceptions.h"
#include "../../GUI.h"
#include <ctime>
#include <direct.h>
using namespace std;

// global storage objects
Storage<patient> patients;
Storage<doctor> doctors;
Storage<Appointment> appointments;
Storage<Bill> bills;
Storage<Prescription> prescriptions;
Admin admin;
FileHandler fh;
GUI gui;

// ==================== HELPER FUNCTIONS ====================

string getCurrentDateTime() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    string day = (now.tm_mday < 10 ? "0" : "") + to_string(now.tm_mday);
    string month = ((now.tm_mon + 1) < 10 ? "0" : "") + to_string(now.tm_mon + 1);
    string year = to_string(now.tm_year + 1900);
    string hour = (now.tm_hour < 10 ? "0" : "") + to_string(now.tm_hour);
    string min = (now.tm_min < 10 ? "0" : "") + to_string(now.tm_min);
    string sec = (now.tm_sec < 10 ? "0" : "") + to_string(now.tm_sec);
    return day + "-" + month + "-" + year + " " + hour + ":" + min + ":" + sec;
}

string getCurrentDate() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    string day = (now.tm_mday < 10 ? "0" : "") + to_string(now.tm_mday);
    string month = ((now.tm_mon + 1) < 10 ? "0" : "") + to_string(now.tm_mon + 1);
    string year = to_string(now.tm_year + 1900);
    return day + "-" + month + "-" + year;
}

// ==================== CREATE FILES ====================

void createFilesIfNotExist() {
    ofstream p("patients.txt", ios::app);       p.close();
    ofstream d("doctors.txt", ios::app);        d.close();
    ofstream a("admin.txt", ios::app);          a.close();
    ofstream ap("appointments.txt", ios::app);  ap.close();
    ofstream b("bills.txt", ios::app);          b.close();
    ofstream pr("prescriptions.txt", ios::app); pr.close();
    ofstream s("security_log.txt", ios::app);   s.close();
    ofstream dis("discharged.txt", ios::app);   dis.close();
}

// ==================== LOAD ALL DATA ====================

void loadAllData() {
    try { fh.loadPatients(patients); }
    catch (FileNotFoundException& e) { cout << e.what() << endl; }

    try { fh.loadDoctors(doctors); }
    catch (FileNotFoundException& e) { cout << e.what() << endl; }

    try { fh.loadAdmin(admin); }
    catch (FileNotFoundException& e) { cout << e.what() << endl; }

    try { fh.loadAppointments(appointments); }
    catch (FileNotFoundException& e) { cout << e.what() << endl; }

    try { fh.loadBills(bills); }
    catch (FileNotFoundException& e) { cout << e.what() << endl; }

    try { fh.loadPrescriptions(prescriptions); }
    catch (FileNotFoundException& e) { cout << e.what() << endl; }
}

// ==================== PATIENT MENU ====================

void runPatientMenu(patient& p) {
    while (true) {
        int choice = gui.showPatientMenu(p.getname(), p.getbalance());

        if (choice == 8 || choice == 10) {
            gui.showMessage("Logged out successfully.");
            break;
        }

        switch (choice) {
        case 1: { // BOOK APPOINTMENT
            string spec = gui.getInput("Enter Specialization (e.g. Cardiology):");
            if (spec == "") break;

            string docNames[50];
            int docIds[50];
            int count = 0;
            for (int i = 0; i < doctors.size(); i++) {
                if (doctors.getAll()[i].getspec() == spec) {
                    docNames[count] = doctors.getAll()[i].getname() +
                        " (Fee: " + to_string((int)doctors.getAll()[i].getfee()) + ")";
                    docIds[count] = doctors.getAll()[i].getid();
                    count++;
                }
            }

            if (count == 0) {
                gui.showMessage("No doctors found for: " + spec);
            }
            else {
                int sel = gui.showSelectionList("Select Doctor", docNames, count);
                if (sel != -1) {
                    string date = gui.getInput("Enter Date (DD-MM-YYYY):");
                    if (!Validator::isValidDate(date)) {
                        gui.showMessage("Invalid Date!");
                        break;
                    }

                    // show available slots
                    string allSlots[8] = { "09:00","10:00","11:00","12:00","13:00","14:00","15:00","16:00" };
                    string availableSlots[8];
                    int slotCount = 0;
                    for (int i = 0; i < 8; i++) {
                        bool taken = false;
                        for (int j = 0; j < appointments.size(); j++) {
                            if (appointments.getAll()[j].getDoctorId() == docIds[sel] &&
                                appointments.getAll()[j].getDate() == date &&
                                appointments.getAll()[j].getTimeSlot() == allSlots[i] &&
                                appointments.getAll()[j].getStatus() != "cancelled") {
                                taken = true;
                                break;
                            }
                        }
                        if (!taken) {
                            availableSlots[slotCount++] = allSlots[i];
                        }
                    }

                    if (slotCount == 0) {
                        gui.showMessage("No slots available for this date.");
                        break;
                    }

                    int slotSel = gui.showSelectionList("Select Time Slot", availableSlots, slotCount);
                    if (slotSel != -1) {
                        float fee = doctors.getAll()[sel].getfee();
                        if (p.getbalance() < fee) {
                            gui.showMessage("Insufficient balance! Please top up first.");
                            break;
                        }

                        // generate new appointment ID
                        int newId = 1;
                        for (int i = 0; i < appointments.size(); i++) {
                            if (appointments.getAll()[i].getAppointmentId() >= newId)
                                newId = appointments.getAll()[i].getAppointmentId() + 1;
                        }

                        Appointment app(newId, p.getid(), docIds[sel], date, availableSlots[slotSel], "pending");
                        appointments.add(app);
                        fh.saveAppointment(app);

                        // generate new bill ID
                        int newBillId = 1;
                        for (int i = 0; i < bills.size(); i++) {
                            if (bills.getAll()[i].getBillId() >= newBillId)
                                newBillId = bills.getAll()[i].getBillId() + 1;
                        }

                        Bill newBill(newBillId, p.getid(), newId, fee, "unpaid", date);
                        bills.add(newBill);
                        fh.saveBill(newBill);

                        p.setbalance(p.getbalance() - fee);
                        fh.updatePatient(p);

                        gui.showMessage("Appointment booked! ID: " + to_string(newId));
                    }
                }
            }
            break;
        }

        case 2: { // CANCEL APPOINTMENT
            string list[100];
            int appIds[100];
            int count = 0;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getPatientId() == p.getid() &&
                    appointments.getAll()[i].getStatus() == "pending") {
                    doctor* d = doctors.findById(appointments.getAll()[i].getDoctorId());
                    list[count] = "ID:" + to_string(appointments.getAll()[i].getAppointmentId()) +
                        " | Dr. " + (d ? d->getname() : "Doc") +
                        " | " + appointments.getAll()[i].getDate();
                    appIds[count++] = appointments.getAll()[i].getAppointmentId();
                }
            }
            if (count == 0) {
                gui.showMessage("No pending appointments to cancel.");
            }
            else {
                int sel = gui.showSelectionList("Select to Cancel", list, count);
                if (sel != -1) {
                    Appointment* a = appointments.findById(appIds[sel]);
                    if (a) {
                        // refund fee
                        doctor* d = doctors.findById(a->getDoctorId());
                        if (d) {
                            p.setbalance(p.getbalance() + d->getfee());
                            fh.updatePatient(p);
                        }
                        a->setStatus("cancelled");
                        fh.updateAppointment(*a);

                        // cancel bill
                        for (int i = 0; i < bills.size(); i++) {
                            if (bills.getAll()[i].getAppointmentId() == appIds[sel]) {
                                bills.getAll()[i].setStatus("cancelled");
                                fh.updateBill(bills.getAll()[i]);
                                break;
                            }
                        }
                        gui.showMessage("Appointment cancelled. Fee refunded.");
                    }
                }
            }
            break;
        }

        case 3: { // VIEW APPOINTMENTS
            string list[100];
            int count = 0;

            // collect appointments
            Appointment myApps[100];
            int myCount = 0;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getPatientId() == p.getid()) {
                    myApps[myCount++] = appointments.getAll()[i];
                }
            }

            // sort by date ascending
            for (int i = 0; i < myCount - 1; i++) {
                for (int j = 0; j < myCount - i - 1; j++) {
                    if (myApps[j].getDate() > myApps[j + 1].getDate()) {
                        Appointment temp = myApps[j];
                        myApps[j] = myApps[j + 1];
                        myApps[j + 1] = temp;
                    }
                }
            }

            for (int i = 0; i < myCount; i++) {
                doctor* d = doctors.findById(myApps[i].getDoctorId());
                list[count++] = myApps[i].getDate() +
                    " | Dr. " + (d ? d->getname() : "Doc") +
                    " | " + myApps[i].getTimeSlot() +
                    " [" + myApps[i].getStatus() + "]";
            }

            if (count == 0) gui.showMessage("No appointments found.");
            else gui.showSelectionList("My Appointments", list, count);
            break;
        }

        case 4: { // VIEW MEDICAL RECORDS
            string list[100];
            int count = 0;

            // collect prescriptions
            Prescription myPres[100];
            int myCount = 0;
            for (int i = 0; i < prescriptions.size(); i++) {
                if (prescriptions.getAll()[i].getPatientId() == p.getid()) {
                    myPres[myCount++] = prescriptions.getAll()[i];
                }
            }

            // sort by date descending
            for (int i = 0; i < myCount - 1; i++) {
                for (int j = 0; j < myCount - i - 1; j++) {
                    if (myPres[j].getDate() < myPres[j + 1].getDate()) {
                        Prescription temp = myPres[j];
                        myPres[j] = myPres[j + 1];
                        myPres[j + 1] = temp;
                    }
                }
            }

            for (int i = 0; i < myCount; i++) {
                doctor* d = doctors.findById(myPres[i].getDoctorId());
                list[count++] = myPres[i].getDate() +
                    " | Dr. " + (d ? d->getname() : "Doc") +
                    " | " + myPres[i].getMedicines();
            }

            if (count == 0) gui.showMessage("No medical records found.");
            else gui.showSelectionList("Medical Records", list, count);
            break;
        }

        case 5: { // VIEW BILLS
            string list[100];
            int count = 0;
            float totalUnpaid = 0;
            for (int i = 0; i < bills.size(); i++) {
                if (bills.getAll()[i].getPatientId() == p.getid()) {
                    list[count++] = "Bill ID:" + to_string(bills.getAll()[i].getBillId()) +
                        " | PKR " + to_string((int)bills.getAll()[i].getAmount()) +
                        " | " + bills.getAll()[i].getStatus() +
                        " | " + bills.getAll()[i].getDate();
                    if (bills.getAll()[i].getStatus() == "unpaid")
                        totalUnpaid += bills.getAll()[i].getAmount();
                }
            }
            if (count == 0) {
                gui.showMessage("No bills found.");
            }
            else {
                list[count++] = "--- Total Unpaid: PKR " + to_string((int)totalUnpaid) + " ---";
                gui.showSelectionList("My Bills", list, count);
            }
            break;
        }

        case 6: { // PAY BILL
            string list[100];
            int billIds[100];
            int count = 0;
            for (int i = 0; i < bills.size(); i++) {
                if (bills.getAll()[i].getPatientId() == p.getid() &&
                    bills.getAll()[i].getStatus() == "unpaid") {
                    list[count] = "Bill ID:" + to_string(bills.getAll()[i].getBillId()) +
                        " | Amount: PKR " + to_string((int)bills.getAll()[i].getAmount());
                    billIds[count++] = bills.getAll()[i].getBillId();
                }
            }
            if (count == 0) {
                gui.showMessage("No unpaid bills.");
            }
            else {
                int sel = gui.showSelectionList("Select Bill to Pay", list, count);
                if (sel != -1) {
                    Bill* b = bills.findById(billIds[sel]);
                    if (b) {
                        if (p.getbalance() >= b->getAmount()) {
                            p.setbalance(p.getbalance() - b->getAmount());
                            b->setStatus("paid");
                            fh.updatePatient(p);
                            fh.updateBill(*b);
                            gui.showMessage("Bill paid! Remaining balance: PKR " + to_string((int)p.getbalance()));
                        }
                        else {
                            gui.showMessage("Insufficient balance! Please top up.");
                        }
                    }
                }
            }
            break;
        }

        case 7: { // TOP UP BALANCE
            string amtStr = gui.getInput("Enter Amount to Add (PKR):");
            if (amtStr == "") break;
            float amt = 0;
            for (int i = 0; i < (int)amtStr.size(); i++) {
                if (amtStr[i] >= '0' && amtStr[i] <= '9') {
                    amt = amt * 10 + (amtStr[i] - '0');
                }
            }
            if (amt > 0) {
                p.setbalance(p.getbalance() + amt);
                fh.updatePatient(p);
                gui.showMessage("Balance updated! New balance: PKR " + to_string((int)p.getbalance()));
            }
            else {
                gui.showMessage("Invalid amount. Must be greater than 0.");
            }
            break;
        }
        }
    }
}

void loginPatient() {
    int failedAttempts = 0;
    while (failedAttempts < 3) {
        int id = 0;
        string password = "";

        bool submitted = gui.showLoginScreen("Patient", id, password);
        if (!submitted) return;

        patient* found = nullptr;
        for (int i = 0; i < patients.size(); i++) {
            if (patients.getAll()[i].getid() == id) {
                found = &patients.getAll()[i];
                break;
            }
        }

        if (found != nullptr && found->getpassword() == password) {
            gui.showMessage("Login successful! Welcome " + found->getname());
            runPatientMenu(*found);
            return;
        }
        else {
            failedAttempts++;
            string msg = "Invalid ID or password. Attempts left: ";
            msg += to_string(3 - failedAttempts);
            gui.showMessage(msg);
            string log = getCurrentDateTime() + ",Patient," + to_string(id) + ",FAILED";
            fh.logSecurityEvent(log);
        }
    }
    gui.showMessage("Account locked. Contact admin.");
}

// ==================== DOCTOR MENU ====================

void runDoctorMenu(doctor& d) {
    while (true) {
        int choice = gui.showDoctorMenu(d.getname(), d.getspec());

        if (choice == 6 || choice == 10) {
            gui.showMessage("Doctor logged out.");
            break;
        }

        string today = getCurrentDate();

        switch (choice) {
        case 1: { // VIEW TODAY'S APPOINTMENTS
            string list[100];
            int count = 0;

            Appointment todayApps[100];
            int tCount = 0;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getDoctorId() == d.getid() &&
                    appointments.getAll()[i].getDate() == today) {
                    todayApps[tCount++] = appointments.getAll()[i];
                }
            }

            // sort by time slot
            for (int i = 0; i < tCount - 1; i++) {
                for (int j = 0; j < tCount - i - 1; j++) {
                    if (todayApps[j].getTimeSlot() > todayApps[j + 1].getTimeSlot()) {
                        Appointment temp = todayApps[j];
                        todayApps[j] = todayApps[j + 1];
                        todayApps[j + 1] = temp;
                    }
                }
            }

            for (int i = 0; i < tCount; i++) {
                patient* p = patients.findById(todayApps[i].getPatientId());
                list[count++] = todayApps[i].getTimeSlot() +
                    " | " + (p ? p->getname() : "Patient") +
                    " [" + todayApps[i].getStatus() + "]";
            }

            if (count == 0) gui.showMessage("No appointments scheduled for today.");
            else gui.showSelectionList("Today's Schedule", list, count);
            break;
        }

        case 2: { // MARK COMPLETE
            string list[100];
            int appIds[100];
            int count = 0;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getDoctorId() == d.getid() &&
                    appointments.getAll()[i].getDate() == today &&
                    appointments.getAll()[i].getStatus() == "pending") {
                    patient* p = patients.findById(appointments.getAll()[i].getPatientId());
                    list[count] = "ID:" + to_string(appointments.getAll()[i].getAppointmentId()) +
                        " | " + (p ? p->getname() : "Patient") +
                        " | " + appointments.getAll()[i].getTimeSlot();
                    appIds[count++] = appointments.getAll()[i].getAppointmentId();
                }
            }
            if (count == 0) gui.showMessage("No pending appointments today.");
            else {
                int sel = gui.showSelectionList("Mark as Complete", list, count);
                if (sel != -1) {
                    Appointment* a = appointments.findById(appIds[sel]);
                    if (a) {
                        a->setStatus("completed");
                        fh.updateAppointment(*a);
                        gui.showMessage("Appointment marked as completed.");
                    }
                }
            }
            break;
        }

        case 3: { // MARK NO-SHOW
            string list[100];
            int appIds[100];
            int count = 0;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getDoctorId() == d.getid() &&
                    appointments.getAll()[i].getDate() == today &&
                    appointments.getAll()[i].getStatus() == "pending") {
                    patient* p = patients.findById(appointments.getAll()[i].getPatientId());
                    list[count] = "ID:" + to_string(appointments.getAll()[i].getAppointmentId()) +
                        " | " + (p ? p->getname() : "Patient");
                    appIds[count++] = appointments.getAll()[i].getAppointmentId();
                }
            }
            if (count == 0) gui.showMessage("No pending appointments today.");
            else {
                int sel = gui.showSelectionList("Mark as No-Show", list, count);
                if (sel != -1) {
                    Appointment* a = appointments.findById(appIds[sel]);
                    if (a) {
                        a->setStatus("noshow");
                        fh.updateAppointment(*a);
                        for (int i = 0; i < bills.size(); i++) {
                            if (bills.getAll()[i].getAppointmentId() == appIds[sel]) {
                                bills.getAll()[i].setStatus("cancelled");
                                fh.updateBill(bills.getAll()[i]);
                                break;
                            }
                        }
                        gui.showMessage("Appointment marked as no-show.");
                    }
                }
            }
            break;
        }

        case 4: { // WRITE PRESCRIPTION
            string list[100];
            int appIds[100];
            int count = 0;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getDoctorId() == d.getid() &&
                    appointments.getAll()[i].getStatus() == "completed") {
                    // check no prescription exists
                    bool exists = false;
                    for (int j = 0; j < prescriptions.size(); j++) {
                        if (prescriptions.getAll()[j].getAppointmentId() ==
                            appointments.getAll()[i].getAppointmentId()) {
                            exists = true;
                            break;
                        }
                    }
                    if (!exists) {
                        list[count] = "Appt ID:" + to_string(appointments.getAll()[i].getAppointmentId()) +
                            " | " + appointments.getAll()[i].getDate();
                        appIds[count++] = appointments.getAll()[i].getAppointmentId();
                    }
                }
            }
            if (count == 0) gui.showMessage("No completed appointments without prescription.");
            else {
                int sel = gui.showSelectionList("Select Appointment", list, count);
                if (sel != -1) {
                    string meds = gui.getInput("Enter Medicines (e.g. Paracetamol 500mg):");
                    string notes = gui.getInput("Enter Notes:");
                    if (meds != "") {
                        int newId = 1;
                        for (int i = 0; i < prescriptions.size(); i++) {
                            if (prescriptions.getAll()[i].getPrescriptionId() >= newId)
                                newId = prescriptions.getAll()[i].getPrescriptionId() + 1;
                        }
                        Appointment* a = appointments.findById(appIds[sel]);
                        Prescription pr(newId, a->getAppointmentId(), a->getPatientId(), d.getid(), today, meds, notes);
                        prescriptions.add(pr);
                        fh.savePrescription(pr);
                        gui.showMessage("Prescription saved!");
                    }
                }
            }
            break;
        }

        case 5: { // VIEW PATIENT MEDICAL HISTORY
            string patIdStr = gui.getInput("Enter Patient ID:");
            if (patIdStr == "") break;

            int patId = 0;
            for (int i = 0; i < (int)patIdStr.size(); i++) {
                if (patIdStr[i] >= '0' && patIdStr[i] <= '9')
                    patId = patId * 10 + (patIdStr[i] - '0');
            }

            // check has completed appointment with this doctor
            bool hasAccess = false;
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getPatientId() == patId &&
                    appointments.getAll()[i].getDoctorId() == d.getid() &&
                    appointments.getAll()[i].getStatus() == "completed") {
                    hasAccess = true;
                    break;
                }
            }

            if (!hasAccess) {
                gui.showMessage("Access denied. Only your own patients.");
                break;
            }

            string history[100];
            int count = 0;

            Prescription myPres[100];
            int myCount = 0;
            for (int i = 0; i < prescriptions.size(); i++) {
                if (prescriptions.getAll()[i].getPatientId() == patId &&
                    prescriptions.getAll()[i].getDoctorId() == d.getid()) {
                    myPres[myCount++] = prescriptions.getAll()[i];
                }
            }

            // sort descending
            for (int i = 0; i < myCount - 1; i++) {
                for (int j = 0; j < myCount - i - 1; j++) {
                    if (myPres[j].getDate() < myPres[j + 1].getDate()) {
                        Prescription temp = myPres[j];
                        myPres[j] = myPres[j + 1];
                        myPres[j + 1] = temp;
                    }
                }
            }

            for (int i = 0; i < myCount; i++) {
                history[count++] = myPres[i].getDate() + " | " + myPres[i].getMedicines() + " | " + myPres[i].getNotes();
            }

            if (count == 0) gui.showMessage("No prescriptions found.");
            else gui.showSelectionList("Patient Medical History", history, count);
            break;
        }
        }
    }
}

void loginDoctor() {
    int failedAttempts = 0;
    while (failedAttempts < 3) {
        int id = 0;
        string password = "";

        bool submitted = gui.showLoginScreen("Doctor", id, password);
        if (!submitted) return;

        doctor* found = nullptr;
        for (int i = 0; i < doctors.size(); i++) {
            if (doctors.getAll()[i].getid() == id) {
                found = &doctors.getAll()[i];
                break;
            }
        }

        if (found != nullptr && found->getpassword() == password) {
            gui.showMessage("Login successful! Welcome Dr. " + found->getname());
            runDoctorMenu(*found);
            return;
        }
        else {
            failedAttempts++;
            string msg = "Invalid ID or password. Attempts left: ";
            msg += to_string(3 - failedAttempts);
            gui.showMessage(msg);
            string log = getCurrentDateTime() + ",Doctor," + to_string(id) + ",FAILED";
            fh.logSecurityEvent(log);
        }
    }
    gui.showMessage("Account locked. Contact admin.");
}

// ==================== ADMIN MENU ====================

void runAdminMenu() {
    while (true) {
        int choice = gui.showAdminMenu();

        if (choice == 10) {
            gui.showMessage("Admin logged out.");
            break;
        }

        switch (choice) {
        case 1: { // ADD DOCTOR
            string name = gui.getInput("Enter Doctor Name:");
            if (name == "") break;
            string spec = gui.getInput("Enter Specialization:");
            string contact = gui.getInput("Enter Contact (11 digits):");
            string password = gui.getInput("Enter Password (min 6 chars):");
            string feeStr = gui.getInput("Enter Consultation Fee:");

            float fee = 0;
            for (int i = 0; i < (int)feeStr.size(); i++) {
                if (feeStr[i] >= '0' && feeStr[i] <= '9')
                    fee = fee * 10 + (feeStr[i] - '0');
            }

            if (!Validator::isValidContact(contact)) {
                gui.showMessage("Invalid contact number.");
                break;
            }
            if (!Validator::isValidPassword(password)) {
                gui.showMessage("Password must be at least 6 characters.");
                break;
            }
            if (fee <= 0) {
                gui.showMessage("Invalid fee.");
                break;
            }

            int newId = 1;
            for (int i = 0; i < doctors.size(); i++) {
                if (doctors.getAll()[i].getid() >= newId)
                    newId = doctors.getAll()[i].getid() + 1;
            }

            doctor newDoc(newId, name, password, spec, contact, fee);
            doctors.add(newDoc);
            fh.saveDoctor(newDoc);
            gui.showMessage("Doctor added! ID: " + to_string(newId));
            break;
        }

        case 2: { // REMOVE DOCTOR
            string list[100];
            int docIds[100];
            int count = 0;
            for (int i = 0; i < doctors.size(); i++) {
                list[count] = "ID:" + to_string(doctors.getAll()[i].getid()) +
                    " | " + doctors.getAll()[i].getname() +
                    " | " + doctors.getAll()[i].getspec();
                docIds[count++] = doctors.getAll()[i].getid();
            }
            if (count == 0) {
                gui.showMessage("No doctors to remove.");
                break;
            }
            int sel = gui.showSelectionList("Select Doctor to Remove", list, count);
            if (sel != -1) {
                // check pending appointments
                bool hasPending = false;
                for (int i = 0; i < appointments.size(); i++) {
                    if (appointments.getAll()[i].getDoctorId() == docIds[sel] &&
                        appointments.getAll()[i].getStatus() == "pending") {
                        hasPending = true;
                        break;
                    }
                }
                if (hasPending) {
                    gui.showMessage("Cannot remove doctor with pending appointments.");
                }
                else {
                    doctors.removeById(docIds[sel]);
                    fh.deleteDoctor(docIds[sel]);
                    gui.showMessage("Doctor removed.");
                }
            }
            break;
        }

        case 3: { // VIEW ALL PATIENTS
            string list[100];
            int count = 0;
            for (int i = 0; i < patients.size(); i++) {
                patient& p = patients.getAll()[i];
                int unpaid = 0;
                for (int j = 0; j < bills.size(); j++) {
                    if (bills.getAll()[j].getPatientId() == p.getid() &&
                        bills.getAll()[j].getStatus() == "unpaid") {
                        unpaid++;
                    }
                }
                list[count++] = "ID:" + to_string(p.getid()) +
                    " | " + p.getname() +
                    " | Age:" + to_string(p.getage()) +
                    " | " + p.getgender() +
                    " | Bal:PKR" + to_string((int)p.getbalance()) +
                    " | Unpaid:" + to_string(unpaid);
            }
            if (count == 0) gui.showMessage("No patients registered.");
            else gui.showSelectionList("All Patients", list, count);
            break;
        }

        case 4: { // VIEW ALL DOCTORS
            string list[100];
            int count = 0;
            for (int i = 0; i < doctors.size(); i++) {
                doctor& d = doctors.getAll()[i];
                list[count++] = "ID:" + to_string(d.getid()) +
                    " | Dr. " + d.getname() +
                    " | " + d.getspec() +
                    " | Fee:PKR" + to_string((int)d.getfee());
            }
            if (count == 0) gui.showMessage("No doctors registered.");
            else gui.showSelectionList("All Doctors", list, count);
            break;
        }

        case 5: { // VIEW ALL APPOINTMENTS
            string list[100];
            int count = 0;

            Appointment allApps[100];
            int aCount = 0;
            for (int i = 0; i < appointments.size(); i++) {
                allApps[aCount++] = appointments.getAll()[i];
            }

            // sort by date descending
            for (int i = 0; i < aCount - 1; i++) {
                for (int j = 0; j < aCount - i - 1; j++) {
                    if (allApps[j].getDate() < allApps[j + 1].getDate()) {
                        Appointment temp = allApps[j];
                        allApps[j] = allApps[j + 1];
                        allApps[j + 1] = temp;
                    }
                }
            }

            for (int i = 0; i < aCount; i++) {
                patient* p = patients.findById(allApps[i].getPatientId());
                doctor* d = doctors.findById(allApps[i].getDoctorId());
                list[count++] = allApps[i].getDate() +
                    " | " + (p ? p->getname() : "Patient") +
                    " | Dr." + (d ? d->getname() : "Doc") +
                    " | " + allApps[i].getTimeSlot() +
                    " [" + allApps[i].getStatus() + "]";
            }
            if (count == 0) gui.showMessage("No appointments found.");
            else gui.showSelectionList("All Appointments", list, count);
            break;
        }

        case 6: { // VIEW UNPAID BILLS
            string list[100];
            int count = 0;
            string today = getCurrentDate();

            for (int i = 0; i < bills.size(); i++) {
                if (bills.getAll()[i].getStatus() == "unpaid") {
                    patient* p = patients.findById(bills.getAll()[i].getPatientId());
                    string overdue = "";

                    // check overdue
                    string dateStr = bills.getAll()[i].getDate();
                    if (dateStr.size() == 10) {
                        int bDay = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');
                        int bMonth = (dateStr[3] - '0') * 10 + (dateStr[4] - '0');
                        int bYear = (dateStr[6] - '0') * 1000 + (dateStr[7] - '0') * 100 + (dateStr[8] - '0') * 10 + (dateStr[9] - '0');

                        int tDay = (today[0] - '0') * 10 + (today[1] - '0');
                        int tMonth = (today[3] - '0') * 10 + (today[4] - '0');
                        int tYear = (today[6] - '0') * 1000 + (today[7] - '0') * 100 + (today[8] - '0') * 10 + (today[9] - '0');

                        tm billDate = {};
                        billDate.tm_mday = bDay;
                        billDate.tm_mon = bMonth - 1;
                        billDate.tm_year = bYear - 1900;
                        time_t billTime = mktime(&billDate);

                        tm todayDate = {};
                        todayDate.tm_mday = tDay;
                        todayDate.tm_mon = tMonth - 1;
                        todayDate.tm_year = tYear - 1900;
                        time_t todayTime = mktime(&todayDate);

                        double diff = difftime(todayTime, billTime);
                        if (diff > 7 * 24 * 60 * 60) overdue = " [OVERDUE]";
                    }

                    list[count++] = "Bill:" + to_string(bills.getAll()[i].getBillId()) +
                        " | " + (p ? p->getname() : "Patient") +
                        " | PKR " + to_string((int)bills.getAll()[i].getAmount()) +
                        " | " + bills.getAll()[i].getDate() + overdue;
                }
            }
            if (count == 0) gui.showMessage("No unpaid bills.");
            else gui.showSelectionList("Unpaid Bills", list, count);
            break;
        }

        case 7: { // DISCHARGE PATIENT
            string idStr = gui.getInput("Enter Patient ID to Discharge:");
            if (idStr == "") break;

            int pId = 0;
            for (int i = 0; i < (int)idStr.size(); i++) {
                if (idStr[i] >= '0' && idStr[i] <= '9')
                    pId = pId * 10 + (idStr[i] - '0');
            }

            patient* p = patients.findById(pId);
            if (!p) {
                gui.showMessage("Patient not found.");
                break;
            }

            // check unpaid bills
            for (int i = 0; i < bills.size(); i++) {
                if (bills.getAll()[i].getPatientId() == pId &&
                    bills.getAll()[i].getStatus() == "unpaid") {
                    gui.showMessage("Cannot discharge. Patient has unpaid bills.");
                    goto skipDischarge;
                }
            }

            // check pending appointments
            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getPatientId() == pId &&
                    appointments.getAll()[i].getStatus() == "pending") {
                    gui.showMessage("Cannot discharge. Patient has pending appointments.");
                    goto skipDischarge;
                }
            }

            fh.saveDischargedPatient(*p);
            fh.deletePatient(pId);
            patients.removeById(pId);
            gui.showMessage("Patient discharged and archived.");

        skipDischarge:;
            break;
        }

        case 8: { // VIEW SECURITY LOG
            ifstream file("security_log.txt");
            string list[100];
            int count = 0;
            string line;
            while (getline(file, line)) {
                if (!line.empty()) {
                    list[count++] = line;
                }
            }
            file.close();
            if (count == 0) gui.showMessage("No security events logged.");
            else gui.showSelectionList("Security Log", list, count);
            break;
        }

        case 9: { // GENERATE DAILY REPORT
            string today = getCurrentDate();
            int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
            float revenue = 0;

            for (int i = 0; i < appointments.size(); i++) {
                if (appointments.getAll()[i].getDate() == today) {
                    total++;
                    string s = appointments.getAll()[i].getStatus();
                    if (s == "pending") pending++;
                    else if (s == "completed") completed++;
                    else if (s == "noshow") noshow++;
                    else if (s == "cancelled") cancelled++;
                }
            }

            for (int i = 0; i < bills.size(); i++) {
                if (bills.getAll()[i].getDate() == today &&
                    bills.getAll()[i].getStatus() == "paid") {
                    revenue += bills.getAll()[i].getAmount();
                }
            }

            string report =
                "Date: " + today +
                "\nTotal Appointments: " + to_string(total) +
                "\nPending: " + to_string(pending) +
                "\nCompleted: " + to_string(completed) +
                "\nNo-show: " + to_string(noshow) +
                "\nCancelled: " + to_string(cancelled) +
                "\nRevenue Today: PKR " + to_string((int)revenue);

            gui.showMessage(report);
            break;
        }
        }
    }
}

void loginAdmin() {
    int failedAttempts = 0;
    while (failedAttempts < 3) {
        int id = 0;
        string password = "";

        bool submitted = gui.showLoginScreen("Admin", id, password);
        if (!submitted) return;

        if (admin.getid() == id && admin.getpassword() == password) {
            gui.showMessage("Login successful! Welcome Admin.");
            runAdminMenu();
            return;
        }
        else {
            failedAttempts++;
            string msg = "Invalid ID or password. Attempts left: ";
            msg += to_string(3 - failedAttempts);
            gui.showMessage(msg);
            string log = getCurrentDateTime() + ",Admin," + to_string(id) + ",FAILED";
            fh.logSecurityEvent(log);
        }
    }
    gui.showMessage("Account locked. Contact admin.");
}

// ==================== RUN SYSTEM ====================

void runSystem() {
    gui.showSplashScreen();

    int choice = 0;
    while (true) {
        choice = gui.showLoginMenu();

        switch (choice) {
        case 1: loginPatient(); break;
        case 2: loginDoctor();  break;
        case 3: loginAdmin();   break;
        case 4:
            gui.showMessage("Thank you for using MediCore. Goodbye!");
            return;
        }
    }
}

// ==================== MAIN ====================

int main() {
    createFilesIfNotExist();
    loadAllData();
    runSystem();
    return 0;
}