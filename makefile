build: 
	gcc -pthread multiplication.c -o multiplication
	gcc -pthread 7353-sort.c -o 7353-sort
clean:
	rm -f multiplication 7353-sort
