#include "Patient.h"
#include "Doctor.h"
#include "Appointment.h"
#include "Bill.h"
#include "Prescription.h"
#include "Storage.h"
#include "FileHandler.h"

// constructors
patient::patient() : person() {
    age = 0;
    gender = "";
    contact = "";
    balance = 0.0;
}

patient::patient(int id, string name, string password, int age, string gender, string contact, float balance)
    : person(id, name, password) {
    this->age = age;
    this->gender = gender;
    this->contact = contact;
    this->balance = balance;
}

// getters
int patient::getage() const { return age; }
string patient::getgender() const { return gender; }
string patient::getcontact() const { return contact; }
float patient::getbalance() const { return balance; }

// setters
void patient::setbalance(float amount) { balance = amount; }

// operator overloads
void patient::operator+=(float amount) { balance += amount; }
void patient::operator-=(float amount) { balance -= amount; }

bool patient::operator==(patient& other) {
    return this->ID == other.ID;
}

ostream& operator<<(ostream& out, patient& p) {
    out << "ID: " << p.ID << endl;
    out << "Name: " << p.name << endl;
    out << "Age: " << p.age << endl;
    out << "Gender: " << p.gender << endl;
    out << "Contact: " << p.contact << endl;
    out << "Balance: PKR " << p.balance << endl;
    return out;
}

// display menu
void patient::displayMenu() {
    cout << "\nWELCOME " << name << endl;
    cout << "Balance: PKR " << balance << endl;
    cout << "-----------------------------------" << endl;
    cout << "1. Book Appointment" << endl;
    cout << "2. Cancel Appointment" << endl;
    cout << "3. View My Appointments" << endl;
    cout << "4. View My Medical Records" << endl;
    cout << "5. View My Bills" << endl;
    cout << "6. Pay Bill" << endl;
    cout << "7. Top Up Balance" << endl;
    cout << "8. Logout" << endl;
}

void patient::display() const {
    cout << "ID: " << ID << " Name: " << name << " Balance: PKR " << balance << endl;
}

// ==================== TOP UP BALANCE ====================

void patient::topUpBalance(FileHandler& fh) {
    int attempts = 0;
    while (attempts < 3) {
        float amount;
        cout << "Enter amount to add (PKR): ";
        cin >> amount;
        try {
            if (!Validator::isValidFloat(amount)) {
                throw InvalidInputException("Amount must be greater than 0.");
            }
            balance += amount;
            fh.updatePatient(*this);
            cout << "Balance updated. New balance: PKR " << balance << endl;
            return;
        }
        catch (InvalidInputException& e) {
            cout << e.what() << endl;
            attempts++;
        }
    }
    cout << "Too many invalid attempts. Returning to menu." << endl;
}

// ==================== VIEW BILLS ====================

void patient::viewBills(Storage<Bill>& bills) {
    cout << "\n--- My Bills ---" << endl;
    bool found = false;
    float totalUnpaid = 0;

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == ID) {
            cout << "Bill ID: " << b.getBillId()
                << " | Appointment ID: " << b.getAppointmentId()
                << " | Amount: PKR " << b.getAmount()
                << " | Status: " << b.getStatus()
                << " | Date: " << b.getDate() << endl;
            found = true;
            if (b.getStatus() == "unpaid") {
                totalUnpaid += b.getAmount();
            }
        }
    }

    if (!found) {
        cout << "No bills found." << endl;
    }
    else {
        cout << "Total outstanding unpaid: PKR " << totalUnpaid << endl;
    }
}

// ==================== VIEW APPOINTMENTS ====================

void patient::viewAppointments(Storage<Appointment>& appointments, Storage<doctor>& doctors) {
    cout << "\n--- My Appointments ---" << endl;

    Appointment myApps[100];
    int count = 0;

    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getPatientId() == ID) {
            myApps[count++] = appointments.getAll()[i];
        }
    }

    if (count == 0) {
        cout << "No appointments found." << endl;
        return;
    }

    // bubble sort by date ascending
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (myApps[j].getDate() > myApps[j + 1].getDate()) {
                Appointment temp = myApps[j];
                myApps[j] = myApps[j + 1];
                myApps[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < count; i++) {
        string docName = "Unknown";
        string docSpec = "Unknown";
        for (int j = 0; j < doctors.size(); j++) {
            if (doctors.getAll()[j].getid() == myApps[i].getDoctorId()) {
                docName = doctors.getAll()[j].getname();
                docSpec = doctors.getAll()[j].getspec();
                break;
            }
        }
        cout << "ID: " << myApps[i].getAppointmentId()
            << " | Doctor: " << docName
            << " | Spec: " << docSpec
            << " | Date: " << myApps[i].getDate()
            << " | Slot: " << myApps[i].getTimeSlot()
            << " | Status: " << myApps[i].getStatus() << endl;
    }
}

