#ifndef BILL_H
#define BILL_H

#include <iostream>
#include <string>
using namespace std;

class Bill {
private:
    int billId;
    int patientId;
    int appointmentId;
    float amount;
    string status;
    string date;

public:
  
    Bill();
    Bill(int billId, int patientId, int appointmentId, float amount, string status, string date);

  
    int getBillId();
    int getPatientId();
    int getAppointmentId();
    float getAmount();
    string getStatus();
    string getDate();
    int getid();

  
    void setBillId(int id);
    void setPatientId(int id);
    void setAppointmentId(int id);
    void setAmount(float amount);
    void setStatus(string status);
    void setDate(string date);

   
    friend ostream& operator<<(ostream& out, Bill& b);
};

#endif
