#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <grp.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>


#define MAXSTR 2048 //tam max de cadena

int readcmdargs(char *input, char * cmdargs[]){
	//si inptu esta vacia devuelve 0
	if ((cmdargs[0]=strtok(input," \n\t)"))==NULL) 
		return 0;

	//graba en cmdargs el primer char de cada argumento
	int i=1;
	while ((cmdargs[i]=strtok(NULL," \n\t"))!=NULL) 
		i++;
	return i; //devuelve el num de argumentos contados
}

char *cwd(){

	static char buff[MAXSTR];
	return getcwd(buff,MAXSTR);
}

void pwd(){
	char *pwd=cwd();
	if (pwd != NULL) printf("%s", pwd);
	else perror(pwd);
}

void cd (char *cmdargs[MAXSTR]){
	if (cmdargs[0] != 0){
		if (chdir(cmdargs[0]) == -1) perror (cmdargs[0]);
	}
}



char *getGroup(gid_t g){
	struct group *grp;
	if ((grp = getgrgid(g)) == NULL) perror("getpwuid() error");
	else return grp->gr_name;
	
}

char *getUser(uid_t u){
	struct passwd *p;
	if ((p = getpwuid(u)) == NULL) perror("getpwuid() error");
	else return p->pw_name;
}

char fileType (mode_t m){
	switch (m & S_IFMT)
	{
		case S_IFSOCK: return 's'; // Socket
		case S_IFLNK: return 'l'; //
		case S_IFREG: return '-';
		case S_IFBLK: return 'b';
		case S_IFDIR: return 'd';
		case S_IFCHR: return 'c';
		case S_IFIFO: return 'p';
		default: return '?';
	}
}

char *fileModeToString (mode_t m){
	char *mode=(char *)malloc(11);
	strcpy(mode,"----------");
	mode[0]=fileType(m);
	if (m&S_IRUSR) mode[1]='r';
	if (m&S_IWUSR) mode[2]='w';
	if (m&S_IXUSR) mode[3]='x';
	if (m&S_IRGRP) mode[4]='r';
	if (m&S_IWGRP) mode[5]='w';
	if (m&S_IXGRP) mode[6]='x';
	if (m&S_IROTH) mode[7]='r';
	if (m&S_IWOTH) mode[8]='w';
	if (m&S_IXOTH) mode[9]='x';
	if (m&S_ISVTX) mode[10]='T';
	return (mode);
}

char *getFileMetaData (char *path,char *name){
	struct stat buf;
	char *file = (char *)malloc(sizeof(char)*MAXSTR);
	char *mode = (char *)malloc(sizeof(char)*MAXSTR);
	char *group = (char *)malloc(sizeof(char)*MAXSTR);
	char *user = (char *)malloc(sizeof(char)*MAXSTR);
	char *metadata = (char *)malloc(sizeof(char)*MAXSTR);
	sprintf (file,"%s/%s", path,name);
	if (stat (file,&buf) != -1)
	{
		mode = fileModeToString(buf.st_mode);
		group = getGroup(buf.st_gid);
		user = getUser(buf.st_uid);
		sprintf(metadata,"%s %2ld %10s %10s %7ld %s\n",
							mode, buf.st_nlink, group, user, buf.st_size, name);
		return metadata;
	}
	else perror("getFileMetaData() stat\n");
}

void ls(char *cmdargs[MAXSTR]){
	DIR *dir;
	struct dirent *element;
	char *path;
	int simple=0; int all=0; int i=0;
	//comprobamos cuales son los argumentos
	if(cmdargs[0] != 0)
	{
		for(i;cmdargs[i]!=NULL;i++){
			if (!strcmp(cmdargs[i],"-1")) simple = 1;
			else if (!strcmp(cmdargs[i],"-a")) all = 1;
			else path = cmdargs[i];
		}
		if(path == NULL) path = cwd();
	}
	else path = cwd();
	//
	if ((dir=opendir(path))==NULL) perror(path);
	else{
		while ((element = readdir(dir)) != NULL){
			if(!all && element->d_name[0]=='.')continue;
			else if(simple) printf("%s\n", element->d_name);
			else printf("%s", getFileMetaData(path,element->d_name));
		}
	}
	closedir(dir);
}

int cmdcall(char *cmdargs[]){

	if (!strcmp(cmdargs[0],"quit"))	
		exit(0);
	else if (!strcmp(cmdargs[0],"exit"))
		exit(0);
	else if (!strcmp(cmdargs[0],"pwd"))
		pwd();
	else if (!strcmp(cmdargs[0],"cd"))
		cd(cmdargs+1);
	else if (!strcmp(cmdargs[0],"ls"))
		ls(cmdargs+1); 

	return 0;
}




int main (int argc, char **argv)
{
	char *input = (char *)malloc(sizeof(char)*MAXSTR);
	char *cmdargs[MAXSTR];
	
	char buf[500], *token, *token1;
	
	FILE *f = fopen(argv[1],"r");

	if (f!=NULL){

		while(fgets(buf, sizeof(buf),(FILE*) f)){
			if(readcmdargs(buf,cmdargs)== 0) continue; //no hay nada
			cmdcall(cmdargs);	
		}
	}
	return(0);






	/*while(1){
		printf("\n#->");
		fgets(input,MAXSTR,stdin); //leer entrada
		//printf("La entradad es:%s\n", input ); //hacemos eco
		
		if(readcmdargs(input,cmdargs)== 0) continue; //no hay nada
		cmdcall(cmdargs);

	}*/
}