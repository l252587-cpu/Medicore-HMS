#include "FileHandler.h"

const string BASE_PATH = "";

// ==================== HELPER FUNCTIONS ====================

void FileHandler::splitLine(string line, string parts[], int& count) {
    count = 0;
    string current = "";
    for (int i = 0; i <= (int)line.length(); i++) {
        if (line[i] == ',' || line[i] == '\0') {
            parts[count] = current;
            count++;
            current = "";
        }
        else {
            current += line[i];
        }
    }
}

int FileHandler::stringToInt(string s) {
    int result = 0;
    int i = 0;
    while (s[i] == ' ') i++;
    for (; s[i] != '\0'; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            result = result * 10 + (s[i] - '0');
        }
    }
    return result;
}

float FileHandler::stringToFloat(string s) {
    float result = 0;
    int i = 0;
    bool afterDot = false;
    float place = 0.1f;
    while (s[i] == ' ') i++;
    for (; s[i] != '\0'; i++) {
        if (s[i] == '.') {
            afterDot = true;
        }
        else if (s[i] >= '0' && s[i] <= '9') {
            if (!afterDot) {
                result = result * 10 + (s[i] - '0');
            }
            else {
                result += (s[i] - '0') * place;
                place *= 0.1f;
            }
        }
    }
    return result;
}

// ==================== LOADING FUNCTIONS ====================

void FileHandler::loadPatients(Storage<patient>& patients) {
    ifstream file(BASE_PATH + "patients.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("Error: patients.txt could not be opened.");
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string parts[7];
        int count = 0;
        splitLine(line, parts, count);
        if (count == 7) {
            int id = stringToInt(parts[0]);
            string name = parts[1];
            int age = stringToInt(parts[2]);
            string gender = parts[3];
            string contact = parts[4];
            string password = parts[5];
            float balance = stringToFloat(parts[6]);
            patient p(id, name, password, age, gender, contact, balance);
            patients.add(p);
        }
    }
    file.close();
}

void FileHandler::loadDoctors(Storage<doctor>& doctors) {
    ifstream file(BASE_PATH + "doctors.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("Error: doctors.txt could not be opened.");
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string parts[6];
        int count = 0;
        splitLine(line, parts, count);
        if (count == 6) {
            int id = stringToInt(parts[0]);
            string name = parts[1];
            string specialization = parts[2];
            string contact = parts[3];
            string password = parts[4];
            float fee = stringToFloat(parts[5]);
            doctor d(id, name, password, specialization, contact, fee);
            doctors.add(d);
        }
    }
    file.close();
}

void FileHandler::loadAdmin(Admin& admin) {
    ifstream file(BASE_PATH + "admin.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("Error: admin.txt could not be opened.");
    }
    string line;
    getline(file, line);
    if (!line.empty()) {
        string parts[3];
        int count = 0;
        splitLine(line, parts, count);
        if (count == 3) {
            int id = stringToInt(parts[0]);
            string name = parts[1];
            string password = parts[2];
            admin = Admin(id, name, password);
        }
    }
    file.close();
}

void FileHandler::loadAppointments(Storage<Appointment>& appointments) {
    ifstream file(BASE_PATH + "appointments.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("Error: appointments.txt could not be opened.");
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string parts[6];
        int count = 0;
        splitLine(line, parts, count);
        if (count == 6) {
            int appointmentId = stringToInt(parts[0]);
            int patientId = stringToInt(parts[1]);
            int doctorId = stringToInt(parts[2]);
            string date = parts[3];
            string timeSlot = parts[4];
            string status = parts[5];
            Appointment a(appointmentId, patientId, doctorId, date, timeSlot, status);
            appointments.add(a);
        }
    }
    file.close();
}

void FileHandler::loadBills(Storage<Bill>& bills) {
    ifstream file(BASE_PATH + "bills.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("Error: bills.txt could not be opened.");
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string parts[6];
        int count = 0;
        splitLine(line, parts, count);
        if (count == 6) {
            int billId = stringToInt(parts[0]);
            int patientId = stringToInt(parts[1]);
            int appointmentId = stringToInt(parts[2]);
            float amount = stringToFloat(parts[3]);
            string status = parts[4];
            string date = parts[5];
            Bill b(billId, patientId, appointmentId, amount, status, date);
            bills.add(b);
        }
    }
    file.close();
}

