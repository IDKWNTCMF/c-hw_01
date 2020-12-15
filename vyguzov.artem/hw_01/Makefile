CFLAGS:=-Wall -Wextra -I include

.PHONY: all
all: hw_01

hw_01: obj/hw_01.o obj/bmp.o
	gcc obj/hw_01.o obj/bmp.o -o hw_01
	
obj:
	mkdir obj

obj/hw_01.o: src/main.c include/bmp.h | obj
	gcc -c $(CFLAGS) src/main.c -o obj/hw_01.o
	
obj/bmp.o: src/bmp.c include/bmp.h | obj
	gcc -c $(CFLAGS) src/bmp.c -o obj/bmp.o
	
.PHONY: clean
clean:
	rm -rf hw_01 obj
