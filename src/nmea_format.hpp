#pragma once

#include "px4_include.hpp"

struct RMC_Data {
	// $<TalkerID>RMC   -- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp>      -- UTC time of GPS sample, hhmmss.sss
	// <Status>         -- "A" = valid, "V" = Warning, "V" is reported in NO FIX conditions and "A" is reported in 2D and 3D fix conditions.
	// <Lat>            -- Latitude as degrees:, DDDMM.MMMMM
	// <N/S>            --  Latitude direction, "N" or "S"
	// <Long>           -- Longitude as degrees:, DDDMM.MMMMM
	// <E/W>            -- Longitude direction, "E" or "W"
	// <Speed>          -- Speed over ground in knots, ddd.d,
	// <Trackgood>      -- Course made good, max. 999.9
	// <Date>           -- Date of Fix: ddmmyy
	// <MagVar>         -- Magnetic Variation, max.: 090.0
	// <MagVarDir>      -- Magnetic Variation Direction, "E" or "W"
	// <mode>           -- Positioning system Mode Indicator, "D" = Differential mode "A" = Autonomous mode "N" = data not valid "E" = Estimated (dead reckoning) mode
	// <Nav_status>     -- Navigational status indicator: "S" = Safe "C" = Caution "U" = Unsafe "V" = Not valid

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

	void print()
	{
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
	// $<TalkerID>GGA   -- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp>      -- UTC time of GPS sample, hhmmss.sss
	// <Lat>            -- Latitude as degrees:, DDDMM.MMMMM
	// <N/S>            -- Latitude direction, "N" or "S"
	// <Long>           -- Longitude as degrees:, DDDMM.MMMMM
	// <E/W>            -- Longitude direction, "E" or "W"
	// <GPSQual>        -- 0 = Fix not available or invalid, 1 = GPS PS Mode fix valid, 2 = Differential GPS SPS Mode fix valid, 6 = Estimated (dead reckoning) mode
	// <Sats>           -- Satellites in use
	// <HDOP>           -- Horizontal Dilution of Precision, max: 99.0
	// <Alt>            -- Height above mean sea level, max: 100000m
	// <AltVal>         -- Reference Unit for Altitude ("M" = meters)
	// <GeoSep>         -- Geoidal Separation measure in "M" = meters
	// <GeoVal>         -- Reference Unit for GeoSep ("M" = meters)
	// <DGPSAge>        -- Not supported
	// <DGPSRef>        -- Not supported

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

	void print()
	{
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

struct VTG_Data {
	// $<TalkerID>VTG   -- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <TMGT>           -- Track in reference to "true" earth poles, ddd.d in degrees
	// <T>              -- Indicates "terrestrial"
	// <TMGM>           -- Track in reference to "magnetic" earth poles, ddd.d in degrees
	// <M>              -- Indicates "magnetic"
	// <SoGN>           -- Speed over Ground in knots
	// <N>              -- Indicates "knots"
	// <SoGK>           -- Speed over Ground in kilometers per hour, ddd.d in km/h
	// <K>              -- Indicates "kilometres"
	// <D>              -- Mode indicator: A = Autonomous mode, D = Differential mode, E = Estimated mode

	float track_true = 0.f;
	char T = '?';
	float track_mag = 0.f;
	char M = '?';
	float ground_speed_knots = 0.f;
	char N = '?';
	float ground_speed_kmh = 0.f;
	char K = '?';
	char mode = '?';

	void print()
	{
		PX4_INFO("VTG");
		PX4_INFO("track_true: %f", track_true);
		PX4_INFO("T: %c", T);
		PX4_INFO("track_mag: %f", track_mag);
		PX4_INFO("M: %c", M);
		PX4_INFO("ground_speed_knots: %f", ground_speed_knots);
		PX4_INFO("N: %c", N);
		PX4_INFO("ground_speed_kmh: %f", ground_speed_kmh);
		PX4_INFO("K: %c", K);
		PX4_INFO("mode: %c", mode);
		PX4_INFO("");
	}
};

struct GST_Data {
	// $<TalkerID>GST 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp> 		-- UTC time of GPS sample, hhmmss.sss
	// <EHPE> 			-- Equivalent Horizontal Position Error, dd.d in m
	// <Semi-major> 	-- Standard deviation (meters) of semi-major axis of error ellipse, dd.d in m
	// <Semi-minor> 	-- Standard deviation (meters) of semi-minor axis of error ellipse, dd.d in m
	// <Angle> 			-- Orientation of semi-major axis of error ellipse (true north degrees), dd.d in degree
	// <LatErr> 		-- Standard deviation (meters) of latitude error, dd.d in m
	// <LonErr> 		-- Standard deviation (meters) of longitude error, dd.d in m
	// <Alt Err Dev> 	-- Standard deviation (meters) of altitude error, dd.d in m

	float timestamp = 0.f;
	float ehpe = 0.f;
	float semi_major_stddev = 0.f;
	float semi_minor_stddev = 0.f;
	float angle = 0.f;
	float lat_err = 0.f;
	float lon_err = 0.f;
	float alt_err = 0.f;

	void print()
	{
		PX4_INFO("GST");
		PX4_INFO("timestamp: %f", timestamp);
		PX4_INFO("ehpe: %f", ehpe);
		PX4_INFO("semi_major_stddev: %f", semi_major_stddev);
		PX4_INFO("semi_minor_stddev: %f", semi_minor_stddev);
		PX4_INFO("angle: %f", angle);
		PX4_INFO("lat_err: %f", lat_err);
		PX4_INFO("lon_err: %f", lon_err);
		PX4_INFO("alt_err: %f", alt_err);
		PX4_INFO("");
	}
};

struct GSA_Data {
	// $<TalkerID>GSA 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Mode> 			-- M = manual, A = automatic
	// <CurrentMode> 	-- 1 = Fix not available or invalid, 2 = GPS SPS Mode fix valid, 3 = Differential GPS SPS Mode fix valid
	// <SatPRN1> 		-- Satellites list used for positioning. See Preliminary notes about satellites PRN ranges for more info about available values.
	// ...
	// <SatPRNN> 		--
	// <PDOP> 			-- Position Dilution of Precision, max: 99.0
	// <HDOP> 			-- Horizontal Dilution of Precision, max: 99.0
	// <VDOP> 			-- Vertical Dilution of Precision, max: 99.0
	// <SystemID> 		-- 1 = GPS, 2 = GLONASS, 3 = GALILEO, 4 = BEIDOU, 5 = QZSS

	char mode = '?';
	int current_mode = 0;
	int sat_prn[12] = {0};
	float pdop = 0.f;
	float hdop = 0.f;
	float vdop = 0.f;
	char system_id = '?';

	void print()
	{
		PX4_INFO("GSA");
		PX4_INFO("mode: %c", mode);
		PX4_INFO("current_mode: %d", current_mode);

		for (int i = 0; i < 12; i++) {
			if (sat_prn[i] != 0) {
				PX4_INFO("sat_prn[%d]: %d", i, sat_prn[i]);
			}
		}

		PX4_INFO("pdop: %f", pdop);
		PX4_INFO("hdop: %f", hdop);
		PX4_INFO("vdop: %f", vdop);
		PX4_INFO("system_id: %c", system_id);
		PX4_INFO("");
	}
};