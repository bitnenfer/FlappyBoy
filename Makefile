CC = lcc -Wa-l -Wl-m -Wl-j -DUSE_SFR_FOR_REG
BUILD_PATH = build/gameboy/
all: flappyboy.gb

flappyboy.gb:
	-mkdir -p $(BUILD_PATH)
	$(CC) -c utils.c
	$(CC) -c main.c
	$(CC) -Wl -o $(BUILD_PATH)flappyboy.gb main.o utils.o
	rm *.lst
	rm *.o
	rm $(BUILD_PATH)*.map
	rm $(BUILD_PATH)*.sym