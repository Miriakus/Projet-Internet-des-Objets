objet : objet.o capteur.o
	cc -o objet objet.o capteur.o

capteur.o : capteur.c capteur.h
	cc -c capteur.c

objet.o : objet.c capteur.h
	cc -c objet.c
#	cc -c -DDEBUG -DDEBUG1 -DDEBUG2 fork4.c

clean :
	rm -f objet *.o



