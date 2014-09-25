CC=gcc
CFLAGS=-c -g -Wall -O3
LDFLAGS=-lwiringPi
SOURCES=main.c
OBJECTS=$(SOURCES:%.c=%.o)
EXECUTABLE=PiPWMTone

all: $(SOURCES) $(EXECUTABLE)
	sudo chown root $(EXECUTABLE)
	sudo chmod u+s $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

clean:
	rm -f *.o $(EXECUTABLE)

copy:
	echo "Copying from RPi"
	rsync -aLv pi@developi:/opt/vc ~/Developer/RPi/opt/
	rsync -aLv pi@developi:/usr/include ~/Developer/RPi/usr/
	rsync -aLv pi@developi:/usr/local/include ~/Developer/RPi/usr/local
	echo "Copying to RPi"
	rsync -aLv -f"- .*" ${SRCROOT}/${PRODUCT_NAME} pi@developi:~/Developer
	#ssh pi@developi "cd ~/Developer/${PRODUCT_NAME};./configure;make"
	ssh pi@developi "cd ~/Developer/${PRODUCT_NAME};make"