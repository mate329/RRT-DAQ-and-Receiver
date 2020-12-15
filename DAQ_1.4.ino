//Matija Kelemen v1.1- glavni kod
//Matteo Samsa v1.2- minor changes :P 2019-09-14
//MS - v1.3 - i zamjenjen za num, stalno stvaralo can_2.csv,
//            maknut interrupt jer je različiti chip od shielda

/*
MR - primanje podataka ->
TPS - 0x01
Temp motora - 0x02
TPS - Throttle Position Sensor - 0x03
Tlak ulja - 0x04
Temp ulja - 0x05
Lamda senzori - 0x06
RPM - 0x07
Accelerometar - 0x08
14 fuseova
Random adrese - nisu bitne
*/

//namjestiti channel za komunikaciju izmedju bolida i receivera !!!!!!! komanda AT+C069 npr

#include "setupDAQ.h"

void setup() {
    Serial.begin(9600); //extra dodano, treba provjeriti jel to to
    HC12.begin(9600);
  
    HC12ReadBuffer.reserve(64);                   // Reserve 64 bytes for Serial message input
    SerialReadBuffer.reserve(64);                 // Reserve 64 bytes for HC12 message input
  
    pinMode(HC12SetPin, OUTPUT);                  // Output High for Transparent / Low for Command
    digitalWrite(HC12SetPin, HIGH);               // Enter Transparent mode
    delay(80);                                    // 80 ms delay before operation per datasheet

//    for (int i=0; i < 10; ++i) {                  // Probaj 10 put inicijalizirati, ako nejde, nejde
//      if (SD.begin(SPI_CS_SD)) {
//        break;
//      }
//    }

    if(!SD.begin(SPI_CS_SD)){
         while(1);
    }

    // Calculate first non existing file on SD card to open
    int num = 1;
    while(SD.exists((String)NAME + (String)num++ + (String)EXTENSION) && num < 50000);
    FILENAME = (String)NAME + (String)--num + (String)EXTENSION; //------------------PAZI DEFINE GORE
    

    // init can bus : baudrate = 500k
    while (CAN_OK != CAN.begin(CAN_500KBPS)) { delay(100); }

    //attachInterrupt(0, MCP2515_ISR, FALLING); // start interrupt
    rtc.begin();
    myFile.close();
}

/*void MCP2515_ISR()
{
    flagRecv = 1;
}*/

void loop()
{
    while (HC12.available()) {                    // While Arduino's HC12 soft serial rx buffer has data
      HC12ByteIn = HC12.read();                   // Store each character from rx buffer in byteIn
      HC12ReadBuffer += char(HC12ByteIn);         // Write each character of byteIn to HC12ReadBuffer
      if (HC12ByteIn == '\n') {                   // At the end of the line
        HC12End = true;                           // Set HC12End flag to true
      }
    }
  
    if (HC12End) {                                // If HC12End flag is true
      if (HC12ReadBuffer.startsWith("AT")) {      // Check to see if a command is received from remote
        digitalWrite(HC12SetPin, LOW);            // Enter command mode
        delay(100);                               // Delay before sending command
        Serial.print(SerialReadBuffer);           // Echo command to serial.
        HC12.print(HC12ReadBuffer);               // Write command to local HC12
        delay(500);                               // Wait 0.5 s for reply
        digitalWrite(HC12SetPin, HIGH);           // Exit command / enter transparent mode
        delay(100);                               // Delay before proceeding
        HC12.println("Remote Command Executed");  // Acknowledge execution
      } else {
        Serial.print(HC12ReadBuffer);             // Send message to screen
      }
      HC12ReadBuffer = "";                        // Empty buffer
      HC12End = false;                            // Reset flag
    }

    
    unsigned long id= 0;
    
/////////////////////////////////////////////////        
    if (CAN_MSGAVAIL == CAN.checkReceive()) {
      
        unsigned char canID = CAN.getCanId();
        CAN.readMsgBuf(&len, buf);
        
        HC12.write(canID);
        HC12.write(buf[0]);
        HC12.write(buf, 2);
////////////////////////////////////////////////

                    
            myFile = SD.open((String)FILENAME, FILE_WRITE);// otvori sa FILENAME NEKI generirani
            // read data,  len: data length, buf: data buf
            CAN.readMsgBufID(&id, &len, buf);
            
            DateTime now = rtc.now();
            myFile.print(now.timestamp(DateTime::TIMESTAMP_FULL));
            myFile.print(",");
            myFile.print(id);
            myFile.print(",");
            
            for(int i = 0; i<len; i++){
//                Serial.print(buf[i]);
//                Serial.print(",");

//              salje u file na sd karticu
//              da otvara file svakih poruka pa onda zapisuje
                myFile.print(buf[i]);
                myFile.print(",");

//              salje preko hc12
                HC12.print(buf[i]);
                HC12.print(",");
            }
            myFile.println();
            HC12.println();
            myFile.close();
        }
        
        
    
    
}

// END FILE
