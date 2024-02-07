#pragma once

#include "px4_include.hpp"

struct RMC_Data {
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

	void print() {
		PX4_INFO("RMC");
	    PX4_INFO("timestamp: %f", timestamp);
	    PX4_INFO("status: %c", status);
	    PX4_INFO("lat: %f", lat);
	    PX4_INFO("ns: %c", ns);
	    PX4_INFO("lon: %f", lon);
	    PX4_INFO("ew: %c", ew);
	    PX4_INFO("speed: %f", speed);
	    PX4_INFO("track_good: %f", track_good);
	    PX4_INFO("date: %d", date);
	    PX4_INFO("mag_var: %f", mag_var);
	    PX4_INFO("mag_var_dir: %c", mag_var_dir);
	    PX4_INFO("mode: %c", mode);
	    PX4_INFO("nav_status: %c", nav_status);
	    PX4_INFO("");
	}
};

struct GGA_Data {
	// $<TalkerID>GGA 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp>	 	-- UTC time of GPS sample, hhmmss.sss
	// <Lat> 			-- Latitude as degrees:, DDDMM.MMMMM
	// <N/S> 			-- Latitude direction, "N" or "S"
	// <Long>			-- Longitude as degrees:, DDDMM.MMMMM
	// <E/W> 			-- Longitude direction, "E" or "W"
	// <GPSQual> 		-- 0 = Fix not available or invalid, 1 = GPS PS Mode fix valid, 2 = Differential GPS SPS Mode fix valid, 6 = Estimated (dead reckoning) mode
	// <Sats> 			-- Satellites in use
	// <HDOP> 			-- Horizontal Dilution of Precision, max: 99.0
	// <Alt> 			-- Height above mean sea level, max: 100000m
	// <AltVal> 		-- Reference Unit for Altitude ("M" = meters)
	// <GeoSep> 		-- Geoidal Separation measure in "M" = meters
	// <GeoVal> 		-- Reference Unit for GeoSep ("M" = meters)
	// <DGPSAge> 		-- Not supported
	// <DGPSRef> 		-- Not supported

	float timestamp = 0.0;
	double lat = 0.0;
	char ns = '?';
	double lon = 0.0;
	char ew = '?';
	int fix_quality = 0;
	int sats = 0;
	float hdop = 0.f;
	int alt = 0;
	char alt_val = '?';
	int geo_sep = 0;
	char geo_val = '?';

	void print() {
		PX4_INFO("GGA");
	    PX4_INFO("timestamp: %f", timestamp);
	    PX4_INFO("lat: %f", lat);
	    PX4_INFO("ns: %c", ns);
	    PX4_INFO("lon: %f", lon);
	    PX4_INFO("ew: %c", ew);
	    PX4_INFO("fix_quality: %d", fix_quality);
	    PX4_INFO("sats: %d", sats);
	    PX4_INFO("hdop: %f", hdop);
	    PX4_INFO("alt: %d", alt);
	    PX4_INFO("alt_val: %c", alt_val);
	    PX4_INFO("geo_sep: %d", geo_sep);
	    PX4_INFO("geo_val: %c", geo_val);
	    PX4_INFO("");
	}
};