// ==================== VIEW MEDICAL RECORDS ====================

void patient::viewMedicalRecords(Storage<Prescription>& prescriptions, Storage<doctor>& doctors) {
    cout << "\n--- My Medical Records ---" << endl;

    Prescription myPres[100];
    int count = 0;

    for (int i = 0; i < prescriptions.size(); i++) {
        if (prescriptions.getAll()[i].getPatientId() == ID) {
            myPres[count++] = prescriptions.getAll()[i];
        }
    }

    if (count == 0) {
        cout << "No medical records found." << endl;
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

    for (int i = 0; i < count; i++) {
        string docName = "Unknown";
        for (int j = 0; j < doctors.size(); j++) {
            if (doctors.getAll()[j].getid() == myPres[i].getDoctorId()) {
                docName = doctors.getAll()[j].getname();
                break;
            }
        }
        cout << "Date: " << myPres[i].getDate()
            << " | Doctor: " << docName
            << " | Medicines: " << myPres[i].getMedicines()
            << " | Notes: " << myPres[i].getNotes() << endl;
    }
}

// ==================== PAY BILL ====================

void patient::payBill(Storage<Bill>& bills, FileHandler& fh) {
    cout << "\n--- Unpaid Bills ---" << endl;
    bool found = false;

    for (int i = 0; i < bills.size(); i++) {
        Bill& b = bills.getAll()[i];
        if (b.getPatientId() == ID && b.getStatus() == "unpaid") {
            cout << "Bill ID: " << b.getBillId()
                << " | Amount: PKR " << b.getAmount()
                << " | Date: " << b.getDate() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "No unpaid bills." << endl;
        return;
    }

    int billId;
    cout << "Enter Bill ID to pay: ";
    cin >> billId;

    Bill* bill = nullptr;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getBillId() == billId &&
            bills.getAll()[i].getPatientId() == ID &&
            bills.getAll()[i].getStatus() == "unpaid") {
            bill = &bills.getAll()[i];
            break;
        }
    }

    if (bill == nullptr) {
        cout << "Invalid Bill ID." << endl;
        return;
    }

    try {
        if (balance < bill->getAmount()) {
            throw InsufficientFundsException("Insufficient balance to pay this bill.");
        }
        balance -= bill->getAmount();
        bill->setStatus("paid");
        fh.updateBill(*bill);
        fh.updatePatient(*this);
        cout << "Bill paid successfully. Remaining balance: PKR " << balance << endl;
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
    }
}

// ==================== CANCEL APPOINTMENT ====================

void patient::cancelAppointment(Storage<Appointment>& appointments, Storage<Bill>& bills, Storage<doctor>& doctors, FileHandler& fh) {
    cout << "\n--- Pending Appointments ---" << endl;
    bool found = false;

    for (int i = 0; i < appointments.size(); i++) {
        Appointment& a = appointments.getAll()[i];
        if (a.getPatientId() == ID && a.getStatus() == "pending") {
            string docName = "Unknown";
            for (int j = 0; j < doctors.size(); j++) {
                if (doctors.getAll()[j].getid() == a.getDoctorId()) {
                    docName = doctors.getAll()[j].getname();
                    break;
                }
            }
            cout << "ID: " << a.getAppointmentId()
                << " | Doctor: " << docName
                << " | Date: " << a.getDate()
                << " | Slot: " << a.getTimeSlot() << endl;
            found = true;
        }
    }

    if (!found) {
        cout << "You have no pending appointments." << endl;
        return;
    }

    int appId;
    cout << "Enter Appointment ID to cancel: ";
    cin >> appId;

    Appointment* app = nullptr;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getAppointmentId() == appId &&
            appointments.getAll()[i].getPatientId() == ID &&
            appointments.getAll()[i].getStatus() == "pending") {
            app = &appointments.getAll()[i];
            break;
        }
    }

    if (app == nullptr) {
        cout << "Invalid appointment ID." << endl;
        return;
    }

    // get doctor fee for refund
    float fee = 0;
    for (int i = 0; i < doctors.size(); i++) {
        if (doctors.getAll()[i].getid() == app->getDoctorId()) {
            fee = doctors.getAll()[i].getfee();
            break;
        }
    }

    // cancel appointment and refund
    app->setStatus("cancelled");
    fh.updateAppointment(*app);
    balance += fee;
    fh.updatePatient(*this);

    // cancel bill
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getAppointmentId() == appId) {
            bills.getAll()[i].setStatus("cancelled");
            fh.updateBill(bills.getAll()[i]);
            break;
        }
    }

    cout << "Appointment cancelled. PKR " << fee << " refunded to your balance." << endl;
}

// ==================== BOOK APPOINTMENT ====================

