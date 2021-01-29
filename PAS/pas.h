#ifndef pas_h
#define pas_h

#define pulsesPerRevolution = 20; // pulses per revolution -48 for PAS with 12 magnets

class pas{

    public:
    
    pas(const int interruptPinA, const int interruptPinB);

    int getDirection();
    int getPosition();
    int getPulseCount();
    float normalizeAngle(float angle);
    void resetPulsestoZero();

    protected:
    
    void isrPasSignalA();
    void isrPasSignalB();

    private:
    
    int direction = 0;
    int theta = 0;
    int thetaPrevious = 0;
    volatile long pasPulseCount = 0;

};

#endif
