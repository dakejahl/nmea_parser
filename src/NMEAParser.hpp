#pragma once

#include <cstring>
#include <cstdio>
#include <cstdlib>

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
};

struct RMC_Data {
	// $GNRMC,,V,,,,,,,,,,N,V*37

	// Recommended Minimum Specific GPS/Transit data. Time, date, position and speed data provided by the GNSS
	// Teseo. This sentence is transmitted at intervals not exceeding 2 seconds and is always accompanied by RMB
	// when destination way point is active.

	// $<TalkerID>RMC 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp> 		-- UTC time of GPS sample, hhmmss.sss
	// <Status> 		-- "A" = valid, "V" = Warning, "V" is reported in NO FIX conditions and "A" is reported in 2D and 3D fix conditions.
	// <Lat> 			-- Latitude as degrees:, DDDMM.MMMMM
	// <N/S> 			--  Latitude direction, "N" or "S"
	// <Long>			-- Longitude as degrees:, DDDMM.MMMMM
	// <E/W> 			-- Longitude direction, "E" or "W"
	// <Speed> 			-- Speed over ground in knots, ddd.d,
	// <Trackgood> 		-- Course made good, max. 999.9
	// <Date> 			-- Date of Fix: ddmmyy
	// <MagVar> 		-- Magnetic Variation, max.: 090.0
	// <MagVarDir> 		-- Magnetic Variation Direction, "E" or "W"
	// <mode> 			-- Positioning system Mode Indicator, "D" = Differential mode "A" = Autonomous mode "N" = data not valid "E" = Estimated (dead reckoning) mode
	// <Nav_status> 	-- Navigational status indicator: "S" = Safe "C" = Caution "U" = Unsafe "V" = Not valid
	// *
	// <checksum>
	// <cr><lf>

	float timestamp = 0.0;
	char status = 'V';
	double lat = 0.0;
	char ns = '?';
	double lon = 0.0;
	char ew = '?';
	float speed = 0.f;
	float track_good = 0.f;
	int date = 0;
	float mag_var = 0.f;
	char mag_var_dir = '?';
	char mode = '?';
	char nav_status = '?';
};