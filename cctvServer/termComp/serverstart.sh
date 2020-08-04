ifconfig eth0 117.16.136.26 netmask 255.255.255.0 
route add default gw 117.16.136.254 eth0
insmod dipsw.ko
insmod camera.ko