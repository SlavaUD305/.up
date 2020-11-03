#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>
#define ALL_WAY "/home/slava/.up/p3/"
int func(char *way){	
	int res = 0, res1 =0;//Переменная результата
	char str[256];//Перемееная для чтения имени файла
	errno = 0;//Переменная ошибки
	int sum_simbol = 0;
	int i;
	int symbol = 0;

	DIR *dir_now = opendir(way);
	if (!(dir_now)) {
		perror("Ошибка каталога");
		res = -1;
	}else{
		struct dirent *item;
		while ((item = readdir (dir_now)) != NULL){
			char *p_char = item -> d_name;
			int symbol = 0;
			if (*p_char == '.') {
				if ((strcmp((*item).d_name,".")!=0)||((strcpm((*item).d_name, "..")!=0))) {

					if (item -> d_type == DT_DIR) {
						char new_way[256];
						strcpy(new_way, way);
						strcat(new_way, item -> d_name);
						strcat(new_way, "//");
						func(new_way);
					}else{
						sum_simbol = strlen(item -> d_name);
						for (i = 0; i < sum_simbol; ++i) {
							if(!(((item -> d_name[i] >= 'A') && (item -> d_name[i] <= 'Z'))
										|| ((item -> d_name[i] >= 'a') && (item -> d_name[i] <= 'z')))){
								symbol++;
								//res1 = 1;
							}
						}
					}
				}


				if  (symbol>=2) {
					char path[255];
					strcpy(path, way);
					strcat(path, item -> d_name); 
					if (unlink(path) == -1) {
						perror ("Error unlink file");
						res = -1;
					}
				}
			}	


			closedir(dir_now);	
		}
		errno = 0;

		if (errno != 0) {
			perror("Проблема открытия каталога");
			res = -1;
		}
		return res;

	}
	int main() {
		func(ALL_WAY);
		return 0;
	}
