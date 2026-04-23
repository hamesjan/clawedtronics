# /bin/bash
cd ~/esp/esp-idf
. ./export.sh
cd ~/projects/clawedtronics/hello_world
idf.py fullclean
idf.py -p /dev/cu.usbmodem1101 flash monitor
