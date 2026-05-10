#include "Admin.h"
#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"
#include <ctime>
#include <fstream>

// constructors
Admin::Admin() : person() {}

Admin::Admin(int id, string name, string password)
    : person(id, name, password) {
}

// helper to get today's date
string getAdminTodayDate() {
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    string day = (now.tm_mday < 10 ? "0" : "") + to_string(now.tm_mday);
    string month = ((now.tm_mon + 1) < 10 ? "0" : "") + to_string(now.tm_mon + 1);
    string year = to_string(now.tm_year + 1900);
    return day + "-" + month + "-" + year;
}

// menus
void Admin::displayMenu() {
    cout << "\nAdmin Panel - MediCore" << endl;
    cout << "======================" << endl;
    cout << "1. Add Doctor" << endl;
    cout << "2. Remove Doctor" << endl;
    cout << "3. View All Patients" << endl;
    cout << "4. View All Doctors" << endl;
    cout << "5. View All Appointments" << endl;
    cout << "6. View Unpaid Bills" << endl;
    cout << "7. Discharge Patient" << endl;
    cout << "8. View Security Log" << endl;
    cout << "9. Generate Daily Report" << endl;
    cout << "10. Logout" << endl;
}

void Admin::display() const {
    cout << "ID: " << ID << " | Name: " << name << endl;
}

// ==================== ADD DOCTOR ====================

void Admin::addDoctor(Storage<doctor>& doctors, FileHandler& fh) {
    string name, specialization, contact, password;
    float fee;

    cout << "Enter doctor name: ";
    cin.ignore();
    getline(cin, name);

    cout << "Enter specialization: ";
    getline(cin, specialization);

    while (true) {
        cout << "Enter contact (11 digits): ";
        cin >> contact;
        if (Validator::isValidContact(contact)) break;
        cout << "Invalid contact. Must be exactly 11 digits." << endl;
    }

    while (true) {
        cout << "Enter password (min 6 chars): ";
        cin >> password;
        if (Validator::isValidPassword(password)) break;
        cout << "Invalid password. Must be at least 6 characters." << endl;
    }

    while (true) {
        cout << "Enter consultation fee: ";
        cin >> fee;
        if (Validator::isValidFloat(fee)) break;
        cout << "Invalid fee. Must be greater than 0." << endl;
    }

    // generate new doctor ID
    int newId = 1;
    for (int i = 0; i < doctors.size(); i++) {
        if (doctors.getAll()[i].getid() >= newId) {
            newId = doctors.getAll()[i].getid() + 1;
        }
    }

    doctor newDoc(newId, name, password, specialization, contact, fee);
    doctors.add(newDoc);
    fh.saveDoctor(newDoc);
    cout << "Doctor added successfully. ID: " << newId << endl;
}

// ==================== REMOVE DOCTOR ====================

void Admin::removeDoctor(Storage<doctor>& doctors, Storage<Appointment>& appointments, FileHandler& fh) {
    cout << "\n--- All Doctors ---" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        doctors.getAll()[i].display();
    }

    int docId;
    cout << "Enter Doctor ID to remove: ";
    cin >> docId;

    // check pending appointments
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getDoctorId() == docId &&
            appointments.getAll()[i].getStatus() == "pending") {
            cout << "Cannot remove doctor with pending appointments." << endl;
            return;
        }
    }

    doctors.removeById(docId);
    fh.deleteDoctor(docId);
    cout << "Doctor removed." << endl;
}

// ==================== VIEW ALL PATIENTS ====================

void Admin::viewAllPatients(Storage<patient>& patients, Storage<Bill>& bills) {
    cout << "\n--- All Patients ---" << endl;
    for (int i = 0; i < patients.size(); i++) {
        patient& p = patients.getAll()[i];

        int unpaidCount = 0;
        for (int j = 0; j < bills.size(); j++) {
            if (bills.getAll()[j].getPatientId() == p.getid() &&
                bills.getAll()[j].getStatus() == "unpaid") {
                unpaidCount++;
            }
        }

        cout << "ID: " << p.getid()
            << " | Name: " << p.getname()
            << " | Age: " << p.getage()
            << " | Gender: " << p.getgender()
            << " | Contact: " << p.getcontact()
            << " | Balance: PKR " << p.getbalance()
            << " | Unpaid Bills: " << unpaidCount << endl;
    }
}

