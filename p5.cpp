#define _CRTDBG_MAP_ALLOC
#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<crtdbg.h>

#include <stdio.h>
#include<windows.h>
#include<tchar.h>
#include<malloc.h>
#include <sys/types.h>
#include <string.h>
#include <wchar.h>
#include <strsafe.h>
#define ALL_WAY _T("C:\\Users\\ugaro\\program\\*")
#define ALL_TRUE_WAY _T("C:\\Users\\ugaro\\program\\")


struct pid {
	HANDLE handle;
	struct pid* next;
};
struct pid_list {
	struct pid* first;
};
struct pid_list* CreatePidList() {
	struct pid_list* list = (struct pid_list*)malloc(sizeof(struct pid_list));
	list->first = NULL;
	return list;
}void ClearPid(struct pid** first) {
	while (*first) {
		struct pid* point = *first;
		*first = (*first)->next;
		free(point);
	}
}

void DeletePidList(struct pid_list** list) {
	ClearPid(&((*list)->first));
	free(*list);
}
void PushFirstPid(struct pid_list** list, HANDLE handle) {
	(*list)->first = (struct pid*)malloc(sizeof(struct pid));
	(*list)->first->handle = handle;
	(*list)->first->next = NULL;
}
struct pid* FindLastPid(struct pid_list** list) {
	struct pid* point = (*list)->first;
	while (point->next) {
		point = point->next;
	}
	return point;
}
void PushPid(struct pid_list** list, HANDLE handle) {
	struct pid* last_pid = FindLastPid(list);
	last_pid->next = (struct pid*)malloc(sizeof(struct pid));
	last_pid = last_pid->next;
	last_pid->handle = handle;
	last_pid->next = NULL;
}void ErrorExit(LPCTSTR lpszFunction) {
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));//ВЫДЕЛЯЕТ ИЗ кучи памяти кусок локал памяти
	StringCchPrintf((LPTSTR)lpDisplayBuf, LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"), lpszFunction, dw, lpMsgBuf);//принимает форматирующею строку и возращает отфармотированную строку
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); // создаёт с мемедж окно с текстом
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

TCHAR* DeleteStar(TCHAR* str) {
	TCHAR* point = str;
	for (; *point != '*' && *point; point++);
	*point = '\0';
	return str;
}TCHAR* AddStar(TCHAR* str) {
	TCHAR* point = str;
	for (; *point; point++);
	*point = '*';
	*(++point) = '\0';
	return str;
}

