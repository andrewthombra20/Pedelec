import sensorPAS as PAS
import RPi.GPIO as gpio
import time

sensor1 = PAS.sensorPAS(10,35,37)


while True:
        print(sensor1.getRPM())

