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


#ser = serial.Serial('COM3', baudrate = 9600)

ADAFRUIT_IO_KEY = "aio_dgRm40ICNrv9QYebCXa3XM1Xybld"
ADAFRUIT_IO_USERNAME = "YRAR"
aio = Client(ADAFRUIT_IO_USERNAME, ADAFRUIT_IO_KEY)

while(1):
    #ser.close()
    #ser.open()
    #Readval pyserial
    #dataser0 = str(ser.read(9))
    #print(dataser0)
    #dataser1 = dataser0.split(',')
    #d1 = dataser1[1]

    #print(d1)

#VALORES ENVIAR

    #LIGHT
    light_feed = aio.feeds('light')
    aio.send_data(light_feed.key, 1)

    #DOOR
    door_feed = aio.feeds('door')
    aio.send_data(door_feed.key, 0)

    #LOCK
    lock_feed = aio.feeds('lock')
    #aio.send_data(lock_feed.key, 1)


#VALOR A RECIBIR

    #LIGHT
    light_state = aio.receive(light_feed.key)
    print(f'LIGHT: {light_state.value}')

    #DOOR
    door_state = aio.receive(door_feed.key)
    print(f'DOOR: {door_state.value}')

    #LOCK
    lock_state = aio.receive(lock_feed.key)
    print(f'LOCK: {lock_state.value}')




    #d3 = int(digital_data3.value)
    #dat3 = d3.to_bytes(3, 'big')
    #ser.write(dat3)
    time.sleep(5)
