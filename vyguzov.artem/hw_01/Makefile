CFLAGS:=-Wall -Wextra -I include

.PHONY: all
all: hw_01

hw_01: obj/hw_01.o obj/bmp.o obj/stego.o
	gcc obj/hw_01.o obj/bmp.o obj/stego.o -o hw_01
	
obj:
	mkdir obj

obj/hw_01.o: src/main.c include/bmp.h include/stego.h | obj
	gcc -c $(CFLAGS) src/main.c -o obj/hw_01.o
	
obj/bmp.o: src/bmp.c include/bmp.h | obj
	gcc -c $(CFLAGS) src/bmp.c -o obj/bmp.o
	
obj/stego.o: src/stego.c include/stego.h include/bmp.h | obj
	gcc -c $(CFLAGS) src/stego.c -o obj/stego.o
	
.PHONY: clean
clean:
	rm -rf hw_01 obj