// ==================== VIEW ALL DOCTORS ====================

void Admin::viewAllDoctors(Storage<doctor>& doctors) {
    cout << "\n--- All Doctors ---" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        doctor& d = doctors.getAll()[i];
        cout << "ID: " << d.getid()
            << " | Name: " << d.getname()
            << " | Spec: " << d.getspec()
            << " | Contact: " << d.getcontact()
            << " | Fee: PKR " << d.getfee() << endl;
    }
}

// ==================== VIEW ALL APPOINTMENTS ====================

void Admin::viewAllAppointments(Storage<Appointment>& appointments, Storage<patient>& patients, Storage<doctor>& doctors) {
    cout << "\n--- All Appointments ---" << endl;

    Appointment allApps[100];
    int count = 0;
    for (int i = 0; i < appointments.size(); i++) {
        allApps[count++] = appointments.getAll()[i];
    }

    // bubble sort by date descending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (allApps[j].getDate() < allApps[j + 1].getDate()) {
                Appointment temp = allApps[j];
                allApps[j] = allApps[j + 1];
                allApps[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        string patName = "Unknown";
        string docName = "Unknown";

        for (int j = 0; j < patients.size(); j++) {
            if (patients.getAll()[j].getid() == allApps[i].getPatientId()) {
                patName = patients.getAll()[j].getname();
                break;
            }
        }
        for (int j = 0; j < doctors.size(); j++) {
            if (doctors.getAll()[j].getid() == allApps[i].getDoctorId()) {
                docName = doctors.getAll()[j].getname();
                break;
            }
        }

        cout << "ID: " << allApps[i].getAppointmentId()
            << " | Patient: " << patName
            << " | Doctor: " << docName
            << " | Date: " << allApps[i].getDate()
            << " | Slot: " << allApps[i].getTimeSlot()
            << " | Status: " << allApps[i].getStatus() << endl;
    }
}

// ==================== VIEW UNPAID BILLS ====================

void Admin::viewUnpaidBills(Storage<Bill>& bills, Storage<patient>& patients) {
    cout << "\n--- Unpaid Bills ---" << endl;
    string today = getAdminTodayDate();
    bool found = false;

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getStatus() == "unpaid") {
            string patName = "Unknown";
            for (int j = 0; j < patients.size(); j++) {
                if (patients.getAll()[j].getid() == b.getPatientId()) {
                    patName = patients.getAll()[j].getname();
                    break;
                }
            }

            // check overdue using difftime
            string dateStr = b.getDate();
            int bDay = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');
            int bMonth = (dateStr[3] - '0') * 10 + (dateStr[4] - '0');
            int bYear = (dateStr[6] - '0') * 1000 + (dateStr[7] - '0') * 100 +
                (dateStr[8] - '0') * 10 + (dateStr[9] - '0');

            int tDay = (today[0] - '0') * 10 + (today[1] - '0');
            int tMonth = (today[3] - '0') * 10 + (today[4] - '0');
            int tYear = (today[6] - '0') * 1000 + (today[7] - '0') * 100 +
                (today[8] - '0') * 10 + (today[9] - '0');

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
            string overdueTag = "";
            if (diff > 7 * 24 * 60 * 60) {
                overdueTag = " [OVERDUE]";
            }

            cout << "Bill ID: " << b.getBillId()
                << " | Patient: " << patName
                << " | Amount: PKR " << b.getAmount()
                << " | Date: " << b.getDate() << overdueTag << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No unpaid bills." << endl;
    }
}

// ==================== DISCHARGE PATIENT ====================

