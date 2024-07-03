# Makefile

exe: dns.o xml.o
	gcc xml.o dns.o -o exe -lmxml4

xml.o: xml.c
	gcc -c xml.c -o xml.o

dns.o: dns.c
	gcc -c dns.c -o dns.o

clean:
	rm -f *.o exe
