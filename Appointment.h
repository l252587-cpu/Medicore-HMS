#ifndef APPOINTMENT_H
#define APPOINTMENT_H

#include <iostream>
#include <string>
using namespace std;

class Appointment {
	int appointmentId;
	int patientId;
	int doctorId;
	string date;
	string timeslot;
	string status;
public:
	Appointment();
	Appointment(int, int, int, string, string, string);

	int getid();
	int getAppointmentId();
	int getPatientId();
	int getDoctorId();
	string getDate();
	string getTimeSlot();
	string getStatus();

	void setAppointmentId(int id);
	void setPatientId(int id);
	void setDoctorId(int id);
	void setDate(string date);
	void setTimeSlot(string timeSlot);
	void setStatus(string status);

	bool operator==(Appointment& other);
	friend ostream& operator<<(ostream& out, Appointment& a);
};
#endif