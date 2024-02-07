#include <NMEAParser.hpp>

void NMEAParser::handle_GGA(const char* msg)
{
	// $GPGGA,183417.000,04814.03970,N,01128.52205,E,0,00,99.0,495.53,M,47.6,M*53

	// Global Positioning System Fixed data

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
	// *
	// <checksum>
	// <cr><lf>

	char* endp = nullptr;

	if (msg && *(++msg) != ',') { _gga.timestamp = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.lat = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.ns = *(msg++); }

	if (msg && *(++msg) != ',') { _gga.lon = strtod(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.ew = *(msg++); }

	if (msg && *(++msg) != ',') { _gga.fix_quality = strtol(msg, &endp, 10); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.sats = strtol(msg, &endp, 10); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.hdop = strtof(msg, &endp); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.alt = strtol(msg, &endp, 10); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.alt_val = *(msg++); }

	if (msg && *(++msg) != ',') { _gga.geo_sep = strtol(msg, &endp, 10); msg = endp; }

	if (msg && *(++msg) != ',') { _gga.geo_val = *(msg++); }

	if (_gga.ns == 'S') {
		_gga.lat = -_gga.lat;
	}

	if (_gga.ew == 'W') {
		_gga.lon = -_gga.lon;
	}

#if defined(DEBUG_BUILD)
	_gga.print();
#endif
}