void FileHandler::loadPrescriptions(Storage<Prescription>& prescriptions) {
    ifstream file(BASE_PATH + "prescriptions.txt");
    if (!file.is_open()) {
        throw FileNotFoundException("Error: prescriptions.txt could not be opened.");
    }
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        string parts[7];
        int count = 0;
        splitLine(line, parts, count);
        if (count == 7) {
            int prescriptionId = stringToInt(parts[0]);
            int appointmentId = stringToInt(parts[1]);
            int patientId = stringToInt(parts[2]);
            int doctorId = stringToInt(parts[3]);
            string date = parts[4];
            string medicines = parts[5];
            string notes = parts[6];
            Prescription p(prescriptionId, appointmentId, patientId, doctorId, date, medicines, notes);
            prescriptions.add(p);
        }
    }
    file.close();
}

// ==================== SAVING FUNCTIONS ====================

void FileHandler::savePatient(patient& p) {
    ofstream file(BASE_PATH + "patients.txt", ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Error: patients.txt could not be opened.");
    }
    file << p.getid() << ","
        << p.getname() << ","
        << p.getage() << ","
        << p.getgender() << ","
        << p.getcontact() << ","
        << p.getpassword() << ","
        << p.getbalance() << "\n";
    file.close();
}

void FileHandler::saveDoctor(doctor& d) {
    ofstream file(BASE_PATH + "doctors.txt", ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Error: doctors.txt could not be opened.");
    }
    file << d.getid() << ","
        << d.getname() << ","
        << d.getspec() << ","
        << d.getcontact() << ","
        << d.getpassword() << ","
        << d.getfee() << "\n";
    file.close();
}

void FileHandler::saveAppointment(Appointment& a) {
    ofstream file(BASE_PATH + "appointments.txt", ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Error: appointments.txt could not be opened.");
    }
    file << a.getAppointmentId() << ","
        << a.getPatientId() << ","
        << a.getDoctorId() << ","
        << a.getDate() << ","
        << a.getTimeSlot() << ","
        << a.getStatus() << "\n";
    file.close();
}

void FileHandler::saveBill(Bill& b) {
    ofstream file(BASE_PATH + "bills.txt", ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Error: bills.txt could not be opened.");
    }
    file << b.getBillId() << ","
        << b.getPatientId() << ","
        << b.getAppointmentId() << ","
        << b.getAmount() << ","
        << b.getStatus() << ","
        << b.getDate() << "\n";
    file.close();
}

void FileHandler::savePrescription(Prescription& p) {
    ofstream file(BASE_PATH + "prescriptions.txt", ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Error: prescriptions.txt could not be opened.");
    }
    file << p.getPrescriptionId() << ","
        << p.getAppointmentId() << ","
        << p.getPatientId() << ","
        << p.getDoctorId() << ","
        << p.getDate() << ","
        << p.getMedicines() << ","
        << p.getNotes() << "\n";
    file.close();
}

// ==================== UPDATING FUNCTIONS ====================

void FileHandler::updatePatient(patient& p) {
    ifstream fileIn(BASE_PATH + "patients.txt");
    if (!fileIn.is_open()) {
        throw FileNotFoundException("Error: patients.txt could not be opened.");
    }
    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(fileIn, line)) {
        if (!line.empty()) {
            lines[lineCount++] = line;
        }
    }
    fileIn.close();

    ofstream fileOut(BASE_PATH + "patients.txt");
    for (int i = 0; i < lineCount; i++) {
        string parts[7];
        int count = 0;
        splitLine(lines[i], parts, count);
        if (count == 7 && stringToInt(parts[0]) == p.getid()) {
            fileOut << p.getid() << ","
                << p.getname() << ","
                << p.getage() << ","
                << p.getgender() << ","
                << p.getcontact() << ","
                << p.getpassword() << ","
                << p.getbalance() << "\n";
        }
        else {
            fileOut << lines[i] << "\n";
        }
    }
    fileOut.close();
}

