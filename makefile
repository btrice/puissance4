essai.exe: essai.o interface.o partie.o case.o dim.o
	   gcc essai.o interface.o partie.o case.o dim.o `pkg-config --cflags --libs gtk+-2.0` -o essai.exe -Wall 
essai.o: essai.c  controleur.h  
	   gcc -c essai.c `pkg-config --cflags --libs gtk+-2.0` -o essai.o -Wall
interface.o: interface.c controleur.h partie.h
	   gcc -c interface.c `pkg-config --cflags --libs gtk+-2.0` -o interface.o -Wall
case.h: 
case.c: case.h
case.o: case.c
	gcc -c case.c -o case.o -Wall --pedantic 

dim.h: 
dim.c: dim.h
dim.o: dim.c
	gcc -c dim.c -o dim.o -Wall --pedantic 

partie.h: case.h dim.h
partie.c: partie.h case.h dim.h
partie.o: partie.c
	gcc -c partie.c -o partie.o -Wall --pedantic 
# nettoyage
clean: 
	rm *.o




