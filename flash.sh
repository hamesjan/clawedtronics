#!/bin/bash
cd ~/esp/esp-idf && . ./export.sh
cd ~/projects/clawedtronics/hello_world
idf.py -p /dev/cu.usbmodem101 flash
echo ""
echo "Flash done. Unplug and replug USB, then run ./monitor.sh"
