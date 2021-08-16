# Código de ejemplo AdafruitIO
# Universidad del Valle de Guatemala
# IE3027 - Electrónica Digital 2
# Editado por Gerardo Fuentes

# Adafruit IO
# https://io.adafruit.com/

# if Module not Found. Open Terminal/CMD and execute:
# pip3 install Adafruit_IO

from Adafruit_IO import Client, RequestError, Feed
import serial
import time


ser = serial.Serial('COM3', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_YoVh965uubGIbGR9Qt5VK1TFtZHB"
ADAFRUIT_IO_USERNAME = "YRAR"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)
while(1):
    #Readval pyserial
    dataser0 = ser.readline(4)
    dataser1 = int(float(dataser0))

    #Digital1 Feed
    digital1_feed = aio.feeds('temp')
    aio.send_data(digital1_feed.key, dataser1)
    digital_data1 = aio.receive(digital1_feed.key)
    print(f'digital signal: {digital_data1.value}')

    #Digital2 Feed
    digital2_feed = aio.feeds('humidity')
    aio.send_data(digital2_feed.key, 21)
    digital_data2 = aio.receive(digital2_feed.key)
    print(f'digital signal: {digital_data2.value}')

    time.sleep(10)

    
#VALOR A RECIBIR
#digital3_feed = aio.feeds('valor')
#digital_data3 = aio.receive(digital3_feed.key)
#dd30 = int(digital_data3)
#dd3 = chr(dd30)
#ser.write(dd3)
