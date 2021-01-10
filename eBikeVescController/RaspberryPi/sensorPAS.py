import time
import RPi.GPIO as gpio

class sensorPAS:
    ticks = 0
    currentDirection = 0
    previousDirection = 0
    theta = 0
    rpm = 0
    elapsedTime = 0
    previousTimer = 0
    currentTimer = 0
    
    def __init__(self,ppr,pasSignalA,pasSignalB):
        self.ppr = ppr # Quadrature signal where 4 changes are detected in signal A and B for a pulse change
        self.pasSignalA = pasSignalA
        self.pasSignalB = pasSignalB
        
        gpio.setmode(gpio.BOARD)
        gpio.setup(self.pasSignalA, gpio.IN, pull_up_down=gpio.PUD_UP)
        gpio.setup(self.pasSignalB, gpio.IN, pull_up_down=gpio.PUD_UP)
        gpio.add_event_detect(self.pasSignalA, gpio.BOTH, callback=self.updateA,bouncetime=10)
        gpio.add_event_detect(self.pasSignalB, gpio.BOTH, callback=self.updateB,bouncetime=10)
    


    def updateA(self, channel):
        global currentTimer,elapsedTime,previousTimer
        if gpio.input(self.pasSignalB) == 0:
            if gpio.input(self.pasSignalA) == 1:
                self.currentTimer = int(round(time.time()*1000))
                self.elapsedTime = self.currentTimer - self.previousTimer
                self.ticks += 1
                self.currentDirection = 1
            else:
                self.ticks -= 1
                self.currentDirection = -1
        else:
            if gpio.input(self.pasSignalA) == 1:
                self.ticks -= 1
                self.currentDirection = -1
            else:
                self.ticks += 1
                self.currentDirection = 1
        self.previousTimer = self.currentTimer
                
    def updateB(self, channel):
        if gpio.input(self.pasSignalB) == 0:
            if gpio.input(self.pasSignalA) == 1:
                self.ticks -= 1
                self.currentDirection = -1
            else:
                self.ticks = 1
                self.currentDirection = 1
        else:
            if gpio.input(self.pasSignalA) == 1:
                self.ticks += 1
                self.currentDirection = 1
            else:
                self.ticks -= 1
                self.currentDirection = -1            
        
    def normalizeAngle(self):      
        while self.theta <= -360.0:
            self.theta += 360
        while self.theta >= 360.0:
            self.theta -= 360
            
    def getTheta(self):
        self.theta = self.ticks * 360/self.ppr*4
        return self.theta
    
    def getDirection(self):
        return self.currentDirection
    
    def getPulseCount(self):
        return self.ticks
    
    def getRPM(self):
        global elapsedTime
        if self.elapsedTime !=0:
            self.rpm = (360 /self.ppr) /self.elapsedTime
        else:
            self.rpm = 0
        return self.rpm
    