#pragma once

#include <cstring>

class NMEAParser {
public:
    NMEAParser();
    ~NMEAParser();
    int parse(const char* buffer, int length);

private:
    static const int BUFFER_SIZE = 2048; // Adjust size as needed based on the expected length of NMEA messages
    char _buffer[BUFFER_SIZE];    // Buffer to hold raw data and incomplete NMEA messages
    int _buffer_length;

    int processBuffer();                 // Process the buffer and return the number of messages parsed
    bool verifyChecksum(const char* nmeaMessage, int length); // Verify the checksum of an NMEA message
};