void FileHandler::updateAppointment(Appointment& a) {
    ifstream fileIn(BASE_PATH + "appointments.txt");
    if (!fileIn.is_open()) {
        throw FileNotFoundException("Error: appointments.txt could not be opened.");
    }
    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(fileIn, line)) {
        if (!line.empty()) {
            lines[lineCount++] = line;
        }
    }
    fileIn.close();

    ofstream fileOut(BASE_PATH + "appointments.txt");
    for (int i = 0; i < lineCount; i++) {
        string parts[6];
        int count = 0;
        splitLine(lines[i], parts, count);
        if (count == 6 && stringToInt(parts[0]) == a.getAppointmentId()) {
            fileOut << a.getAppointmentId() << ","
                << a.getPatientId() << ","
                << a.getDoctorId() << ","
                << a.getDate() << ","
                << a.getTimeSlot() << ","
                << a.getStatus() << "\n";
        }
        else {
            fileOut << lines[i] << "\n";
        }
    }
    fileOut.close();
}

void FileHandler::updateBill(Bill& b) {
    ifstream fileIn(BASE_PATH + "bills.txt");
    if (!fileIn.is_open()) {
        throw FileNotFoundException("Error: bills.txt could not be opened.");
    }
    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(fileIn, line)) {
        if (!line.empty()) {
            lines[lineCount++] = line;
        }
    }
    fileIn.close();

    ofstream fileOut(BASE_PATH + "bills.txt");
    for (int i = 0; i < lineCount; i++) {
        string parts[6];
        int count = 0;
        splitLine(lines[i], parts, count);
        if (count == 6 && stringToInt(parts[0]) == b.getBillId()) {
            fileOut << b.getBillId() << ","
                << b.getPatientId() << ","
                << b.getAppointmentId() << ","
                << b.getAmount() << ","
                << b.getStatus() << ","
                << b.getDate() << "\n";
        }
        else {
            fileOut << lines[i] << "\n";
        }
    }
    fileOut.close();
}

// ==================== DELETING FUNCTIONS ====================

void FileHandler::deletePatient(int id) {
    ifstream fileIn(BASE_PATH + "patients.txt");
    if (!fileIn.is_open()) {
        throw FileNotFoundException("Error: patients.txt could not be opened.");
    }
    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(fileIn, line)) {
        if (!line.empty()) {
            lines[lineCount++] = line;
        }
    }
    fileIn.close();

    ofstream fileOut(BASE_PATH + "patients.txt");
    for (int i = 0; i < lineCount; i++) {
        string parts[7];
        int count = 0;
        splitLine(lines[i], parts, count);
        if (count == 7 && stringToInt(parts[0]) == id) {
            continue;
        }
        fileOut << lines[i] << "\n";
    }
    fileOut.close();
}

void FileHandler::deleteDoctor(int id) {
    ifstream fileIn(BASE_PATH + "doctors.txt");
    if (!fileIn.is_open()) {
        throw FileNotFoundException("Error: doctors.txt could not be opened.");
    }
    string lines[100];
    int lineCount = 0;
    string line;
    while (getline(fileIn, line)) {
        if (!line.empty()) {
            lines[lineCount++] = line;
        }
    }
    fileIn.close();

    ofstream fileOut(BASE_PATH + "doctors.txt");
    for (int i = 0; i < lineCount; i++) {
        string parts[6];
        int count = 0;
        splitLine(lines[i], parts, count);
        if (count == 6 && stringToInt(parts[0]) == id) {
            continue;
        }
        fileOut << lines[i] << "\n";
    }
    fileOut.close();
}

// ==================== SPECIAL FUNCTIONS ====================

void FileHandler::logSecurityEvent(string log) {
    ofstream file(BASE_PATH + "security_log.txt", ios::app);
    if (!file.is_open()) {
        cout << "Cannot open security_log.txt" << endl;
        return;
    }
    file << log << "\n";
    file.close();
}

void FileHandler::saveDischargedPatient(patient& p) {
    ofstream file(BASE_PATH + "discharged.txt", ios::app);
    if (!file.is_open()) {
        throw FileNotFoundException("Error: discharged.txt could not be opened.");
    }
    file << p.getid() << ","
        << p.getname() << ","
        << p.getage() << ","
        << p.getgender() << ","
        << p.getcontact() << ","
        << p.getpassword() << ","
        << p.getbalance() << "\n";
    file.close();
}