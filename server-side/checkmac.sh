#!/bin/bash
ip=`ssh admin@192.168.0.1 "dumpleases -f /tmp/var/dnsmasq.leases -a -r| grep '${1}'" | awk '{print $3}'`
#arp=`ssh admin@192.168.0.1 "arp -a ${ip}"`
#if [[ $arp == "No match found"* ]]; then
#    echo 0 
#else 
#    echo 1 
#fi
ping -c 1 ${ip}  > /dev/null 2>&1
if [ $? -ne 0 ]; then
	echo 0
else
	echo 1
fi
