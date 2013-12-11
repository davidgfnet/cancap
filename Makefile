

all:
	g++ -o cancap packet.cc main.cc util.cc usb_if.cc -ggdb -O0 -lusb-1.0

clean:
	rm -f cancap
