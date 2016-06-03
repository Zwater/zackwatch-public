#!/bin/bash
#check arp table first
ip=`ssh admin@192.168.1.1 "arp -a" | grep ${1} | awk '{print $2}' | tr -d '()'`
#ok not in arp table, check dhcp leases
if [[ -z $ip ]]; then
  ip=`ssh admin@192.168.1.1 "dumpleases -f /tmp/var/dnsmasq.leases -a -r" | grep ${1} | awk '{print $3}'`
fi
#ok maybe wrong column
if [[ $ip != 192* ]]; then
  ip=`ssh admin@192.168.1.1 "dumpleases -f /tmp/var/dnsmasq.leases -a -r" | grep ${1} | awk '{print $4}'`
fi
#echo $ip
#arp=`ssh admin@192.168.1.1 "arp -a ${ip}"`
#if [[ $arp == "No match found"* ]]; then
#    echo 0 
#else 
#    echo 1 
#fi
#ok use router to ping so to populate arp for next time
ssh admin@192.168.1.1 "ping -c 1 ${ip}"  > /dev/null 2>&1
if [ $? -ne 0 ]; then
        echo 0
else
        echo 1
fi

