

all:
	g++ -o cancap packet.cc main.cc util.cc usb_if.cc -ggdb -O0 -lusb-1.0 -lpthread

clean:
	rm -f cancap

win32:
	# Compile with libusbx (build in mingw32) statically
	i686-w64-mingw32-g++ -o cancap.exe packet.cc main.cc util.cc usb_if.cc -ggdb -O0 -Wl,-static -lusb-1.0  -static-libgcc -static-libstdc++ -lpthread
	# Strip shitty symbols and stuff
	i686-w64-mingw32-strip --strip-all cancap.exe

	#i686-w64-mingw32-g++ -o cancap.exe packet.cc main.cc util.cc usb_if.cc -ggdb -O0 -I win_dep/libusb/ win_dep/libusb/libusb-1.0.dll  -static-libgcc -static-libstdc++ 
