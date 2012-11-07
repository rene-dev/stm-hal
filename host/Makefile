all: host

host: host.cpp
	clang++ host.cpp `pkg-config libusb-1.0 --cflags --libs` -o host

clean:
	rm -rf host
