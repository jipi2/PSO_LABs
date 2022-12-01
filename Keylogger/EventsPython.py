# get-keyboard-event-file.py

import struct
import sys
from datetime import datetime

def GetKeyboardEventFile(tokenToLookFor):
	section = ""
	line = ""
	eventName = ""

	fp = open ("/proc/bus/input/devices", "r")
	done = False
	while False == done:
		line = fp.readline()
		if line:
			if "" == line.strip():
				if -1 != section.find(tokenToLookFor) and -1 == section.lower().find("mouse"):
					print ("Found [" + tokenToLookFor + "] in:\n" + section)
					lines = section.split('\n')
					for sectionLine in lines:
						if -1 != sectionLine.strip().find("Handlers=") and "" == eventName:
							print ("Found Handlers line: [" + sectionLine + "]")
							sectionLineParts = sectionLine.strip().split(' ')
							eventName = sectionLineParts[-1]
							print ("Found eventName [" + eventName + "]")
							done = True
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
	keyboardEventFile = ""
	try:
		keyboardEventFile = GetKeyboardEventFile("EV=120013");
		print ("Keyboard Event File is [" + keyboardEventFile + "]")
	except BaseException as err:
		print ("Couldn't get the keyboard event file due to error [" + err + "]")
	return 0

if "__main__" == __name__:
	main(sys.argv[1:])
