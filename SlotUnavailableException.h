#ifndef SLOTUNAVAILABLEEXCEPTION_H
#define SLOTUNAVAILABLEEXCEPTION_H

#include "HospitalException.h"

class SlotUnavailableException : public HospitalException {
public:
    SlotUnavailableException() : HospitalException("Error: This time slot is already booked.") {}
    SlotUnavailableException(string msg) : HospitalException(msg) {}
};

#endif
