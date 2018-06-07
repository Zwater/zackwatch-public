#!/bin/sh
export CC=arm-none-eabi-gcc

MAKEFILE="makefile.ycm"
if [ -f .ycm_extra_conf.py ]; then
  rm -f .ycm_extra_conf.py
fi
pebble clean 1>/dev/null 2>&1
echo 'all:' > $MAKEFILE
pebble build -v 2>&1 | grep runner >> $MAKEFILE
sed \
	-e 's/\[//g' \
	-e 's/\]//g' \
	-e "s/ u'/ '/g" \
	-e "s/'//g" \
	-e "s/, / /g" \
	-e 's/^.*runner/\tcd build;/g' \
	-e 's/-D__FILE_NAME__="\([^"]*\)"/-D__FILE_NAME__=\\"\1\\"/g' \
	-e 's/arm-none-eabi-gcc/$(CC)/g' \
	-i $MAKEFILE
make -f $MAKEFILE
~/.vim/bundle/YCM-Generator/config_gen.py -b make -M="-f $MAKEFILE" .
sed \
	-e '/-D__FILE_NAME__/d' \
	-e '/-DPBL_DISPLAY_WIDTH/d' \
	-e '/-DPBL_DISPLAY_HEIGHT/d' \
	-i .ycm_extra_conf.py
rm $MAKEFILE
