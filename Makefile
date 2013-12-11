

all:
	g++ -o cancap packet.cc main.cc util.cc usb_if.cc -ggdb -O0 -lusb-1.0

clean:
	rm -f cancap

win32:
	i686-w64-mingw32-g++ -o cancap.exe packet.cc main.cc util.cc usb_if.cc -ggdb -O0 -I win_dep/libusb/ win_dep/libusb/libusb-1.0.dll  -static-libgcc -static-libstdc++ 
