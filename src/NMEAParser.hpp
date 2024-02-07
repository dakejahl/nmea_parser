#pragma once

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <nmea_format.hpp>

#ifndef PX4_INFO
#define PX4_INFO(fmt, ...) printf(fmt "\n", ##__VA_ARGS__)
#endif

class NMEAParser {
public:
	int parse(const char* buffer, int length); // Returns the number of messages parsed

private:
	// Decodes an NMEA message and updates the GNSS data structure
	void handle_nmea_message(const char* buffer, int length);

	void handle_ZDA(const char* msg);
	void handle_GGA(const char* msg);
	void handle_HDT(const char* msg);
	void handle_GNS(const char* msg);
	void handle_RMC(const char* msg);
	void handle_GST(const char* msg);
	void handle_GSA(const char* msg);
	void handle_GSV(const char* msg);
	void handle_VTG(const char* msg);
	void handle_GBS(const char* msg);

	// Process the buffer and return the number of messages parsed
	int process_buffer();
	bool validate_checksum(const char* nmeaMessage, int length);

	static const int BUFFER_SIZE = 2048;
	char _buffer[2048] = {};
	int _buffer_length = 0;

	RMC_Data _rmc;
};

