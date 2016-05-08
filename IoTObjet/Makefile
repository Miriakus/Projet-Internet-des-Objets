objet : objet.o capteur.o
	gcc -o objet objet.o capteur.o -lpthread

capteur.o : capteur.c capteur.h
	gcc -c capteur.c

objet.o : objet.c capteur.h
	gcc -c objet.c

clean :
	rm -f objet *.o



