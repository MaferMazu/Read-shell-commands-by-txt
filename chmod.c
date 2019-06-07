#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

void main(){
	int i,j;
	struct stat sbuf;
	mode_t mode;
	char perms[10];
	char *line="basic.sh"; 
	char *name;
	stat(name,&sbuf);
	
	printf("File Permissions: \t");
    printf( (S_ISDIR(sbuf.st_mode)) ? "d" : "-");
    printf( (sbuf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (sbuf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (sbuf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (sbuf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (sbuf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (sbuf.st_mode & S_IXGRP) ? "x" : "-");
    printf( (sbuf.st_mode & S_IROTH) ? "r" : "-");
    printf( (sbuf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (sbuf.st_mode & S_IXOTH) ? "x" : "-");
    printf("\n\n");
    printf("%o\n", sbuf.st_mode);
    mode = sbuf.st_mode & 0;
    printf("%o\n", sbuf.st_mode);

    // modify permissions to rwxr-xr-x
    mode |= S_IRUSR;       /* Set owner read bit */
    mode |= S_IWUSR;       /* Set owner write bit */
    mode |= S_IXUSR;       /* Set owner execute bit */
    mode |= S_IRGRP;       /* Set group read bit */
    mode &= ~(S_IWGRP);
    mode |= S_IXGRP;       /* Set group execute bit */
    mode |= S_IROTH;       /* Set other read */
    mode &= ~(S_IWOTH);
    mode |= S_IXOTH;       /* Set other execute */

    chmod(name, mode);
    printf("%o\n", sbuf.st_mode);
    
    printf("\n\n");

}