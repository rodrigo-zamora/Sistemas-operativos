all: init getty shell
	
init: init.c
	gcc -o init.out init.c
	
getty: getty.c
	gcc -o getty.out getty.c
	
shell: shell.c
	gcc -o shell.out shell.c
	
clean: 
	rm init.out getty.out sh.out