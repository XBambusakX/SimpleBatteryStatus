all:
	g++ -o SimpleBatteryStatus ./src/* `pkg-config --cflags --libs sdl3`
