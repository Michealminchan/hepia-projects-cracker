FLAGS=-g -Wall -Wextra -std=gnu99 -D_GNU_SOURCE
LIBS=-lpthread -lrt -lcrypt -lm

main: cracker.o pwd_generator.o time_measure.o
	gcc $(FLAGS) -o main main.c *.o $(LIBS)

cracker.o: cracker.c cracker.h pwd_generator.h
	gcc $(FLAGS) -c cracker.c 

pwd_generator.o: pwd_generator.c pwd_generator.h
	gcc $(FLAGS) -c pwd_generator.c 

time_measure.o: time_measure.c time_measure.h
	gcc $(FLAGS) -c time_measure.c


clean:
	rm main *.o

crack: main
	./main 431pugYzZc5QM 43 16

crack_test: main
	./main 43oEb0lpjXzQc 43 16