DWORD WINAPI ren_rev(LPVOID lpParametr) {
	TCHAR* way = (TCHAR*)lpParametr;
	int res = 1;
	int res1 = 0;
	TCHAR help[255];
	TCHAR new_way1[256];

	TCHAR specialName1[255] = _T("C:\\Users\\user\\program\\");
	WIN32_FIND_DATA file_now;
	AddStar(way);
	HANDLE dir_now = FindFirstFile(way, &file_now);
	DeleteStar(way);
	if (dir_now != INVALID_HANDLE_VALUE) {//101
										  //110
										  //100

		struct pid_list* list = CreatePidList();

		while (dir_now && res) {
			TCHAR* p_char = file_now.cFileName;
			if (_tcscmp(file_now.cFileName, _T(".")) != 0) {
				if (_tcscmp(file_now.cFileName, _T("..")) != 0) {
					if (file_now.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						TCHAR new_way[255];
						_tcscpy(new_way, way);
						_tcscat(new_way, file_now.cFileName);
						_tcscat(new_way, _T("\\"));
						HANDLE pid = CreateThread(NULL, 0, ren_rev, new_way, 0, NULL);

						if (pid) {
							if (list->first == NULL) PushFirstPid(&list, pid);
							else PushPid(&list, pid);
						}
						else ErrorExit(_T("error:> Error of create Hundle"));
					}
					else {
						int i = 0, j = 0, count = 0, error;
						//for (; *p_char != '.' && *p_char; p_char++, i++) {
							//res++;
						//};
						if ((file_now.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
							printf("Simple File\n");
							for (int i = 0; file_now.cFileName[i] != '\0'; i++) {
								if (!(((file_now.cFileName[i] >= 'A') && (file_now.cFileName[i] <= 'Z'))
									|| ((file_now.cFileName[i] >= 'a') && (file_now.cFileName[i] <= 'z')))) {//101
									count++;																 //110
									res1 = 1;																//
								}
							}

							_tcscpy(help, way);
							_tcscat(help, file_now.cFileName);
							//_tcscat(specialName1, file_now.cFileName);
							_tprintf(_T("%s"), specialName1);
							if ((res1 == 1) && (count >= 2)) {
								if (DeleteFile(help) == 0) {
									_tprintf(_T("Delete %s\n"), way);
									ErrorExit(_T("error:>DeleteFail"));
									perror("Ошибка удаления");
									error = 1;
								}
							}
							TCHAR specialName1[255] = _T("C:\\Users\\user\\program\\");
						}
					}
				}
			}

			res = FindNextFile(dir_now, &file_now);

		}
		DWORD d_res = GetLastError();
		if (d_res != ERROR_NO_MORE_FILES) {
			ErrorExit(_T("error:> Error of found more files"));
		}
		res = FindClose(dir_now);
		if (!res) {
			ErrorExit(_T("error:> Error of close files"));
		}
		struct pid* point = list->first;
		DWORD result_of_wait;
		for (; point; point = point->next) {
			result_of_wait = WaitForSingleObject(point->handle, INFINITE);
			if (result_of_wait == WAIT_FAILED) {
				ErrorExit(_T("error:> Error of wait handle"));
			}
		}
		DeletePidList(&list);

	}
	else {
		ErrorExit(_T("error:> Error of found dir"));
	}
	return 0;
}


int main() {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	TCHAR way[256] = ALL_TRUE_WAY;
	TCHAR specialName1[255] = _T("C:\\Users\\user\\program\\");
	int res = 1;
	int res1 = 0;
	int error = 0;
	TCHAR new_way1[256];
	TCHAR help[255];
	WIN32_FIND_DATA file_now;
	AddStar(way);
	HANDLE dir_now = FindFirstFile(way, &file_now);
	DeleteStar(way); //
	if (dir_now != INVALID_HANDLE_VALUE) {
		struct pid_list* list = CreatePidList();
		while (dir_now && res) {
			TCHAR* p_char = file_now.cFileName;
			if (_tcscmp(file_now.cFileName, _T(".")) != 0) {
				if (_tcscmp(file_now.cFileName, _T("..")) != 0) {
					if (file_now.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
						TCHAR new_way[255];
						_tcscpy(new_way, way);
						_tcscat(new_way, file_now.cFileName);
						_tcscat(new_way, _T("\\"));
						HANDLE pid = CreateThread(NULL, 0, ren_rev, new_way, 0, NULL);
						if (pid) {
							if (list->first == NULL) PushFirstPid(&list, pid);
							else PushPid(&list, pid);
						}
						else ErrorExit(_T("error:> Error of create Hundle"));
					}
					else {
						int i = 0, j = 0, count = 0;
						//for (; *p_char != '.' && *p_char; p_char++, i++) {
							//res++;
						//};
						if ((file_now.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
							printf("Simple File\n");
							for (int i = 0; file_now.cFileName[i] != '\0'; i++) {
								if (!(((file_now.cFileName[i] >= 'A') && (file_now.cFileName[i] <= 'Z'))
									|| ((file_now.cFileName[i] >= 'a') && (file_now.cFileName[i] <= 'z')))) {
									count++;
									res1 = 1;
								}
							}
							_tcscpy(help, way);
							_tcscat(help, file_now.cFileName);
							//_tcscat(specialName1, file_now.cFileName);
							_tprintf(_T("%s"), specialName1);
							if ((res1 == 1) && (count >= 2)) {
								if (DeleteFile(help) == 0) {
									ErrorExit(_T("error:>DeleteFail"));
									perror("Ошибка удаления");
									error = 1;
								}
							}
							TCHAR specialName1[255] = _T("C:\\Users\\user\\program\\");
						}


					}


				}
			}
			res = FindNextFile(dir_now, &file_now);
		}

		DWORD d_res = GetLastError();
		if (d_res != ERROR_NO_MORE_FILES) {
			ErrorExit(_T("error:> Error of found more files"));
		}
		res = FindClose(dir_now);
		if (!res) {
			ErrorExit(_T("error:> Error of close files"));
		}
		struct pid* point = list->first;
		DWORD result_of_wait;
		for (; point; point = point->next) {
			result_of_wait = WaitForSingleObject(point->handle, INFINITE);
			if (result_of_wait == WAIT_FAILED) {
				ErrorExit(_T("error:> Error of wait handle"));
			}
			else {
				CloseHandle(point->handle);
			}
		}
		DeletePidList(&list);


	}
	else {
		ErrorExit(_T("error:> Error of found dir"));
	}
	_CrtDumpMemoryLeaks();
	return 0;
}




























































































//TCHAR* new_way = (TCHAR*)malloc(sizeof(new_way1));
