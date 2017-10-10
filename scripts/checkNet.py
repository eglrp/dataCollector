import os
import time
while(1)
    time.sleep(30)
    os.system("ifconfig eth0 192.168.1.255 netmask 255.255.255.0")
    os.systme("route add 192.168.1.201 eth0")