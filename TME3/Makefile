numcom:	main.o transmitter.o receiver.o
	gcc -o numcom main.o transmitter.o receiver.o

main.o: main.c transmitter.h receiver.h
	gcc -c main.c

transmitter.o: transmitter.c transmitter.h
	gcc -c transmitter.c 

receiver.o: receiver.c receiver.h
	gcc -c receiver.c

clean:
	rm *.o

clear:
	rm *.o numcom