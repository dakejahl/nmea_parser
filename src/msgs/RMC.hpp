#include <cstdlib>
#include <px4_include.hpp>

struct RMC_Data {
	// $GNRMC,,V,,,,,,,,,,N,V*37

	// Recommended Minimum Specific GPS/Transit data. Time, date, position and speed data provided by the GNSS
	// Teseo. This sentence is transmitted at intervals not exceeding 2 seconds and is always accompanied by RMB
	// when destination way point is active.

	// $<TalkerID>RMC 	-- GN (multi-constellation), GP (GPS), GL (GLONASS), GA (GALILEO), BD (BEIDOU), QZ (QZSS)
	// <Timestamp> 		-- UTC time of GPS sample, hhmmss.sss
	// <Status> 		-- "A" = valid, "V" = Warning, "V" is reported in NO FIX conditions and "A" is reported in 2D and 3D fix conditions.
	// <Lat> 			-- Latitude as degrees:, DDDMM.MMMMM
	// <N/S> 			-- Latitude direction, "N" or "S"
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

static RMC_Data handle_RMC(const char* msg)
{
	RMC_Data rmc = {};
	char* endp = nullptr;

	if (msg && *(++msg) != ',') { rmc.timestamp = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.status = *(msg++); }

	if (msg && *(++msg) != ',') { rmc.lat = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.ns = *(msg++); }

	if (msg && *(++msg) != ',') { rmc.lon = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.ew = *(msg++); }

	if (msg && *(++msg) != ',') { rmc.speed = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.track_good = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.date = static_cast<int>(strtol(msg, &endp, 10)); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.mag_var = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { rmc.mag_var_dir = *(msg++); }

	if (msg && *(++msg) != ',') { rmc.mode = *(msg++); }

	if (msg && *(++msg) != ',') { rmc.nav_status = *(msg++); }

#if defined(DEBUG_BUILD)
	rmc.print();
#endif

	return rmc;
}