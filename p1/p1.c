#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<dirent.h>
#include<errno.h>

int main() {
}	int res = 0, res1 =0;//Переменная результата
char str[256];//Перемееная для чтения имени файла
errno = 0;//Переменная ошибки
int sum_simbol = 0;
int i;

DIR *p = opendir(".");
if (p == 0) {
	perror("Ошибка каталога");
	res = -1;
}else{
	struct dirent *item;
	while (item = readdir (p)){
		int symbol = 0;

		if (item -> d_type == DT_REG) {
			sum_simbol = strlen(item -> d_name);
			for (i = 0; i < sum_simbol; ++i) {
				if(!(((item -> d_name[i] >= 'A') && (item -> d_name[i] <= 'Z'))
							|| ((item -> d_name[i] >= 'a') && (item -> d_name[i] <= 'z')))){
					symbol++;
					res1 = 1;
				}
			}
			if ((res1== 1) && (symbol>=2)) {
				if (unlink(item -> d_name) == -1) {
					perror ("Error unlink file");
					res = -1;
				}
			}	

		}
		errno = 0;
	}
	if (errno != 0) {
		perror("Проблема открытия каталога");
		res = -1;
	}
}
closedir(p);
return res;

