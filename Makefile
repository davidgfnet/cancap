

all:
	g++ -o cancap packet.cc main.cc util.cc -ggdb -O0

clean:
	rm -f cancap
