#pragma once

#include <cstring>
#include <cstdio>

#ifndef PX4_INFO
#define PX4_INFO(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#endif

class NMEAParser {
public:
	NMEAParser();
	int parse(const char* buffer, int length); // Returns the number of messages parsed

private:
	static const int BUFFER_SIZE = 2048;
	char _buffer[BUFFER_SIZE] = {};
	int _buffer_length = 0;

	int process_buffer(); // Process the buffer and return the number of messages parsed
	bool validate_checksum(const char* nmeaMessage, int length);
};