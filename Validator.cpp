#include "Validator.h"
#include <ctime>

// helper function to check if a character is a digit
bool isDigit(char c) {
    return c >= '0' && c <= '9';
}

// helper function to convert a single character to its number value
int charToInt(char c) {
    return c - '0';
}

// helper function to get length of string manually
int getLength(string s) {
    return (int)s.length();
}

// ID must be greater than 0
bool Validator::isValidId(int id) {
    return id > 0;
}

// date must be in DD-MM-YYYY format
bool Validator::isValidDate(string date) {
    // check length must be exactly 10
    if (getLength(date) != 10) {
        return false;
    }

    // check dashes are in right place
    if (date[2] != '-' || date[5] != '-') {
        return false;
    }

    // check all other positions are digits
    for (int i = 0; i < 10; i++) {
        if (i == 2 || i == 5) continue;
        if (!isDigit(date[i])) {
            return false;
        }
    }

    // extract day manually
    int day = charToInt(date[0]) * 10 + charToInt(date[1]);

    // extract month manually
    int month = charToInt(date[3]) * 10 + charToInt(date[4]);

    // extract year manually
    int year = charToInt(date[6]) * 1000 +
        charToInt(date[7]) * 100 +
        charToInt(date[8]) * 10 +
        charToInt(date[9]);

    // get current year using ctime
    time_t t = time(0);
    tm now;
    localtime_s(&now, &t);
    int currentYear = now.tm_year + 1900;

    // validate ranges
    if (day < 1 || day > 31) {
        return false;
    }
    if (month < 1 || month > 12) {
        return false;
    }
    if (year < currentYear) {
        return false;
    }

    return true;
}

// time slot must be one of the 8 fixed slots
bool Validator::isValidTimeSlot(string slot) {
    string validSlots[8] = {
        "09:00", "10:00", "11:00", "12:00",
        "13:00", "14:00", "15:00", "16:00"
    };

    for (int i = 0; i < 8; i++) {
        if (slot == validSlots[i]) {
            return true;
        }
    }
    return false;
}

// contact must be exactly 11 digits all numeric
bool Validator::isValidContact(string contact) {
    // check length manually
    if (getLength(contact) != 11) {
        return false;
    }

    // check all characters are digits
    for (int i = 0; i < 11; i++) {
        if (!isDigit(contact[i])) {
            return false;
        }
    }

    return true;
}

// password must be at least 6 characters
bool Validator::isValidPassword(string password) {
    return getLength(password) >= 6;
}

// amount must be greater than 0
bool Validator::isValidFloat(float amount) {
    return amount > 0;
}

// choice must be between min and max
bool Validator::isValidMenuChoice(int choice, int min, int max) {
    return choice >= min && choice <= max;
}