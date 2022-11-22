#!/bin/bash
event="`sudo cat /proc/bus/input/devices | grep -Poz '("STK-7007F1"[\s\S]+?)\Kevent\d+'`"
echo $event #event26, for example