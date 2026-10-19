all: build upload

build:
	pio run

upload:
	pio run -t upload

ide:
	pio run -t compiledb

monitor:
	pio device monitor -p /dev/ttyACM0 -b 115200

pinit:
	pio project init --ide vim -d . --sample-code --board esp32-s3-devkitc-1
