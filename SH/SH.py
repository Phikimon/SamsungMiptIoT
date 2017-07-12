import time, datetime, json
import paho.mqtt.client as mqtt

def on_connect(client, userdata, rc, nothing):
	print("Connected with result code "+str(rc))
	client.subscribe("devices/lora/#")###

def on_message(client, userdata, msg):
	try:
		data = json.loads(msg.payload)
		if 'data' in data:
			if ('luminocity' in data['data'] and 'humidity' in data['data']):
				f = open("./history", 'a')
				f.write("["+str(data['status']['devEUI'])+" ")
				f.write(str(data['status']['date'])+" ")
				f.write("luminocity+humidity]: ")
				f.write(str(data['data']['luminocity'])+" ")
				f.write(str(data['data']['humidity'])+"\n")
			elif ('luminocity' in data['data']):
				f = open("/run/media/student/Transcend/history", 'a')
				f.write("["+str(data['status']['devEUI'])+" ")
				f.write(str(data['status']['date'])+" ")
				f.write("luminocity]: ")
				f.write(str(data['data']['luminocity'])+"\n")
			elif ('humidity' in data['data']):
				f = open("/run/media/student/Transcend/history", 'a')
				f.write("["+str(data['status']['devEUI'])+" ")
				f.write(str(data['status']['date'])+" ")
				f.write("humidity]: ")
				f.write(str(data['data']['humidity'])+"\n")
			f.close()
	except:
		print("Not JSON")

client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

client.connect("artik.vdi.mipt.ru", 1883, 60)
client.loop_forever()
