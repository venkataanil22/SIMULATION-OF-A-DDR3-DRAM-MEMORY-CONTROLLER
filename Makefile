DRAM_Controller: main.o
	gcc main.o -o DRAM_Controller


main.o:	main.c 
	gcc -c main.c 

clean:
	rm -f *.o *~ core 

