import paho.mqtt.client as mqtt
import json
import datetime




def sendConfigToServer(idDevice, modules, conf):
    client = mqtt.Client()
    serverURL = "artik.vdi.mipt.ru"
    client.connect(serverURL, 1883, 60)

    for module in modules:
        cmd = "mod " + module["name"] + " " + "1"
        device = "devices/lora/" + idDevice 
        client.publish(device + "/config", cmd, 1)
        actualConfigs = {}
        for attribute in module["data"]["names"]:
            if attribute == "luminocity":
                actualConfigs["period"] = conf["lighting"]["sensor_period"]
            #if attribute == "lamp":
            #    print conf["lighting"]
            #if attribute == "pump":
            #    print conf["watering"]
            if attribute == "humidity":
                actualConfigs["period"] = conf["watering"]["sensor_period"]

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
                modules = device["modules"]

    jsonConf = {}
    with open("config.json", "r") as configFile:
        jsonConf = json.loads(configFile.read())

    sendConfigToServer(idDevice, modules, jsonConf)


def on_connect(client, userdata, rc, unusable = 0):
    log("Connected with result code " + str(rc))
    client.subscribe("CM_AC")

def on_message(client, userdata, msg):
    run(msg.payload)


def listenLocalServer():
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.connect("127.0.0.1", 1883, 60)

    client.loop_forever()

def tests():
    run("807B8590200001E4")



if __name__ == "__main__":
    tests()
else:
    listenLocalServer()



