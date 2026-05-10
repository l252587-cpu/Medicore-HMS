#ifndef INSUFFICIENTFUNDSEXCEPTION_H
#define INSUFFICIENTFUNDSEXCEPTION_H

#include "HospitalException.h"

class InsufficientFundsException : public HospitalException {
public:
    InsufficientFundsException() : HospitalException("Error: Insufficient balance.") {}
    InsufficientFundsException(string msg) : HospitalException(msg) {}
};

#endif