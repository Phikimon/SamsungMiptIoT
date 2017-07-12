from lamp import*
import json
file = open("dev.json", "r")
map = json.loads(file.read())["map"]

for i in load_lamp_array(map):
    print i.id
