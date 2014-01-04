cancap
======

CANCAPture from SBACOM to PCAP format

Instructions
------------

To install windows driver you should use:

http://zadig.akeo.ie/

Download your version (there are XP and Vista/W7 versions).
Now plug the device. If the install assistant appears, close it.
Now choose the device in the Zadig device list, and install a driver.
Any driver should do, I choose WinUSB, but it's up to you.

Once the driver is properly installed, it should appear in the Device
Manager. Now you're ready to go.


Run the program like this:
cancap.exe usb outfile.txt

If you want to close it you should use Ctrl+C. It *should* gracefully stop
it, otherwise the driver will get stuck and it will be impossible to attach
again to the device (if this happens, unplug and replug the device).

On Linux, everything works out of the box, no issues at all :D


Named Pipes
-----------

You can use Named Pipes to perform real time capturing on Wireshark (or any other
application supporting named pipes). On Linux you probably know how to do it
already, just create a name pipe (using mknod) and use it as output file.

On Windows you just need to specify a proper pipe filename in the format:

\\.\pipe\mynamedpipe

This way the output will go into a named instead of a file. CanCap will recognize
the filename and automatically create a Named Pipe instead of a file.

