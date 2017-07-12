import json
while 1:
	
	data = json.loads(raw_input())['data']
	if data.get("luminocity") != None: 
		lum = data.get("luminocity")

		if lum < 200:
			print "luminosity " + str(lum) + " is ok"
		else:
			print "WARNING  high luminosity " + str(lum)
	if data.get("btn") != None:
		btn = data.get("btn")
		state = data.get("state")
		print "button  " + str(btn) + "was " + str(state)
