// Quadrature Signal from Sensors (PAS/ Encoder). Encoder (Kuebler) signals A & B detected and outputsrotations 
const byte interruptPinA = 2;     //Signal A
const byte interruptPinB = 3;     //Signal B

volatile long EncoderCount = 0;

float Theta =0;
float Theta_prev =0;

float ppr = 0;        // pulses per revolution

void ISR_EncoderA() {                                 // Attach interrrupt function for change in signal A
  bool PinB = digitalRead(interruptPinB);
  bool PinA = digitalRead(interruptPinA);

  if (PinB == LOW) {
    if (PinA == HIGH) {
      EncoderCount++;
    }
    else {
      EncoderCount--;
    }
  }

  else {
    if (PinA == HIGH) {
      EncoderCount--;
    }
    else {
      EncoderCount++;
    }
  }
}



void ISR_EncoderB() {                                    // Attach interrrupt function for change in signal B
  bool PinB = digitalRead(interruptPinA);
  bool PinA = digitalRead(interruptPinB);

  if (PinA == LOW) {
    if (PinB == HIGH) {
      EncoderCount--;
    }
    else {
      EncoderCount++;
    }
  }

  else {
    if (PinB == HIGH) {
      EncoderCount++;
    }
    else {
      EncoderCount--;
    }
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(interruptPinA, INPUT_PULLUP);
  pinMode(interruptPinB, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPinA), ISR_EncoderA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPinB), ISR_EncoderB, CHANGE);
}

void loop() {
  Theta = EncoderCount*360/ppr;
  Theta_prev = Theta ;

  if(Theta>0){                    //Normalization of angles to keep it between-360 and 360 degrees
    Theta = normalize_angle(Theta);
  }
  else if (Theta<0){
    Theta = negnormalize_angle(Theta);
  }
  Serial.println(Theta,2);
}

float normalize_angle(float angle)
{
    float k = angle;

    while(k < 0.0)
        k += 360.0;
    while(k >= 360.0)
        k -= 360.0;
    return k;
}

 float negnormalize_angle(float angle)
{
    float k = angle;

    while(k > 0.0)
        k -= 360.0;
    while(k <= -360.0)
        k += 360.0;
    return k;
}
