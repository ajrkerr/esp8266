# /bin/bash

python tools/esptool/esptool.py --baud 460800 -p /dev/tty.usbserial write_flash 0x00000 alternate_firmwares/nodemcu/nodemcu_dev_0x00000.bin 0x10000 alternate_firmwares/nodemcu/nodemcu_dev_0x10000.bin
