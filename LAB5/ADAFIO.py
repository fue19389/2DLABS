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


ser = serial.Serial('COM6', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_YoVh965uubGIbGR9Qt5VK1TFtZHB"
ADAFRUIT_IO_USERNAME = "YRAR"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    ser.close()
    ser.open()
    #Readval pyserial
    dataser0 = str(ser.read(5))
    print(dataser0)
    dataser1 = dataser0.split(',')
    d1 = dataser1[1]

    print(d1)


    #Digital1 Feed
    digital1_feed = aio.feeds('temp')
    aio.send_data(digital1_feed.key, d1)
    digital_data1 = aio.receive(digital1_feed.key)
    print(f'temperatura: {digital_data1.value}')

    #Digital2 Feed
    digital2_feed = aio.feeds('humidity')
    aio.send_data(digital2_feed.key, 72)
    digital_data2 = aio.receive(digital2_feed.key)
    print(f'humedad: {digital_data2.value}')


#VALOR A RECIBIR
    digital3_feed = aio.feeds('valor')
    digital_data3 = aio.receive(digital3_feed.key)
    print(f'valor de barra: {digital_data3.value}')
    d3 = int(digital_data3.value)
    dat3 = d3.to_bytes(3, 'big')
    ser.write(dat3)
    time.sleep(1)
