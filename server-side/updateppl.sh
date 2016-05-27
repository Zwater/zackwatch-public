#!/bin/sh
mate="mac-address"
daktak="daktak-dns-name"
m=`/usr/local/bin/checkmac.sh ${mate}`
r=`/usr/local/bin/checkmac.sh ${daktak}`
echo "{\"D\":\"${r}\", \"M\":\"${m}\"}" > /tmp/ppl.json
mv /tmp/ppl.json /var/www/localhost/htdocs