void Admin::dischargePatient(Storage<patient>& patients, Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<Prescription>& prescriptions, FileHandler& fh) {
    int patId;
    cout << "Enter Patient ID: ";
    cin >> patId;

    patient* pat = nullptr;
    for (int i = 0; i < patients.size(); i++) {
        if (patients.getAll()[i].getid() == patId) {
            pat = &patients.getAll()[i];
            break;
        }
    }

    if (pat == nullptr) {
        cout << "Patient not found." << endl;
        return;
    }

    // check unpaid bills
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getPatientId() == patId &&
            bills.getAll()[i].getStatus() == "unpaid") {
            cout << "Cannot discharge patient with unpaid bills." << endl;
            return;
        }
    }

    // check pending appointments
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getPatientId() == patId &&
            appointments.getAll()[i].getStatus() == "pending") {
            cout << "Cannot discharge patient with pending appointments." << endl;
            return;
        }
    }

    fh.saveDischargedPatient(*pat);
    fh.deletePatient(patId);
    patients.removeById(patId);
    cout << "Patient discharged and archived successfully." << endl;
}

// ==================== VIEW SECURITY LOG ====================

void Admin::viewSecurityLog() {
    ifstream file("security_log.txt");
    if (!file.is_open()) {
        cout << "No security events logged." << endl;
        return;
    }

    string line;
    bool empty = true;
    cout << "\n--- Security Log ---" << endl;
    while (getline(file, line)) {
        if (!line.empty()) {
            cout << line << endl;
            empty = false;
        }
    }

    if (empty) {
        cout << "No security events logged." << endl;
    }
    file.close();
}

// ==================== GENERATE DAILY REPORT ====================

void Admin::generateDailyReport(Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<patient>& patients, Storage<doctor>& doctors) {
    string today = getAdminTodayDate();
    cout << "\n--- Daily Report: " << today << " ---" << endl;

    // count appointments by status
    int total = 0, pending = 0, completed = 0, noshow = 0, cancelled = 0;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getDate() == today) {
            total++;
            string s = appointments.getAll()[i].getStatus();
            if (s == "pending")        pending++;
            else if (s == "completed") completed++;
            else if (s == "noshow")    noshow++;
            else if (s == "cancelled") cancelled++;
        }
    }

    cout << "Total appointments today: " << total
        << " (Pending: " << pending
        << " Completed: " << completed
        << " No-show: " << noshow
        << " Cancelled: " << cancelled << ")" << endl;

    // revenue
    float revenue = 0;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getDate() == today &&
            bills.getAll()[i].getStatus() == "paid") {
            revenue += bills.getAll()[i].getAmount();
        }
    }
    cout << "Revenue collected today: PKR " << revenue << endl;

    // patients with unpaid bills
    cout << "\nPatients with unpaid bills:" << endl;
    bool anyUnpaid = false;
    for (int i = 0; i < patients.size(); i++) {
        float totalOwed = 0;
        for (int j = 0; j < bills.size(); j++) {
            if (bills.getAll()[j].getPatientId() == patients.getAll()[i].getid() &&
                bills.getAll()[j].getStatus() == "unpaid") {
                totalOwed += bills.getAll()[j].getAmount();
            }
        }
        if (totalOwed > 0) {
            cout << "Patient: " << patients.getAll()[i].getname()
                << " | Total Owed: PKR " << totalOwed << endl;
            anyUnpaid = true;
        }
    }
    if (!anyUnpaid) cout << "None." << endl;

    // doctor wise summary
    cout << "\nDoctor-wise summary for today:" << endl;
    for (int i = 0; i < doctors.size(); i++) {
        int dCompleted = 0, dPending = 0, dNoshow = 0;
        for (int j = 0; j < appointments.size(); j++) {
            if (appointments.getAll()[j].getDoctorId() == doctors.getAll()[i].getid() &&
                appointments.getAll()[j].getDate() == today) {
                string s = appointments.getAll()[j].getStatus();
                if (s == "completed")  dCompleted++;
                else if (s == "pending") dPending++;
                else if (s == "noshow")  dNoshow++;
            }
        }
        cout << "Dr. " << doctors.getAll()[i].getname()
            << " | Completed: " << dCompleted
            << " | Pending: " << dPending
            << " | No-show: " << dNoshow << endl;
    }
}