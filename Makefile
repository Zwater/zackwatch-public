
# platform
#P="chalk"

VERSION=$(shell cat package.json | grep version | grep -o "[0-9][0-9]*\.[0-9][0-9]*")
NAME=$(shell cat package.json | grep '"name":' | head -1 | sed 's/,//g' |sed 's/"//g' | awk '{ print $2 }')

all: build install

build:
	pebble build

config:
	pebble emu-app-config --emulator $(PEBBLE_EMULATOR)

log:
	pebble logs --emulator $(PEBBLE_EMULATOR)

travis_build:
	yes | ~/pebble-dev/${PEBBLE_SDK}/bin/pebble build

install:
	pebble install --emulator $(PEBBLE_EMULATOR)

clean:
	pebble clean

size:
	pebble analyze-size

logs:
	pebble logs --emulator $(PEBBLE_EMULATOR)

screenshot:
	pebble screenshot --phone ${PEBBLE_PHONE}

deploy:
	pebble install --phone ${PEBBLE_PHONE}

.PHONY: all build config log install clean size logs screenshot deploy
