#include <SoftwareSerial.h>

const byte HC12RxdPin = 3;                      // "RXD" Pin on HC12
const byte HC12TxdPin = 2;                      // "TXD" Pin on HC12
const byte HC12SetPin = 4;                      // "SET" Pin on HC12
SoftwareSerial HC12(HC12TxdPin, HC12RxdPin);

#define THROTTLE_POSITION_SENSOR_ID 0x01
#define ENGINE_TEMPERATURE_ID 0x02
#define OIL_PRESSURE_ID 0x03
#define OIL_TEMPERATURE_ID 0x04
#define LAMBDA_SENSOR_ID 0x05 
#define ENGINE_RPM_ID 0x06
#define ACCELEROMETER_ID 0x07

//STARCICU TU DODAJ ADRESE^^^^^^^^^^^^^^^^^^^^^^^^^
// #define uredjaj_ID <adresa>
