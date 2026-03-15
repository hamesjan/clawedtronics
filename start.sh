# /bin/bash
cd ~/esp/esp-idf
. ./export.sh
cd ~/projects/clawvendingmachine/hello_world
idf.py -p /dev/cu.usbmodem1101 flash monitor
