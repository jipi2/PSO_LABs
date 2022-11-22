# get-keyboard-event-file.py

import struct
import sys
from datetime import datetime

def GetKeyboardEventFile(tokenToLookFor):
	# Any exception raised here will be processed by the calling function.
	section = ""
	line = ""
	eventName = ""

	fp = open ("/proc/bus/input/devices", "r")
	done = False
	while False == done:
		line = fp.readline()
		if line:
			#print (line.strip())
			if "" == line.strip():
				#print ("\nFound Section:\n" + section)
				if -1 != section.find(tokenToLookFor) and -1 == section.lower().find("mouse"):
					# It is entirely possible there to be multiple devices
					# listed as a keyboard. In this case, I will look for
					# the word "mouse" and exclude anything that contains
					# that. This section may need to be suited to taste
					print ("Found [" + tokenToLookFor + "] in:\n" + section)
					# Get the last part of the "Handlers" line:
					lines = section.split('\n')
					for sectionLine in lines:
						# The strip() method is needed because there may be trailing spaces
						# at the end of this line. This will confuse the split() method.
						if -1 != sectionLine.strip().find("Handlers=") and "" == eventName:
							print ("Found Handlers line: [" + sectionLine + "]")
							sectionLineParts = sectionLine.strip().split(' ')
							eventName = sectionLineParts[-1]
							print ("Found eventName [" + eventName + "]")
							done = True
				# Adding this section to show the extra section containing EV=120013
				elif -1 != section.find(tokenToLookFor) and -1 != section.lower().find("mouse"):
					print ("Found [" + tokenToLookFor + "] in the section below, but " +
						"it is not the keyboard event file:\n" + section)
				section = ""
			else:
				section = section + line
		else:
			done = True
	fp.close()

	if "" == eventName:
		raise Exception("No event name was found for the token [" + tokenToLookFor + "]")

	return "/dev/input/" + eventName

def main(argv):
	# Need to add code which figures out the name of this file from 
	# /proc/bus/input/devices - Look for EV=120013
	# Per Linux docs, 120013 is a hex number indicating which types of events
	# this device supports, and this number happens to include the keyboard
	# event.
	keyboardEventFile = ""
	try:
		keyboardEventFile = GetKeyboardEventFile("EV=120013");
		print ("Keyboard Event File is [" + keyboardEventFile + "]")
	except BaseException as err:
		print ("Couldn't get the keyboard event file due to error [" + err + "]")
	return 0

if "__main__" == __name__:
	main(sys.argv[1:])
