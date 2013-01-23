import time, serial, sys, feedparser

#Settings - Change these to match your account details
USERNAME="barton.satchwill@cybera.ca"
PASSWORD="m&gn3t0sph3r3"
PROTOCOL="https://"
SERVER="mail.google.com"
PATH="/gmail/feed/atom"

SERIALPORT = 'COM3' # Change this to your serial port!
SERIALPORT = '/dev/tty.usbmodemfd121' # Change this to your serial port!


if len(sys.argv) == 3:
	USERNAME = sys.argv[1]
	PASSWORD = sys.argv[2]
else:
	print "need email address and password"
	exit()

# Set up serial port
try:
	ser = serial.Serial(SERIALPORT, 9600)
except serial.SerialException:
	print "Could not open serial port %s!" % SERIALPORT
	sys.exit()

while True:
	panic = False

	response = feedparser.parse(PROTOCOL + USERNAME + ":" + PASSWORD + "@" + SERVER + PATH)

	try:
		newmails = int(response["feed"]["fullcount"])
	except KeyError:
		print "Could not get email.  Check you address and password"
		ser.write('X')
		sys.exit()


	# nagios@nova-ab.dair-atir.canarie.ca
	# to:	 dair-sysadmins@cybera.ca
	# subject:	 [dair-sysadmins] ** PROBLEM Service Alert: dair-ua-c08/nova-compute is CRITICAL **
	for i in range(0, newmails):
		author = response['items'][i]['authors'][0].name
		subject = response['items'][i].title
		# print "    %s : %s" % (author, subject) 
		if "nagios" in author and "CRITICAL" in subject:
			panic = True

	# Output data to serial port
	if panic:
		print "heads up!"
		ser.write('M')
	else: 
		print "relax, it's cool."
		ser.write('N')

	time.sleep(45)

ser.close()
