// P2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <tchar.h>
#include <stdbool.h>
#include <stdio.h>
#include <strsafe.h>

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

int main()
{
	WIN32_FIND_DATA findData;
	DWORD Error = 0;
	DWORD error = 0;
	bool end_flag = true;
	int count = 0;
	int maxCountOfcFileName = 0;
	int maxCountOfnameC = 0;
	int maxCountOfspecialName = 0;
	int maxCountOfName = 0;
	int res;
	TCHAR name[255];
	TCHAR cFileName[255] = _T("C:\\Users\\user\\program\\*");
	HANDLE hFind = FindFirstFile(_T("C:\\Users\\user\\program\\*"), &findData);
	TCHAR specialName1[255] = _T("C:\\Users\\user\\program\\");
	printf("Open File\n");
	if (INVALID_HANDLE_VALUE == hFind) {
		ErrorExit(_T("error:> FindFirstFile"));
		error = -1;
		end_flag = false;
	}
	if (end_flag) {
		printf("End Flag\n");
		do {
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
				_tcscat(specialName1, findData.cFileName);
				_tprintf(_T("%s"), specialName1);
				if ((res == 1) && (count >= 2)) {
					if (DeleteFile(specialName1) == 0) {
						ErrorExit(_T("error:>DeleteFail"));
						perror("Ошибка удаления");
						error = 1;
					}
				}
				TCHAR specialName1[255] = _T("C:\\Users\\user\\program\\");
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



// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
