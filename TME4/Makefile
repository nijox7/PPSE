SRC=src/
OBJ=obj/
FILES=$(OBJ)main.o $(OBJ)transmitter.o $(OBJ)receiver.o $(OBJ)gaussian.o

simulator:	$(FILES)
	gcc -o simulator $(FILES) -lm

$(OBJ)main.o: $(SRC)main.c $(SRC)transmitter.h $(SRC)receiver.h
	gcc -c $(SRC)main.c -o $(OBJ)main.o

$(OBJ)transmitter.o: $(SRC)transmitter.c $(SRC)transmitter.h
	gcc -c $(SRC)transmitter.c -o $(OBJ)transmitter.o

$(OBJ)receiver.o: $(SRC)receiver.c $(SRC)receiver.h $(SRC)gaussian.h
	gcc -c $(SRC)receiver.c -o $(OBJ)receiver.o

$(OBJ)gaussian.o: $(SRC)gaussian.c $(SRC)gaussian.h
	gcc -c $(SRC)gaussian.c -lm -o $(OBJ)gaussian.o

clean:
	rm $(OBJ)*.o

clear:
	rm $(OBJ)*.o simulator