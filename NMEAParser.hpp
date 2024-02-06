#pragma once

#include <cstring>

class NMEAParser {
public:
	NMEAParser();
	~NMEAParser();
	int parse(const char* buffer, int length);

private:
	static const int BUFFER_SIZE = 2048;
	char _buffer[BUFFER_SIZE] = {};
	int _buffer_length = 0;

	int processBuffer(); // Process the buffer and return the number of messages parsed
	bool validateChecksum(const char* nmeaMessage, int length);
};