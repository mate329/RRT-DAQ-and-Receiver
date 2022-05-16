// MR - receiver v0.1
#include "setupReceiver.h"

/*
MR - v1.4 - primanje podataka ->
TPS - 0x01
Temp motora - 0x02
Tlak ulja - 0x03
Temp ulja - 0x04
Lamda senzori - 0x05
RPM - 0x06
Accelerometar - 0x07
14 fuseova
Random adrese - nisu bitne
*/

//nakon sto se svi podaci dobiju, onda svaku varijablu posebno ispisati
//testirati na svojoj arduino plocici program za ispisivanje telemetrije

//namjestiti channel za komunikaciju izmedju bolida i receivera !!!!!!! komanda AT+C069 npr

void setup() {
  Serial.begin(9600);
  HC12.begin(9600);
}

void loop() {
  int tps;
  int engineTemperature;
  int oilPressure;
  int oilTemperature;
  int lambdaValue;
  int engineRPM;
  int accelerometer;
  
  while(HC12.available()){
    unsigned char canID = HC12.read();
    int value = HC12.read();
    
    switch(canID){ 
            case THROTTLE_POSITION_SENSOR_ID:{
              tps = value;
              break;
              }
            case ENGINE_TEMPERATURE_ID:{
              engineTemperature = value;
              break;
              }
            case OIL_PRESSURE_ID:{
              oilPressure = value;
              break;
              }
            case OIL_TEMPERATURE_ID:{
              oilTemperature = value;
              break;
              }
            case LAMBDA_SENSOR_ID:{
              lambdaValue = value;
            }
              break;
            case ENGINE_RPM_ID:{
              engineRPM = value;
              break;
            }
            case ACCELEROMETER_ID:{
              accelerometer = value;
              break;
              }
            default:{
              }
            }
/*STARCICU
// tu moras samo dodati sljedeci snippet
i napraviti iznad svega int <vrijednost>
case uredjaj_ID (koji definiras u .h fileu):{
              vrijednost_koju_cemo_slati_na_komp = value;
              break;
              }
*/

/* moguci problem
 *  ako HC12 cijelo vrijeme prima vrijednosti, onda nikad while nece zavrsiti sto znaci
 *  da se podaci nikad nece printati
 *  
 *  nakon testa vidjeti dal moze Serial.print biti izvan whilea tj oce li tako funkcionirati
 *  ako ne, onda se jednostavno Serial.print ubaci u while i sve bi trebalo bit ok
 */
    
    
    Serial.print(tps + ", ");
    Serial.print(engineTemperature + ", ");
    Serial.print(oilPressure + ", ");
    Serial.print(oilTemperature + ", ");
    Serial.print(engineRPM + ", ");
    Serial.print(accelerometer + ", ");
    Serial.println(lambdaValue);
    delay(50);
  }
}
