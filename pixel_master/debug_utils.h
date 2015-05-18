#ifndef DEBUG_UTILS_H
#define DEBUG_UTILS_H

#define DEBUG 0

#if DEBUG
#define DEBUG_PRINT(str) Serial.print(str)
#define DEBUG_PRINT2(str, opt) Serial.print(str, opt)
#define DEBUG_PRINTLN(str) Serial.println(str)
#define DEBUG_PRINTLN2(str, opt) Serial.println(str, opt)
#else
#define DEBUG_PRINT(str) 
#define DEBUG_PRINT2(str, opt) 
#define DEBUG_PRINTLN(str)
#define DEBUG_PRINTLN2(str, opt)
#endif

#endif

 