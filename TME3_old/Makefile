OBJ=main.o transmitter.o receiver.o gaussian.o

numcom:	$(OBJ)
	gcc -o numcom $(OBJ) -lm

main.o: main.c transmitter.h receiver.h
	gcc -c main.c

transmitter.o: transmitter.c transmitter.h
	gcc -c transmitter.c

receiver.o: receiver.c receiver.h gaussian.h
	gcc -c receiver.c

gaussian.o: gaussian.c gaussian.h
	gcc -c gaussian.c -lm

clean:
	rm *.o

clear:
	rm *.o numcom