#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#define MAX_PATH 260

int par(char *PATH, char *str) {
	int share = 0;
	char str1[255];
	int j = 0;
	DIR *catalog;
	if (!(catalog = opendir(PATH)))
		fprintf(stderr, "Error: opening to path %s: %s\n", PATH, strerror(errno));
	else {
		struct dirent *file;
		unsigned int i = 1;
		errno = 0;
		char temp_name[255];
		while (((file = readdir(catalog)) != NULL) && (errno == 0)) {
			char *p;
			if (file -> d_type == DT_REG) {
				for (j=0; j < strlen(file -> d_name); j++) {
					if (file->d_name[j] == '.') {
						share = j;
					}
				}
				for (int n = j; n < strlen(file -> d_name); n++) {
					strcat(str1,file->d_name[n]);
				}
				if (strlen(str1)> strlen(file -> d_name)-share){
					strcpy(str,file->d_name);
					i++;
				}
				else if ((file->d_type==DT_DIR) && ((strcmp(file ->d_name, ".")) != 0) && ((strcmp(file->d_name,".."))!=0)){
					char nextPath[MAX_PATH];
					strcpy(nextPath, PATH);
					strcat(nextPath,file->d_name);
					strcat(nextPath,"/");
					par(nextPath, str);
				}
			}
			if ((file == NULL) && (errno != 0))
				perror("Error read catalog");
			closedir(catalog);
		}
	}
	return 0;
}
int main() {
	char maxlenghtname[260]="\0";
	par("/home/slava/YP/pr8/", maxlenghtname);
	printf("%s", maxlenghtname);
	return 0;
}
