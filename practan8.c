#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<stdbool.h>
#include<errno.h>
#include <sys/wait.h>
typedef struct result{
	char name[256];
}result;

typedef struct list {
	pid_t id;
	struct list * next;
}list;

int add(list * first, pid_t pid) {
	int res = 0;
	while (first->next != 0) first = first->next;
	first->next = (list*) malloc(sizeof(list));
	if (first->next != 0) {
		first->next->id = pid;
		first->next->next = 0;
	} else res = -1;
	return res;
}

int del(list * first) {
	list * save = first;
	for(;first->next != 0; save = first, first = first->next);
	free(save->next);
	save->next = 0;
	return save->next == 0 ? 0 : 1;
}

int clear_list(list * first) {
	int res = 0;
	for (;first->next != 0;) res = del(first);
	first->id = 0;
	return res;
}

pid_t last_pid(list * first) {
	while (first->next != 0) first = first->next;
	return first->id;
}




result max_expansion_dir(char dir[256], pid_t * pid){
	int pipefd[2];
	pipefd[0] = 0;
	pipefd[1] = 0;
	int i_res = 0, i_res2 = 0;
	int count_expansion = 0, max = 0;
	bool fl = false;
	char name[256];
	strcpy(name, dir);
	list * first = (list*)malloc(sizeof(list));
	result res, res2, final_res;
	memset(final_res.name, 0, 256);
	memset(res.name, 0, 256);
	DIR * direct = NULL;
	if(pipe(pipefd) == -1) 
		perror("error of creating pipe");
	else{
		if(first!=0){
			first->id = -2;
			first->next = 0;
			if ((direct = opendir(dir)) != NULL){
				struct dirent *item = NULL;
				do{
					errno = 0;

					if ((item = readdir(direct)) != NULL) {
						strcpy(name, item->d_name);
						//printf("%s", name);

						if (item->d_type == DT_REG) {

							for(int i = strlen(name); (i>=0); i--){
								if(name[i] == '.')
									fl = true;
							}
							for(int i = strlen(name); (i>=0)&&(name[i] != '.')&&(fl == true); i--){
								count_expansion++;
							}
							if(max<count_expansion){
								max = count_expansion;
								memset(res.name, 0, 256);
								strcpy(res.name, name);
								strcat(res.name, "\n");
							}   
							count_expansion = 0;
						}


						else if (item->d_type == DT_DIR && strcmp(item->d_name, ".") != 0 && strcmp(item->d_name, "..") != 0) {

							*pid = fork();
							if (add(first, *pid) == -1) 
								perror("error while add");
							if (*pid == 0) { //child process
								strcpy(name, dir);

								strcat(name, item->d_name);
								strcat(name, "/");

								if (closedir(direct) != 0) 
									perror("Error of closing directory");
								if ((direct = opendir(name)) == NULL) {
									perror("error of opening new dir");
								}


								clear_list(first);
							} 
							else if (*pid == -1)
								perror("error of create process");

						}
					}
					else if (item == NULL && errno != 0)
						perror("error while read dir");
				}while(item!=NULL);
			}else {
				perror("error of openning directory");
			}

			if (*pid > 0) {
				int status;

				for (; last_pid(first) != 0 && last_pid(first) != -2;) {
					pid_t result_of_waitpid = waitpid(last_pid(first), &status, 0);
					del(first);
					if (!WIFEXITED(status)) {
						if (result_of_waitpid == -1) perror("Error of waitpid");
						else {
							if (!WIFEXITED(status)) {
								perror("Child process hasn't finished properly\n");
							} else {
								printf("Child process has returned %d\n", WEXITSTATUS(status));
							}
						}   
					}
				}
			}
			if (*pid == 0){
				if (close(pipefd[0]) == -1){
					perror("error of closing pipe");
				}
				ssize_t reswrite = write(pipefd[1], &res, sizeof(res));
				if (reswrite!=sizeof(res)){
					perror("error of writing pipe");
				}
				if (close(pipefd[1]) == -1){
					perror("error of closing pipe");
				}
			} else if (first->id == -2) {
				if (close(pipefd[1]) == -1){
					perror("error of closing pipe");
				}
				ssize_t resread=1;
				for(resread=read(pipefd[0], &res2, sizeof(res2));resread > 0;resread=read(pipefd[0], &res2, sizeof(res2))){//read - cчитывает данные файлового описателя 
					for(int i=strlen(res.name); (i>=0) &&(res.name[i]!='.'); i--, i_res++);
					for(int i=strlen(res2.name); (i>=0) &&(res2.name[i]!='.'); i--, i_res2++);
					if(i_res2>i_res) {
						strcpy(final_res.name, res2.name);
					}
					else strcpy(final_res.name, res.name);
				}
				if (close(pipefd[0])==-1)
					perror("error of closing pipe");
				if (resread<0){
					perror("error of read pipe");
				}
				*pid=-2;
			}
		} else perror("error of malloc");
		if (closedir(direct) != 0) perror("error close dir");
		free(first); 
	}
	return final_res;
}
int main(){
	printf("1");
	char way[256] = "./";
	printf("2");
	result a1;
	printf("3");
	pid_t pid = 0;
	printf("4");
	result final = max_expansion_dir(way, &pid);
	printf("5");
	printf("%s", final.name);
}
