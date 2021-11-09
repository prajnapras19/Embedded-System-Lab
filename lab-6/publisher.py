import serial
import time
import json
import paho.mqtt.client as mqtt

print("creating new instance")
client = mqtt.Client("Publisher1")

#username = ''
#password = ''
#client.username_pw_set(username, password)

print("connecting to broker")
broker_address = '127.0.0.1'
client.connect(broker_address)

ser = serial.Serial(
    port='/dev/ttyACM0',
    baudrate=9600,
    parity=serial.PARITY_ODD,
    stopbits=serial.STOPBITS_TWO,
    bytesize=serial.SEVENBITS
)

buffer = ser.read(ser.in_waiting)
client.loop_start()
while True:
    try:
        s = ser.read_until().decode().strip()
        print('s:', s)
        data = {"msg" : s}
        client.publish("hello/v1", json.dumps(data))
    except:
        break
