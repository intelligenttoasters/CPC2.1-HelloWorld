default: com run

go: com real

com:
	gcc -o cmodem main.c cmodem.c

run:
	./cmodem test /dev/ptmx | od -tx1a

real:
	./cmodem HelloWorld.rbf /dev/ttyS7


