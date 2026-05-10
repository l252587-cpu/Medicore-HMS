#ifndef PERSON_H
#define PERSON_H
#include <string>
using namespace std;

class person {
protected:
	int ID;
	string name;
	string password;
public:
	person();
	person(int, string, string);

	int getid();
	string getname();
	string getpassword();

	virtual void displayMenu() = 0;
	virtual void display() const = 0;

	virtual ~person() {}

};
#endif
