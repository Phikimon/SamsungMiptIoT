import paho.mqtt.client as mqtt
import json
import datetime




def sendConfigToServer(idDevice, modules, conf):
    client = mqtt.Client()
    url = "artik.vdi.mipt.ru"
    client.connect(url, 1883, 60)

    for module in modules:
        cmd = "mod " + module["name"] + " 1"
        device = "devices/lora/"+idDevice 
        client.publish(device + "/config", cmd, 1)
        print module["data"]["names"][0]
        actualConfigs = {}
        for prop in module["data"]["names"]:
            if prop == "luminocity":
                actualConfigs["sensor_period"] = conf["lighting"]["sensor_period"]
            if prop == "lamp":
                print conf["lighting"]
            if prop == "pump":
                print conf["watering"]
            if prop == "humidity":
                actualConfigs["sensor_period"] = conf["lighting"]["sensor_period"]

        print actualConfigs
        for config in actualConfigs:
            cmd = "set " + config +  " " + str(actualConfigs[config])
            client.publish(device, cmd, 1)
    cmd = "save"
    client.publish(device + "/config", cmd, 1)
    cmd = "reboot"
    client.publish(device + "/config", cmd, 1)
    client.disconnect()




def log(message):
    now = datetime.datetime.now()
    with open("AC.log", "a") as fileLog:
        fileLog.write(str(now) + " : " + message + "\n")

def run(idDevice = "0"):
    log("Recieved new id device " + str(idDevice))
    if idDevice == "0":
        return

    modules = []
    with open("dev.json", "r") as devFile:
        jsonDev = json.loads(devFile.read())
        for device in jsonDev["devices"]:
            if idDevice == device["ID"]:
                modules.append(device["modules"])

    jsonConf = {}
    with open("config.json", "r") as configFile:
        jsonConf = json.loads(configFile.read())


    #print modules[0]
    #print jsonConf
    sendConfigToServer(idDevice, modules[0], jsonConf)

def on_connect(client, userdata, rc, unUsable = 0):
    log("Connected with result code " + str(rc))
    client.subscribe("CM_AC")

def on_message(client, userdata, msg):
    run(msg.payload)



if __name__ == "__main__":
    run("807B8590200001E4")
else:
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect("127.0.0.1", 1883, 60)

    client.loop_forever()



