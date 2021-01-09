#include <VescUart.h>

VescUart UART;

float dutyCycle = 0.1; /** The current in amps */

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  
  while (!Serial1) {;}

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial1);
}

void loop() {
  
  UART.setDuty(dutyCycle);
  // UART.setBrakeCurrent(dutyCycle);

  if ( UART.getVescValues() ) 
  {
    Serial.println(UART.data.rpm);
    Serial.println(UART.data.inpVoltage);
  }
  else
  {
    Serial.println("Failed to get data!");
  }
  
}
