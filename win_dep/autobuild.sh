
# Get the source from the git repo
git clone git://git.libusb.org/libusb.git

# Now configure and build
cd libusb

./autogen.sh
./configure --host=i686-w64-mingw32 --build=i686-w64-mingw32

# Now build!
make -j3

# The result file is here:
cp ./libusb/.libs/libusb-1.0.dll libusb-1.0.dll

