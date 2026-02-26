CC = g++
CFLAGS = -Wall -std=c++11
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

radar_app: main.cpp serialib.cpp
	$(CC) $(CFLAGS) main.cpp serialib.cpp $(LIBS) -o radar_app

clean:
	rm -f radar_app