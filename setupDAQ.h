#include <SPI.h>
#include "mcp_can.h"
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 rtc;
#include <SoftwareSerial.h>

const byte HC12RxdPin = 3;                      // "RXD" Pin on HC12
const byte HC12TxdPin = 2;                      // "TXD" Pin on HC12
const byte HC12SetPin = 4;                      // "SET" Pin on HC12

unsigned long timer = millis();                 // Delay Timer

char SerialByteIn;                              // Temporary variable
char HC12ByteIn;                                // Temporary variable
String HC12ReadBuffer = "";                     // Read/Write Buffer 1 for HC12
String SerialReadBuffer = "";                   // Read/Write Buffer 2 for Serial
boolean SerialEnd = false;                      // Flag to indicate End of Serial String
boolean HC12End = false;                        // Flag to indiacte End of HC12 String
boolean commandMode = false;                    // Send AT commands

File myFile;

SoftwareSerial HC12(HC12TxdPin, HC12RxdPin);

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_CAN    = 8;
const int SPI_CS_SD     = 9;

// File to save on SD card
const char NAME[]   = "CAN_";
const char EXTENSION[]  = ".csv";
String FILENAME;

MCP_CAN CAN(SPI_CS_CAN);                                    // Set CS pin

unsigned char flagRecv = 0;
unsigned char len = 0;
unsigned char buf[8];