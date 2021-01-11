
                             
const int interruptPinA = 2;     //PAS sensor signal A
const int interruptPinB = 3;     //PAS sensor signal B

volatile long pasPulseCount = 0;

int direction = 0;
float theta = 0;
float thetaPrevious = 0;
float pulsesPerRevolution = 20;  // pulses per revolution -48 for PAS with 12 magnets


void setup() 
{
  Serial.begin(115200);
  pinMode(interruptPinA, INPUT_PULLUP);
  pinMode(interruptPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinA), ISRPasSignalA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPinB), ISRPasSignalB, CHANGE);    
}

void loop() {
  theta = pasPulseCount*360/pulsesPerRevolution;
  theta = normalizeAngle(theta);
  Serial.println(direction);
  thetaPrevious = theta;
}

float normalizeAngle(float angle)
{
    while(angle <= -360.0)
        angle += 360.0;
    while(angle >= 360.0)
        angle -= 360.0;
    return angle;
}

void ISRPasSignalA() 
{
  bool signalPinB = digitalRead(interruptPinB);
  bool signalPinA = digitalRead(interruptPinA);
  if (signalPinB == LOW)
  {
    if (signalPinA == HIGH) 
    {
      pasPulseCount++;
      direction= 1;
    }
    else 
    {
      pasPulseCount--;
      direction= -1;
    }
  }
  else 
  {
    if (signalPinA == HIGH) 
    {
      pasPulseCount--;
      direction= -1;
    }
    else 
    {
      pasPulseCount++;
      direction= 1;
    }
  }
}

void ISRPasSignalB() 
{
  bool signalPinB = digitalRead(interruptPinA);
  bool signalPinA = digitalRead(interruptPinB);
  if (signalPinA == LOW) {
    if (signalPinB == HIGH) {
      pasPulseCount--;
      direction= -1;
      
    }
    else {
      pasPulseCount++;
      direction= 1;
    }
  }

  else {
    if (signalPinB == HIGH) {
      pasPulseCount++;
      direction= 1;
    }
    else {
      pasPulseCount--;
      direction= -1;
    }
  }
}
