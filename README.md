Reads data from a serial port and parses NMEA strings. Data is stored in structs corresponding to the NMEA message ID.
```
nmea_parser
├── CMakeLists.txt
├── Makefile
└── src
    ├── main.cpp
    ├── msgs
    │   ├── GBS.hpp
    │   ├── GGA.hpp
    │   ├── GNS.hpp
    │   ├── GSA.hpp
    │   ├── GST.hpp
    │   ├── GSV.hpp
    │   ├── HDT.hpp
    │   ├── RMC.hpp
    │   ├── VTG.hpp
    │   └── ZDA.hpp
    ├── nmea_format.hpp
    ├── NMEAParser.cpp
    └── NMEAParser.hpp
```