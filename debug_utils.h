#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

//#define DEBUG

#ifdef DEBUG
#define INIT_DEBUG() Serial.begin(115200);
#define DEBUG_PRINT(str)    \
    Serial.print(millis());     \
    Serial.print(": ");    \
    Serial.print(__PRETTY_FUNCTION__); \
    Serial.print(' ');      \
    Serial.print(__FILE__);     \
    Serial.print(':');      \
    Serial.print(__LINE__);     \
    Serial.print(' ');      \
    Serial.println(str);
#define DBPRINT(s) Serial.print(s);
#define DBPRINTLN(s) Serial.println(s);
#else
#define INIT_DEBUG()
#define DEBUG_PRINT(str)
#define DBPRINT(s)
#define DBPRINTLN(s)
#endif

#endif