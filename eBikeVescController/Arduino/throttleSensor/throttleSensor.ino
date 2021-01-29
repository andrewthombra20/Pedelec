#include <VescUart.h>

VescUart UART;

const int numReadings = 40;
const int inputPin    = A0;
const float maxDutyCycle = 0.7;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
float average = 0;              // the average
float averageOld = 0;                
float dutyCycle = 0;
float filteredValue = 0;

void setup() {
  Serial.begin(115200);
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
  Serial2.begin(115200);
  while (!Serial2) {;}
  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial2);
}

void loop() {
  total = total - readings[readIndex];
  readings[readIndex] = analogRead(inputPin);
  total = total + readings[readIndex];
  readIndex = readIndex + 1;

  if (readIndex >= numReadings) {
    readIndex = 0;
  }

  average = abs(total) / numReadings;
  filteredValue = (1.0-0.8) * average   +   0.8 * averageOld;   

  if (filteredValue < 250){
    filteredValue = 250;
  }
  else if (filteredValue > 800) 
  {
    filteredValue = 800;
  }
  dutyCycle = mapLinear(filteredValue, 250 , 800.0, 0.0, maxDutyCycle);     
  Serial.println(dutyCycle);
  UART.setDuty(dutyCycle);
  averageOld = average;
  delay(1);        
}


double mapLinear(float x, float in_min, float in_max, float out_min, float out_max)        
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

 