void patient::bookAppointment(Storage<doctor>& doctors, Storage<Appointment>& appointments, Storage<Bill>& bills, FileHandler& fh) {
    // step 1 - search by specialization
    string spec;
    cout << "Enter specialization to search: ";
    cin >> spec;

    // convert to lowercase manually
    for (int i = 0; i < (int)spec.length(); i++) {
        if (spec[i] >= 'A' && spec[i] <= 'Z') {
            spec[i] = spec[i] + 32;
        }
    }

    // show matching doctors
    bool docFound = false;
    for (int i = 0; i < doctors.size(); i++) {
        string docSpec = doctors.getAll()[i].getspec();
        for (int j = 0; j < (int)docSpec.length(); j++) {
            if (docSpec[j] >= 'A' && docSpec[j] <= 'Z') {
                docSpec[j] = docSpec[j] + 32;
            }
        }
        if (docSpec == spec) {
            cout << "ID: " << doctors.getAll()[i].getid()
                << " | Name: " << doctors.getAll()[i].getname()
                << " | Fee: PKR " << doctors.getAll()[i].getfee() << endl;
            docFound = true;
        }
    }

    if (!docFound) {
        cout << "No doctors available for that specialization." << endl;
        return;
    }

    // step 2 - select doctor
    int docId;
    cout << "Enter Doctor ID: ";
    cin >> docId;

    doctor* selectedDoc = nullptr;
    for (int i = 0; i < doctors.size(); i++) {
        if (doctors.getAll()[i].getid() == docId) {
            selectedDoc = &doctors.getAll()[i];
            break;
        }
    }

    if (selectedDoc == nullptr) {
        cout << "Doctor not found." << endl;
        return;
    }

    // step 3 - enter date
    string date;
    int dateAttempts = 0;
    while (dateAttempts < 3) {
        cout << "Enter date (DD-MM-YYYY): ";
        cin >> date;
        if (Validator::isValidDate(date)) {
            break;
        }
        cout << "Invalid date. Use format DD-MM-YYYY." << endl;
        dateAttempts++;
        if (dateAttempts == 3) {
            cout << "Too many invalid attempts. Returning to menu." << endl;
            return;
        }
    }

    // step 4 - show available slots
    string allSlots[8] = { "09:00", "10:00", "11:00", "12:00", "13:00", "14:00", "15:00", "16:00" };
    cout << "Available slots: ";
    for (int i = 0; i < 8; i++) {
        bool taken = false;
        for (int j = 0; j < appointments.size(); j++) {
            Appointment& a = appointments.getAll()[j];
            if (a.getDoctorId() == docId &&
                a.getDate() == date &&
                a.getTimeSlot() == allSlots[i] &&
                a.getStatus() != "cancelled") {
                taken = true;
                break;
            }
        }
        if (!taken) {
            cout << allSlots[i] << " ";
        }
    }
    cout << endl;

    // step 5 - select slot
    string slot;
    while (true) {
        cout << "Enter time slot: ";
        cin >> slot;

        if (!Validator::isValidTimeSlot(slot)) {
            cout << "Invalid slot." << endl;
            continue;
        }

        try {
            bool slotTaken = false;
            for (int i = 0; i < appointments.size(); i++) {
                Appointment& a = appointments.getAll()[i];
                if (a.getDoctorId() == docId &&
                    a.getDate() == date &&
                    a.getTimeSlot() == slot &&
                    a.getStatus() != "cancelled") {
                    slotTaken = true;
                    break;
                }
            }
            if (slotTaken) {
                throw SlotUnavailableException("This slot is already booked. Choose another.");
            }
            break;
        }
        catch (SlotUnavailableException& e) {
            cout << e.what() << endl;
        }
    }

    // step 6 - check balance
    try {
        if (balance < selectedDoc->getfee()) {
            throw InsufficientFundsException("Insufficient balance. Please top up.");
        }
    }
    catch (InsufficientFundsException& e) {
        cout << e.what() << endl;
        return;
    }

    // step 7 - deduct and book
    balance -= selectedDoc->getfee();

    // generate appointment ID
    int newAppId = 1;
    for (int i = 0; i < appointments.size(); i++) {
        if (appointments.getAll()[i].getAppointmentId() >= newAppId) {
            newAppId = appointments.getAll()[i].getAppointmentId() + 1;
        }
    }

    Appointment newApp(newAppId, ID, docId, date, slot, "pending");
    appointments.add(newApp);
    fh.saveAppointment(newApp);

    // generate bill ID
    int newBillId = 1;
    for (int i = 0; i < bills.size(); i++) {
        if (bills.getAll()[i].getBillId() >= newBillId) {
            newBillId = bills.getAll()[i].getBillId() + 1;
        }
    }

    Bill newBill(newBillId, ID, newAppId, selectedDoc->getfee(), "unpaid", date);
    bills.add(newBill);
    fh.saveBill(newBill);
    fh.updatePatient(*this);

    cout << "Appointment booked successfully. Appointment ID: " << newAppId << endl;
}