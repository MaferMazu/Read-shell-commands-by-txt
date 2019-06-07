#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char **argv)
{
	char buf[500], *token, *token1;
	
	const char *pipe="|";
	const char *input="<";
	const char *output=">";
	
	FILE *f = fopen(argv[1],"r");

	if (f!=NULL){

		while(fgets(buf, sizeof(buf),(FILE*) f)){
			int nr=0;
			token = strtok(buf,pipe);
			while(token!=NULL){
				nr++;
				printf("%d\n", nr);
				printf( " %s\n", token );
				/*token = strtok(NULL, pipe);*/
			}
			if(nr==1){
				printf( " %s\n", token );
			}
		}
	}
	return(0);
}