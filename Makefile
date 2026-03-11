CC=gcc
CFLAGS=-Wall -Wextra -I./include -I./thirdparty/raylib-5.0_win64_mingw-w64/include/ -I./thirdparty/lua-5.4.2/include/ -I./thirdparty/miniaudio-0.11.25/ -Wswitch-enum -Werror=switch-enum -Wno-char-subscripts -Wno-sign-compare -Wno-type-limits -Wno-unused-variable
LDFLAGS=-L./thirdparty/raylib-5.0_win64_mingw-w64/lib -L./thirdparty/lua-5.4.2/
LIBS=-lraylib -lgdi32 -lwinmm -llua54

daw: src/*.c
	$(CC) $(CFLAGS) -O2 -o $@ src/*.c $(LDFLAGS) $(LIBS)

debug: src/*.c
	$(CC) $(CFLAGS) -ggdb -DDEBUG=1 -o daw-debug src/*.c $(LDFLAGS) $(LIBS)

all: daw debug


