# People Watching
## A watchface for people with way too much control over their roomate's internet connections.
This is a watchface for Pebble and Pebble Time watches, that shows the weather from forecast.io, as well as a group of indicators that light up depending on if the wearer's roomates or family are home, provided they've set up the required scripts and webpages.

The "people watching" functionality can be disabled in the watchface settings. at present, there's nothing to take it's place. Reccomendations for what should take it's place are welcome.

##Setup

Really, you can set this up however you like, depending on your situation. I personally use my DHCP server, with the lease time set fairly low, and parse it for only the active leases with [this script](http://askubuntu.com/a/553387) from askUbuntu

I feed that into a script that just reads the list, and then searches the list for each MAC address listed in a file, formatted with the MAC address, a tab, and an initial representing the person that MAC address belongs to. It's best to use their cell phone's MAC.

All that is required is a webpage with a JSON string that looks like this:

`{"Z":"1", "M":"0", "S":"1", "A":"0"}`

More detailed setup with multiple methods will come when I find the time to do a proper writeup.

APK to push battery status available [https://github.com/daktak/zackwatch_android](https://github.com/daktak/zackwatch_android/releases/download/v0.1/org.zackwatch.apk)

