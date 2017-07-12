class lamp:
    id = ""
    mqtt_client = None
    status = True
    state = 0
    is_lamp = False

    def __init__(self, m_dict, mqtt):
        self.id = m_dict["ID"]
        self.mqtt_client = mqtt
        if m_dict["name"] == "lamp":
            self.is_lamp = True
        
    def turn_on(self):
        self.state = 1
        self.mqtt_client.publish("devices/lora/" + self.id + "/gpio", "set 17 1")
    def turn_off(self):
        self.state = 0
        self.mqtt_client.publish("devices/lora/" + self.id + "/gpio", "set 17 0")
    def set_state(self, stt):
        self.state = int(stt)
        self.mqtt_client.publish("devices/lora/" + self.id + "/gpio", "set 17 " + str(int(stt)))
        
def load_lamp_array(map_array, client = None):
    res = []
    for i in map_array:
        unit = lamp(i["controller"], client)
        if unit.is_lamp:
            res.append(unit)
    return res

            
        
        
                 
                 
    
