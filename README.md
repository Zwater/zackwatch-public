# People Watching
## A watchface for people with way too much control over their roomate's internet connections.

![Alt text](releases/zackwatch.png?raw=true "Watch")

This is a watchface for Pebble and Pebble Time watches, that shows the weather from forecast.io, as well as a group of indicators that light up depending on if the wearer's roomates or family are home, provided they've set up the required scripts and webpages.

The "people watching" functionality can be disabled in the watchface settings. at present, there's nothing to take it's place. Reccomendations for what should take it's place are welcome.

##Setup

Really, you can set this up however you like, depending on your situation. I personally use my DHCP server, with the lease time set fairly low, and parse it for only the active leases with [this script](http://askubuntu.com/a/553387) from askUbuntu

I feed that into a script that just reads the list, and then searches the list for each MAC address listed in a file, formatted with the MAC address, a tab, and an initial representing the person that MAC address belongs to. It's best to use their cell phone's MAC. My script looks like this:
```
#!/bin/bash

#the script from askUbuntu
hosts=$(python /home/zack/.scripts/home_detector/hostlist.py) 

#my list of names and MAC addresses
numPeople=$(wc -l /home/zack/.scripts/home_detector/people | awk '{print $1}') 
x=$numPeople

#empties files for writing new statuses
>/home/zack/.scripts/home_detector/current 
>/home/zack/.scripts/home_detector/status

#starts a JSON string
printf "{" >> /home/zack/.scripts/home_detector/current 

#builds a JSON string, iterating through my file, and comparing each line with what hostlist.py returned
while [ $x != 0 ] 
do
    who=$(sed $x'q;d' /home/zack/.scripts/home_detector/people)
    whoname=$(echo -n "$who" | awk '{print $2}' | tr -d '\r')
    echo -n $hosts | grep --ignore-case -q $(echo -n $who | awk '{printf $1}') && printf "\"$whoname\":\"1\", " >> /ho$
    x=$[$x-1]
done
current=$(cat /home/zack/.scripts/home_detector/current)

#chops off the last comma added by our while loop
final=$(echo ${current::-2}) 

#overwrites a webpage on my server that the watchface reads with the final product, and caps it off with a curly brace to make it a proper JSON string.
printf "$final" > /srv/http/people 
printf "}" >> /srv/http/people
```

All that is required is a webpage with a JSON string that looks like this:

`{"Z":"1", "M":"0", "S":"1", "A":"0"}`

Cron runs this script every minute on my server. A downside to my method is the use of fairly short leases on my DHCP server. this really isin't too much of a problem for me, because there are only four people on my network, however this would theoretically cause problems on large networks as devices constantly request new leases and clog up the network. Also, there's a maximum of about 8 minutes before this system will register a device as "away", as most phones seem to request a new lease about 2 minutes before their current one runs out.

An alternative method is included in this repository that uses the server's arp table to determine the presence of a device on the network, as well as using the router's built-in hosts page, accessable by SSH. My router dosen't support this(read: I don't know how yet), so I use my method. My method is a little more detailed and requires setting up a DHCP server, which some people may find out of their skillset or unable to do because of permissions and whatnot. 

More detailed setup with more methods will come when I find the time to do a proper writeup.

Thanks to [Github user daktak](https://github.com/daktak) for all the help creating this. This is my first ever attempt at anything in C and JavaScript, and they have put tremendous effort into making this project look like something done professionally.
