default: comp

all: comp

run: comp
	/usr/bin/gnome-terminal --profile=CPC -e '/bin/bash -c "ls -al; read"'

comp:
	gcc -o cmodem -Isrc src/main.c src/cmodem.c
	
com:
	picocom --send-cmd ./cmodem /dev/ttyACM?
