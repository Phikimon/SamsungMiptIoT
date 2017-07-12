import json
import time
import paho.mqtt.client as mqtt
import lamp

LIGHTING_ID = "807B8590200001E4"

# The callback for when the clien receives a CONNACK response from the server.
def on_connect(client, userdata, rc ,arrrrg = 0):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    client.subscribe("devices/lora/+/opt3001")

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
     

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("artik.vdi.mipt.ru", 1883, 60)

# Blocking call that processes network traffic, dispatches callbacks and
# handles reconnecting.
# Other loop*() functions are available that give a threaded interface and a
# manual interface.

client.loop_start()

man_state = "off"
while 1:
        cfg_file = open( "config.json", "r")
        cfg_str = json.loads(cfg_file.read())["lighting"]
        cfg_file.close()
        file = open("dev.json", "r")
        lamps = lamp.load_lamp_array(json.loads(file.read())["map"], client)
        file.close()
        
        if "manual" == cfg_str["mode"]:
            if man_state != cfg_str["manual"]["state"]:
                man_state = cfg_str["manual"]["state"]
                for i in lamps:
                    i.set_state(man_state == "on")
                    print i.state
        time.sleep(10)        
            


