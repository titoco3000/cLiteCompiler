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

#include <sys/types.h> // For stat().
#include <sys/stat.h>  // For stat().

#include <dirent.h>
#include <stdio.h>

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
void applyFunctionToAllFilesRecursive(const char *path, void (*f)(char *))
{
    struct dirent *dp;
    DIR *dir = opendir(path);

    // Unable to open directory stream
    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            char buffer[512];
            strcpy(buffer, path);
            strcat(buffer, FOLDER_SEPARATOR);
            strcat(buffer, dp->d_name);
            if (
#ifdef _WIN32
                is_directory(buffer)
#else
                dp->d_type == 4
#endif
            )
            {
                applyFunctionToAllFilesRecursive(buffer, f);
            }
            else
                (*f)(buffer);
        }
    }

    // Close directory stream
    closedir(dir);
}

#endif