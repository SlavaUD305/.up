// p3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<tchar.h>
#include<sys/types.h>
#include<string.h>
#include<strsafe.h>


void ErrorExit(LPCTSTR lpszFunction) {
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
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
}

int fileOfWindows(TCHAR buffer[]) {
	WIN32_FIND_DATA findData;
	DWORD Error = 0;
	DWORD error = 0;
	bool end_flag = true;
	//int count = 0;
	int res = 0;
	int maxCountOfcFileName = 0;
	int maxCountOfnameC = 0;
	int maxCountOfspecialName = 0;
	int maxCountOfName = 0;
	int p = 0;
	TCHAR name[255];
	TCHAR help[255];
	TCHAR help1[255];
	TCHAR name1[255];
	TCHAR cFileName[255] = _T("C:\\Users\\ugaro\\program\\*");
	TCHAR specialName1[255] = _T("C:\\Users\\ugaro\\program\\");
	_tcscat(buffer, _T("\\*"));
	HANDLE hFind = FindFirstFile(buffer, &findData);
	_tprintf(_T("%s\n"), buffer);
	
	printf("Open File\n");
	if (INVALID_HANDLE_VALUE == hFind) {
		ErrorExit(_T("error:> FindFirstFile"));
		error = -1;
		end_flag = false;
	} else
	if (end_flag) {
		printf("End Flag\n");
		do {
			int count = 0;
			TCHAR specialName1[255] = _T("C:\\Users\\ugaro\\program\\");
			_tprintf(findData.cFileName);
			printf("\n");
			if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				printf("Simple File\n");
				for (int i = 0; findData.cFileName[i] != '\0'; i++) {
					if (!(((findData.cFileName[i] >= 'A') && (findData.cFileName[i] <= 'Z'))
						|| ((findData.cFileName[i] >= 'a') && (findData.cFileName[i] <= 'z')))) {
						count++;
						res = 1;
					}
				}
				_tprintf(_T("Path buffer: %s\n"), buffer);
				int maxCountname1 = _tcslen(buffer);
				printf("%d\n", maxCountname1);
				for (int i = 0; i < maxCountname1 - 1; i++) {
					help[i] = buffer[i];
				}
				help[maxCountname1 - 1] = _T('\0');
				_tprintf(_T("Path help: %s\n"), help);
				_tcscpy(help1, help);
				_tcscat(help1, findData.cFileName); // 
				_tprintf(_T("Path help1: %s\n"), help1);
				_tcscat(help, name); // 
				_tprintf(_T("Path help: %s\n"), help);
				if ((res == 1) && (count >= 2)) {
					if (DeleteFile(help1) == 0) {
						ErrorExit(_T("error:>DeleteFail"));
						perror("Ошибка удаления");
						error = 1;
					}
				}
				TCHAR specialName1[255] = _T("C:\\Users\\ugaro\\program\\");
			}			else if ((findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) {
				if (_tcscmp(findData.cFileName, _T(".")) != 0) {
					if (_tcscmp(findData.cFileName, _T("..")) != 0) {
						printf("Open directory\n");
						_tprintf(_T("Path buffer: %s\n"), buffer);
						_tcscpy(name1, buffer);
						int maxCountname1 = _tcslen(name1);
						printf("%d\n", maxCountname1);
						for (int i = 0; i < maxCountname1 - 2; i++) {
							help[i] = name1[i];
						}
						help[maxCountname1 - 2] = _T('\0');
						_tprintf(_T("Path help: %s\n"), help);
						_tcscat(help, _T("\\"));
						_tcscat(help, findData.cFileName);
						_tprintf(_T("Path help: %s\n"), help);
						fileOfWindows(help);
					}
				}
			}
		} while (FindNextFile(hFind, &findData) != NULL);
		Error = GetLastError();
		if (Error != ERROR_NO_MORE_FILES) {
			ErrorExit(_T("error:>FindNextFile"));
			error = -1;
		}
	}
	FindClose(hFind);
	return error;
}
int main() {
	int error = 0;
	TCHAR buffer[255] = _T("C:\\Users\\ugaro\\program\\test");
	error = fileOfWindows(buffer);
	return error;
}


// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
