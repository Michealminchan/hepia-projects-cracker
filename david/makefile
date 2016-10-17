FLAGS=-g -Wall -Wextra -std=gnu99 -D__USE_GNU -D_GNU_SOURCE
LIBS=-lpthread -lrt -lcrypt

all: main main_encrypt main_stats

main: main.c password_cracker.o password.o time_measure.o file.o threads.o
	gcc $(FLAGS) -o main main.c *.o $(LIBS)

main_encrypt: main_encrypt.c password.o
	gcc $(FLAGS) -o main_encrypt main_encrypt.c password.o $(LIBS)

main_stats: main_stats.c password_cracker.o password.o time_measure.o file.o threads.o
	gcc $(FLAGS) -o main_stats main_stats.c *.o $(LIBS)



password_cracker.o: password_cracker.c password_cracker.h
	gcc $(FLAGS) -c password_cracker.c

password.o: password.c password.h
	gcc $(FLAGS) -c password.c

time_measure.o: time_measure.c time_measure.h
	gcc $(FLAGS) -c time_measure.c

file.o: file.c file.h
	gcc $(FLAGS) -c file.c

threads.o: threads.c threads.h
	gcc $(FLAGS) -c threads.c



clean:
	rm main main_encrypt main_stats *.o

crack: main
	./main 431pugYzZc5QM 43 4
