#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <string>
using namespace std;

class Validator {
public:
    static bool isValidId(int id);
    static bool isValidDate(string date);
    static bool isValidTimeSlot(string slot);
    static bool isValidContact(string contact);
    static bool isValidPassword(string password);
    static bool isValidFloat(float amount);
    static bool isValidMenuChoice(int choice, int min, int max);
};

#endif
