// NMEAParser.cpp

#include "NMEAParser.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>

NMEAParser::NMEAParser()
	: _buffer_length(0)
{}

NMEAParser::~NMEAParser()
{}

int NMEAParser::parse(const char* buffer, int length)
{
	// Append new data to the internal buffer
	if (_buffer_length + length < BUFFER_SIZE) {
		memcpy(_buffer + _buffer_length, buffer, length);
		_buffer_length += length;
	} else {
		std::cout << "overflow" << std::endl;
		// Handle buffer overflow here.
	}

	// Process the buffer and return the number of messages parsed
	return processBuffer();
}

const char* findCharInArray(const char* start, char c, int length)
{
	for (int i = 0; i < length; i++) {
		if (start[i] == c) {
			return &start[i];
		}
	}
	return nullptr;
}

int NMEAParser::processBuffer()
{
	int messagesParsed = 0;
	int startPos = 0;
	int bytes_remaining = _buffer_length;

	while (1) {
		// Find the start ($) of an NMEA message
		const char* start = findCharInArray(_buffer + startPos, '$', bytes_remaining);

		if (bytes_remaining == 0) {
			break;
		}

		if (!start) {
			std::cout << "missingstart: bytes_remaining:" << bytes_remaining << std::endl;
			std::string msg(_buffer + startPos, bytes_remaining);
			std::cout << "missing start: " << msg.c_str() << std::endl;
			break;
		}

		// Find the end (*) of the NMEA message, starting from the found start position
		const char* end = findCharInArray(start, '*', _buffer + _buffer_length - start);
		if (!end) {
			std::cout << "missing end" << std::endl;
			break; // No end found from the start position, message is likely incomplete
		}

		// Found start and end of message
		int messageLength = end - start + 5; // * (1), crc (2), crlf (2)

		// $ + data + * + checksum
		// $GNGGA,,,,,,0,00,0.0,,M,,M,,*56
		// auto strmsg = std::string(start, messageLength);
		// std::cout << strmsg << std::endl;

		if (messageLength > bytes_remaining) {
			std::cout << "incomplete message" << std::endl;
			break;
		}

		if (validateChecksum(start, messageLength)) {
			messagesParsed++;

			// TODO: handle_message()

			// Increment to the start of next expected message
			startPos = (start - _buffer) + messageLength;

		} else {
			std::cout << "Invalid checksum" << std::endl;
			// If checksum is invalid or message incomplete, move startPos just after this '$'
			startPos = (start - _buffer) + 1;
		}

		bytes_remaining = _buffer_length - startPos;
	}

	// Shift remaining data to the beginning of the buffer
	if (startPos < _buffer_length) {
		memmove(_buffer, _buffer + startPos, bytes_remaining);
		_buffer_length -= startPos;

		std::cout << "bytes_remaining:" << bytes_remaining << std::endl;

		for (size_t i = 0; i < _buffer_length; i++) {
			printf("%c", _buffer[i]);
		}
		printf("\n\n");

	} else {
		_buffer_length = 0;
	}

	std::cout << "Parsed messages: " << messagesParsed << std::endl;

	return messagesParsed;
}


bool NMEAParser::validateChecksum(const char* nmeaMessage, int length)
{
	const char* asteriskPos = strchr(nmeaMessage, '*');

	unsigned char calculatedChecksum = 0;
	for (const char* p = nmeaMessage + 1; p < asteriskPos; p++) {
		calculatedChecksum ^= *p;
	}

	char receivedChecksumStr[3] = {asteriskPos[1], asteriskPos[2], '\0'};
	int messageChecksum = 0;
	sscanf(receivedChecksumStr, "%X", &messageChecksum);

	return messageChecksum == calculatedChecksum;
}
