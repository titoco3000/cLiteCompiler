#ifndef FILE_HELPER_H
#define FILE_HELPER_H

#ifdef _WIN32
#include <io.h>
#define ACCESS_FUNC(a, b) _access(a, b)
#define FOLDER_SEPARATOR "\\"
#else
#include <unistd.h>
#define ACCESS_FUNC(a, b) access(a, b)
#define FOLDER_SEPARATOR "/"
#endif

#include <sys/types.h> // Para stat().
#include <sys/stat.h>  // Para stat().

#include <dirent.h>
#include <stdio.h>
#include <pthread.h>

#ifndef S_IFDIR
    // Isso não é realmente necessario, e nunca vai ser executado;
    // é só para parar erros irritantes no intellisense
    #define S_IFDIR 16384
#endif

int is_directory(char *addr)
{
    if (ACCESS_FUNC(addr, 0) == 0)
    {

        struct stat status;
        stat(addr, &status);

        return (status.st_mode & S_IFDIR) != 0;
    }
    return 0;
}


typedef struct thread_struct
{
    void (*func)(char *);
    char *buffer;
}thread_struct;

void *thread_func(void *arg){
    thread_struct *th = (thread_struct*)arg;
    (*th->func)(th->buffer);
}
void applyFunctionToAllFilesRecursive(const char *path, void (*f)(char *))
{
    struct dirent *dp;
    DIR *dir = opendir(path);

    // Se não conseguir abrir directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        // Se não é "." (pasta atual) ou ".." (pasta acima)
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char buffer[512];
            strcpy(buffer, path);
            strcat(buffer, FOLDER_SEPARATOR);
            strcat(buffer, dp->d_name);
            if (is_directory(buffer))
                applyFunctionToAllFilesRecursive(buffer, f);
            else{
                pthread_t thread;
                thread_struct th;
                th.func = f;
                th.buffer = buffer;
                //Cria thread separada para rodar função, para se der erro não 
                //encerrar o programa. Passa para a função o endereço do arquivo.
                pthread_create(&thread, NULL, thread_func, (void*)&th);
                pthread_join(thread, NULL);
            }
        }
    }

    // Fecha directory stream
    closedir(dir);
}

#endif