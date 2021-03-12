/*
This example takes range measurements with the VL53L1X and displays additional 
details (status and signal/ambient rates) for each measurement, which can help
you determine whether the sensor is operating normally and the reported range is
valid. The range is in units of mm, and the rates are in units of MCPS (mega 
counts per second).
*/

#include <Wire.h>
#include <VL53L1X.h> // biblioteca para o sensor

#define _LIDAR_COUNT 2 // quantidade de sensores

VL53L1X sensor[_LIDAR_COUNT]; // vetor com _LIDAR_COUNT referencias

#define _PIN_XSHUT_START_NUM 5 // start PIN number on Arduino
#define _I2C_ADRESS_START_HEX 0x30 // start Adress I2C(SDA,SCL)

void setup()
{

  pinModeInit(); // initial lidar xshut pin

  Serial.begin(9600); // original 115200
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  Wire.beginTransmission(0x29);

  setl2CAddressMode();

  Serial.print("I2C DistanceWn");
  Serial.println();
  Serial.print("-------------");
  Serial.println();
}

// função de repetição
void loop()
{

  uint16_t sensorDistance = 0; // (mm)

  for (uint8_t i = 0; i < _LIDAR_COUNT; i++){
    
      sensor[i].read(); 
      
      Serial.print("Status: ");
      Serial.println(sensor[i].ranging_data.range_status); // status do sensor (Ler domumentação para mais informações.

      sensorDistance = sensor[i].ranging_data.range_mm; // atribui o valor da distância lida para sensorDistance
      Serial.print("Distancia: ");
      Serial.print(sensorDistance);
      Serial.println(" mm");
  }

  Serial.println(" ----- ");
  Serial.println();

  delay(100);
  
  /* if single sensor
  
  Serial.print("range: ");
  Serial.print(sensor.ranging_data.range_mm);
  Serial.print("\tstatus: ");
  Serial.print(VL53L1X::rangeStatusToString(sensor.ranging_data.range_status));
  Serial.print("\tpeak signal: ");
  Serial.print(sensor.ranging_data.peak_signal_count_rate_MCPS);
  Serial.print("\tambient: ");
  Serial.print(sensor.ranging_data.ambient_count_rate_MCPS);
  */
}


void pinModeInit(){
  // seta low para os pinos referentes ao XSHUT no sensor.
  // nesse caso, eles começam com o pino _PIN_XSHUT_START_NUM e terminam
  // em _PIN_XSHUT_START_NUM + _LIDAR_COUNT
  for(uint8_t i = 0; i < _LIDAR_COUNT; i++){
      pinMode(_PIN_XSHUT_START_NUM + i, OUTPUT); // reset port
      digitalWrite(_PIN_XSHUT_START_NUM+i, LOW); // init low
  }
}

void setl2CAddressMode(){
    for (uint8_t i = 0; i < _LIDAR_COUNT; i++){
        digitalWrite(_PIN_XSHUT_START_NUM+i, HIGH); // reset Port Enable |enable XSHUT | that is Reset I2C Address

        sensor[i].init();
        sensor[i].setAddress(_I2C_ADRESS_START_HEX+i); // assign l2c Adress  (SDA, SCL)

        sensor[i].setDistanceMode(VL53L1X::Long);
        sensor[i].setMeasurementTimingBudget(4000); // max distance is 4000 mm
        sensor[i].startContinuous(50); // 50 ms or 10 ms
        sensor[i].setTimeout(100);
    }
}
