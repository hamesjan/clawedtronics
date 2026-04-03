# /bin/bash
cd ~/esp/esp-idf
. ./export.sh
cd ~/projects/clawedmachine/mdb-master-esp32s3
idf.py -p /dev/cu.usbmodem1101 flash monitor
