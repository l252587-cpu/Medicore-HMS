#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include "HospitalException.h"

class FileNotFoundException : public HospitalException {
public:
    FileNotFoundException() : HospitalException("Error: File could not be opened.") {}
    FileNotFoundException(string msg) : HospitalException(msg) {}
};

#endif
