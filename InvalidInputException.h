#ifndef INVALIDINPUTEXCEPTION_H
#define INVALIDINPUTEXCEPTION_H

#include "HospitalException.h"

class InvalidInputException : public HospitalException {
public:
    InvalidInputException() : HospitalException("Error: Invalid input entered.") {}
    InvalidInputException(string msg) : HospitalException(msg) {}
};

#endif