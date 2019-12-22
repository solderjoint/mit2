#!/usr/bash

wc \
	src/*.h src/*.c src/*.ld \
       	src/logic/*.h src/logic/*.c \
       	src/periph/*.h src/periph/*.c \
       	src/system/*.h src/system/*.c \
       	src/util/*.h src/util/*.c

