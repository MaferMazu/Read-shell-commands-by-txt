#include <stdio.h>

parse (buf,args)
char *buf;
char **args;
{
	while (*buf != NULL){
		/* frajas en blanco
		* usa nulo para que el argumento anterior se termine
		*/	

		while ((*buf == ''))  ||  ((*buf == '\t' ))	
			*buf++ = NULL;

		/* salva los argumentos */
		*args++ = buf;

		/* omite los argumentos */ 
		while ((*buf != NULL) && (*buf != '' ) && (*buf != '\t'))
			buf++;
	}

	*args = NULL;
}

/* execute
genera un proceso hijo y ejecuta los programas
*/

execute(args)
char **args;
{
	int pid, status;

	/* 
	obtiene el proceso hijo
	*/

	if ((pid = fork()) < 0){
		perror(”fork”);
		exit(1);
	}

	/* el hijo ejecuta el codigo dentro del if
	*/

	if (pid == 0){
		execvp(*args,args);
		perror(*args);
		exit(1);
	}

	/*
	el padre ejecuta la espera
	*/ 
	while (wait(&status) != pid)
		/* vacio*/;
}


main()
{
  char buf[1024];
  char *args[64];

  for (;;) {
  /*Lectura de comandos*/
  printf(”>”);
  if (gets(buf) == NULL){
    printf(” /n”);
    exit(0);
  }
  /* Pico el string en argumentos */
  parse(buf,args);

  /*Ejecuto el comando*/
  execute(args);
  }
}

