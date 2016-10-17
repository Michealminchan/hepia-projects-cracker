main: main.o cracker.o pwdGenerator.o
	gcc -o main main.o cracker.o pwdGenerator.o -lpthread -lcrypt -lm

main.o: main.c cracker.h
	gcc -std=gnu99 -Wall -Wextra -pg -c main.c

cracker.o: cracker.c cracker.h pwdGenerator.h
	gcc -std=gnu99 -Wall -Wextra -pg -c cracker.c 

pwdGenerator.o: pwdGenerator.c pwdGenerator.h
	gcc -std=gnu99 -Wall -Wextra -pg -c pwdGenerator.c 

clean:
	rm -f *.o main	