#!/bin/sh
mate="mac-address"
daktak="daktak-dns-name"
m=`/usr/local/bin/checkmac.sh ${mate}`
d=`/usr/local/bin/checkmac.sh ${daktak}`
json="{\"D\":\"${d}\", \"M\":\"${m}\""
#you can also include remote people
#assuming remote-server has a cronned script updating file "user"
#u=`ssh user@remote-server "cat user"`
#if succeed
#if [ $? -eq 0 ]; then
  #if not null
#    json="${json}, \"U\":\"${u}\""
#fi
echo "${json} }" > /tmp/ppl.json
mv /tmp/ppl.json /var/www/localhost/htdocs
