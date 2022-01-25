# SDK
The Sdk of Instrustar Series Oscilloscopes. The open source support ISDS205 ISDS210 ISDS220 and ISDS206 oscilloscopes.

######################demo######################

1,demo-VC
	
	a Demo written with VC
	
2,demo-Labview
	
	a Demo written with Labview,only test in windows

3,dome-Python
	
	a Demo written with Python,only test in windows

4,DllTest
	
	a  command line Demo written with c++, test in windows and ubuntu linux

######################linux######################

1,install libsub

	
	tar xvjf libusb-1.0.24.tar.bz2
	
	./configure --build=x86_64-linux --disable-udev
	
	make install / sudo make install
2,copy so "./linux/*.so" file to your system dynamic libraries

	like /lib or /usr/lib or /usr/lib/x86_64-linux-gnu
	
3,Compile dlltest and then run
	sudo ./DllTest

