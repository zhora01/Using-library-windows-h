#include <windows.h>
#include <locale.h>
#include <tchar.h>
#include <cstdio>
#include <string>

typedef std::basic_string<TCHAR> tstring;

HANDLE pMutex = CreateMutex(NULL, FALSE, NULL); //Мьютекс
HANDLE h;                                       //Указатель на файлы/папку
WIN32_FIND_DATA wfd;                            //информация о файлах

DWORD WINAPI findFile(LPVOID lpParam)   //Процесс поиска файлов
{
    WaitForSingleObject(pMutex, INFINITE);  //Мютекс для синхронизации потоков

    ReleaseMutex(pMutex);

    return 0;
}

int main(int argc, _TCHAR* argv[])
{
    setlocale(LC_ALL, "RUSSIAN");
    HANDLE pThread[MAX_PATH];                   //Потоки

    tstring dirpath(_T("c:/Test")), mask(_T("*.*")), path(dirpath + _T('/') + mask);         //Поиск в каталоге Test файлов по маске *.*
    int nd = 0, nf = 0;
    tstring folderNamef[50];    //Массив строк, хранящий названия папок
    tstring folderNamed[50];

    h = FindFirstFile(path.c_str(), &wfd);              //Находим первый файл/каталог

    if (h != INVALID_HANDLE_VALUE)            //Если нет ошибки
    {
        do  //Читаем каталоги
        {
            if (wfd.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)    //Если это каталог
            {
                if (strcmp((char*)wfd.cFileName, "..") && strcmp((char*)wfd.cFileName, "."))   //Пропускаем .. и .
                {
                    folderNamed[nd] = dirpath + _T('/') + wfd.cFileName;
                    _tprintf(_T("Найденые каталоги: %s\n"), folderNamed[nd].c_str());
                    nd++;    //Счетчик каталогов
                }
            }
            else
            {
                folderNamef[nf] = dirpath + _T('/') + wfd.cFileName;
                _tprintf(_T("Найденный файл: %s\n"), folderNamef[nf].c_str());
                ++nf;
            }
        }         while (FindNextFile(h, &wfd));

        FindClose(h);   //Прекращаем поиск

        printf("Количество каталогов: %d\n", nd);

        for (int i = 1; i <= nd; i++)    //Создаём потоки
            pThread[i] = CreateThread(NULL, 0, findFile,
                const_cast<TCHAR*>(folderNamed[i].c_str()), 0, 0);  //Функции передаем название каталогов

        for (int i = 1; i <= nd; i++)    //Принимаем потоки
        {
            WaitForSingleObject(pThread[i], INFINITE);
            CloseHandle(pThread[i]);
        }
    }

    return 0;
}