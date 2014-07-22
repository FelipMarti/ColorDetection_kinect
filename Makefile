CC=g++
CFLAGS+=`pkg-config --cflags opencv`
LDFLAGS+=`pkg-config --libs opencv`
INCLUDE = -I/usr/local/include/libfreenect -I /usr/include/libusb-1.0
FREE_LIBS = -L/usr/local/lib -lfreenect
SOURCES=openkinect_OpenCvExample.cpp color_detection_kinect.cpp
OBJS=$(SOURCES:.cpp=.o)
PROG=color_detection_kinect

all: $(SOURCES) $(PROG)

$(PROG): $(OBJS)
	$(CC) -o $(PROG) $(OBJS) $(LDFLAGS) $(FREE_LIBS)

.cpp.o:
	$(CC) $(INCLUDE) -c $(CFLAGS) $<

clean:
	rm -f $(OBJS) $(PROG)

