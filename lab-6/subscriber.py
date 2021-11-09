# references: http://www.steves-internet-guide.com/subscribing-topics-mqtt-client/

import paho.mqtt.client as mqtt

def on_message(client, userdata, message):
    print("message received " ,str(message.payload.decode("utf-8")))
    print("message topic=",message.topic)
    print("message qos=",message.qos)
    print("message retain flag=",message.retain)
    print()

print("creating new instance")
client = mqtt.Client("Subscriber1")

client.on_message=on_message

print("connecting to broker")
broker_address = '127.0.0.1'
client.connect(broker_address)

client.subscribe('hello/v1')

while True:
    try:
        client.loop()
    except:
        break
