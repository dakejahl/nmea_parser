// NMEAParser.cpp

#include "NMEAParser.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>

NMEAParser::NMEAParser() : _buffer_length(0) {
    // Initialize internal buffer
    memset(_buffer, 0, BUFFER_SIZE);
}

NMEAParser::~NMEAParser() {
    // Destructor
}

int NMEAParser::parse(const char* buffer, int length) {
    // Append new data to the internal buffer
    if (_buffer_length + length < BUFFER_SIZE) {
        memcpy(_buffer + _buffer_length, buffer, length);
        _buffer_length += length;
    } else {
        // Handle buffer overflow or implement a circular buffer if necessary
        std::cout << "Handle buffer overflow or implement a circular buffer if necessary" << std::endl;
    }

    // Process the buffer and return the number of messages parsed
    return processBuffer();
}

int NMEAParser::processBuffer() {
    int messagesParsed = 0;
    int startPos = 0;

    // Discard any data before the first '$'
    char* firstMessageStart = strchr(_buffer, '$');
    if (firstMessageStart != nullptr) {
        int discardLength = firstMessageStart - _buffer;
        memmove(_buffer, _buffer + discardLength, _buffer_length - discardLength);
        _buffer_length -= discardLength;
    }

    while (true) {
        // Find the start ($) and end (*) of an NMEA message
        char* start = strchr(_buffer + startPos, '$');
        char* end = strchr(_buffer + startPos, '*');

        if (!start || !end || end - start > _buffer_length - 3) {
            std::cout << "Incomplete or no message: " << "_buffer_length:" << _buffer_length << std::endl;
            break; // Incomplete or no message
        }

        // Calculate message length
        int messageLength = (end - start) + 3;

        // Verify and handle the message
        if (verifyChecksum(start, messageLength)) {
            messagesParsed++;
            std::string msg(start + 3, 3);
            std::cout << "handling message :) -- size:" << messageLength << " -- " << msg.c_str() << std::endl;
            // TODO: Handle the parsed message
        }

        // Move to the next potential message start
        startPos = (end - _buffer) + 3;
    }

    // Shift remaining data to the beginning of the buffer
    if (startPos < _buffer_length) {
        memmove(_buffer, _buffer + startPos, _buffer_length - startPos);
        _buffer_length -= startPos;
    } else {
        _buffer_length = 0;
    }

    return messagesParsed;
}

bool NMEAParser::verifyChecksum(const char* nmeaMessage, int length) {
    if (length < 4) { // $ + data + * + checksum
        return false; // Invalid message format
    }

    const char* asteriskPos = strchr(nmeaMessage, '*');
    if (!asteriskPos || asteriskPos - nmeaMessage > length - 3) {
        return false; // No checksum or malformed message
    }

    unsigned char calculatedChecksum = 0;
    for (const char* p = nmeaMessage + 1; p < asteriskPos; p++) {
        calculatedChecksum ^= *p;
    }

    char receivedChecksum[3] = {asteriskPos[1], asteriskPos[2], '\0'};
    int messageChecksum = 0;
    sscanf(receivedChecksum, "%X", &messageChecksum);

    return messageChecksum == calculatedChecksum;
}
