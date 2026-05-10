#ifndef HOSPITALEXCEPTION_H
#define HOSPITALEXCEPTION_H

#include <string>
using namespace std;

class HospitalException {
protected:
    string message;
public:
    HospitalException(string msg) {
        message = msg;
    }
    virtual string what() {
        return message;
    }
};

#endif
