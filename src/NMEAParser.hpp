#pragma once

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include "nmea_format.hpp"

class NMEAParser
{
public:
	int parse(const char* buffer, int length); // Returns the number of messages parsed

	SensorGps gps_report() { return _gps_report; };

private:
	// Parses an NMEA message and updates the NMEA data structures
	void handle_nmea_message(const char* buffer, int length);

	void handle_ZDA(const char* msg);
	void handle_GGA(const char* msg);
	void handle_RMC(const char* msg);
	void handle_GST(const char* msg);
	void handle_GSA(const char* msg);
	void handle_GSV(const char* msg);
	void handle_VTG(const char* msg);
	void handle_GBS(const char* msg);

	// Copy data from NMEA structs to SensorGps
	void update_gps_report();

	// Process the buffer and return the number of messages parsed
	int process_buffer();
	bool validate_checksum(const char* nmeaMessage, int length);

	static const int BUFFER_SIZE = 2048;
	char _buffer[2048] = {};
	int _buffer_length = 0;

	RMC_Data _rmc;
	GGA_Data _gga;
	VTG_Data _vtg;
	GST_Data _gst;
	GSA_Data _gsa;
	// GBS_Data _gbs;

	// Data from NMEA structs will be filled into this report for PX4
	SensorGps _gps_report = {};
};

