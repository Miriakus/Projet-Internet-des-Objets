objet : main.o capteur.o serveur.o
	gcc -o objet main.o capteur.o serveur.o -lpthread

serveur.o : serveur.c serveur.h
	gcc -c serveur.c

capteur.o : capteur.c capteur.h
	gcc -c capteur.c

main.o : main.c capteur.h
	gcc -c main.c

clean :
	rm -f objet *.o



