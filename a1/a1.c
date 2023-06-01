#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>

void simpleList(const char *path, int size, char *nameStarts)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    struct stat status;
    char fullPath[512];
    dir = opendir(path);
    if (dir == NULL)
    {
        perror("ERROR\n");
        return;
    }
    else
    {
        printf("SUCCESS\n");
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, "..") == 0 || strcmp(entry->d_name, ".") == 0)
        {
            continue;
        }
        snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
        if (lstat(fullPath, &status) == -1)
        {
            continue;
        }
        if (size > 0)
        {
            if (status.st_size > size)
                printf("%s\n", fullPath);
        }
        else if (!strcmp(nameStarts, "null") == 0)
        {
            if (strncmp(entry->d_name, nameStarts, strlen(nameStarts)) == 0)
            {
                printf("%s\n", fullPath);
            }
        }
        else
        {
            printf("%s\n", fullPath);
        }
    }
    closedir(dir);
}

void recursiveListing(const char *path, int success, int size, char *nameStarts)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512];
    struct stat status;
    dir = opendir(path);
    if (dir == NULL)
    {
        if (success == 1)
        {
            perror("ERROR\n");
        }
        return;
    }
    else if (success == 1)
    {
        printf("SUCCESS\n");
        success = 0;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &status) == -1)
            {
                continue;
            }
            if (size > 0)
            {
                if (status.st_size > size)
                {
                    if (S_ISDIR(status.st_mode) == 0)
                    {
                        printf("%s\n", fullPath);
                    }
                }
            }
            else if (!strcmp(nameStarts, "null") == 0)
            {
                if (strncmp(entry->d_name, nameStarts, strlen(nameStarts)) == 0)
                {
                    printf("%s\n", fullPath);
                }
            }
            else
            {
                printf("%s\n", fullPath);
            }
            if (S_ISDIR(status.st_mode))
            {
                recursiveListing(fullPath, 0, size, nameStarts);
            }
        }
    }
    closedir(dir);
}

void parse(char *path)
{
    int fd1 = -1;
    fd1 = open(path, O_RDONLY);
    if (fd1 == -1)
    {
        return;
    }
    lseek(fd1, 0, SEEK_END);
    unsigned char magic[3] = "";
    lseek(fd1, -2, SEEK_CUR);
    read(fd1, magic, 2);
    if (strcmp((char *)magic, "bm") != 0)
    {
        printf("ERROR\nwrong magic");
        return;
    }
    short int hSize = 0;
    lseek(fd1, -4, SEEK_CUR);
    read(fd1, &hSize, 2);
    lseek(fd1, -hSize, SEEK_END);
    short int version = 0;
    read(fd1, &version, 2);
    if (version < 63 || version > 169)
    {
        printf("ERROR\nwrong version");
        return;
    }
    unsigned char nrSec = ' ';
    read(fd1, &nrSec, 1);
    if (nrSec < 8 || nrSec > 11)
    {
        printf("ERROR\nwrong sect_nr");
        return;
    }
    int checkPoint = lseek(fd1, 0, SEEK_CUR);
    for (int i = 0; i < nrSec; i++)
    {
        lseek(fd1, 9, SEEK_CUR);
        short int sectType = 0;
        read(fd1, &sectType, 2);
        if (sectType != 11 && sectType != 77)
        {
            printf("ERROR\nwrong sect_types");
            return;
        }
        lseek(fd1, 8, SEEK_CUR);
    }
    printf("SUCCESS\nversion=%d\nnr_sections=%d\n", version, nrSec);
    lseek(fd1, checkPoint, SEEK_SET);
    for (int i = 0; i < nrSec; i++)
    {
        unsigned char sectName[10] = "";
        read(fd1, sectName, 9);
        short int sectType = 0;
        read(fd1, &sectType, 2);
        int sectOffset = 0;
        read(fd1, &sectOffset, 4);
        int sectSize = 0;
        read(fd1, &sectSize, 4);
        printf("section%d: %s %d %d\n", i+1, sectName, sectType, sectSize);
    }
}

int extract(char *path, int section, int line, int print)
{
    int fd1 = -1;
    fd1 = open(path, O_RDONLY);
    if (fd1 == -1)
    {
        return -1;
    }
    lseek(fd1, -4, SEEK_END);
    short int sizeOfH = 0;
    read(fd1, &sizeOfH, 2);
    lseek(fd1, -sizeOfH, SEEK_END);
    short int version = 0;
    read(fd1, &version, 2);
    unsigned char nrSecC = ' ';
    read(fd1, &nrSecC, 1);
    int nrSec = nrSecC & 0XFF;
    if (nrSec < section)
    {
        return -2;
    }
    int offSet = 0;
    int sizeS = 0;
    for (int i = 0; i < section; i++)
    {
        char section[9] = "";
        read(fd1, section, 9);
        short int type = 0;
        read(fd1, &type, 2);
        read(fd1, &offSet, 4);
        read(fd1, &sizeS, 4);
    }
    int untilLine = 0;
    int i = 0;
    lseek(fd1, offSet, SEEK_SET);
    untilLine = 0;
    while (i <= sizeS)
    {
        char newLine = ' ';
        read(fd1, &newLine, 1);
        i++;
        if (newLine == '\n' || newLine == '\0')
        {
            untilLine++;
        }
        if (untilLine == line)
        {
            break;
        }
    }
    if (untilLine == line)
    {
        if (print == 1)
        {
            printf("SUCCESS\n");
            char extract = ' ';
            while (extract != '\n' && extract != '\0')
            {
                lseek(fd1, -2, SEEK_CUR);
                read(fd1, &extract, 1);
                printf("%c", extract);
            }
        }
        return 1;
    }
    else
    {
        return -3;
    }
}

void findAll(char *path, int success)
{
    DIR *dir = NULL;
    struct dirent *entry = NULL;
    char fullPath[512] = "";
    struct stat status;
    dir = opendir(path);
    if (dir == NULL)
    {
        if (success == 1)
        {
            perror("ERROR\n");
        }
        return;
    }
    else if (success == 1)
    {
        printf("SUCCESS\n");
        success = 0;
    }
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
        {
            snprintf(fullPath, 512, "%s/%s", path, entry->d_name);
            if (lstat(fullPath, &status) == -1)
            {
                continue;
            }
            else if (S_ISREG(status.st_mode))
            {
                int fd1 = -1;
                fd1 = open(fullPath, O_RDONLY);
                if (fd1 == -1)
                {
                    return;
                }
                lseek(fd1, -4, SEEK_END);
                int sizeOfH = 0;
                read(fd1, &sizeOfH, 2);
                lseek(fd1, -sizeOfH, SEEK_END);
                int version = 0;
                read(fd1, &version, 2);
                int nrSec = 0;
                read(fd1, &nrSec, 1);
                for (int i = 0; i < nrSec; i++)
                {
                    if (extract(fullPath, i, 16, 0) > 0)
                    {
                        printf("%s\n", fullPath);
                        break;
                    }
                }
            }
            if (S_ISDIR(status.st_mode))
            {
                findAll(fullPath, 0);
            }
        }
    }
    closedir(dir);
}

int main(int argc, char **argv)
{
    if (argc >= 2)
    {
        if (strcmp(argv[1], "variant") == 0)
        {
            printf("42590\n");
        }
        if (strcmp(argv[1], "list") == 0)
        {
            int recursive = 0;
            int size = -1;
            char *nameStarts;
            char *path;
            int nameStartsOperation = 0;
            for (int i = 1; i < argc; i++)
            {
                if (strcmp(argv[i], "recursive") == 0)
                {
                    recursive = 1;
                }
                if (strstr(argv[i], "size_greater") != NULL)
                {
                    char *sizeString = argv[i];
                    sizeString += 13;
                    size = atoi(sizeString);
                }
                if (strstr(argv[i], "name_starts_with") != NULL)
                {
                    nameStarts = argv[i];
                    nameStarts += 17;
                    nameStartsOperation = 1;
                }
            }
            if (nameStartsOperation == 0)
            {
                nameStarts = "null";
            }
            path = argv[argc - 1];
            path += 5;
            if (recursive == 1)
            {
                recursiveListing(path, 1, size, nameStarts);
            }
            else
            {
                simpleList(path, size, nameStarts);
            }
        }

        char *path;
        for (int i = 0; i < argc; i++)
        {
            if (strcmp(argv[i], "parse") == 0)
            {
                for (int j = 0; j < argc; j++)
                {
                    if (strstr(argv[j], "path=") != NULL)
                    {
                        path = argv[j];
                        path += 5;
                        parse(path);
                        break;
                    }
                }
            }
        }

        if (strcmp(argv[1], "extract") == 0)
        {
            char *path;
            path = argv[2];
            path += 5;
            char *section = argv[3];
            section += 8;
            char *line = argv[4];
            line += 5;
            int extractOutput = extract(path, atoi(section), atoi(line), 1); // 1 pentru a afisa line, mai avem nevoie de extract si la findall si nu vrem sa printeze linia
            if (extractOutput == -1)
            {
                printf("ERROR\ninvalid file");
            }
            else if (extractOutput == -2)
            {
                printf("ERROR\ninvalid section");
            }
            else if (extractOutput == -3)
            {
                printf("ERROR\ninvalid line");
            }
        }
        if (strcmp(argv[1], "findall") == 0)
        {
            char *path;
            path = argv[2];
            path += 5;
            findAll(path, 1);
        }
    }
    return 0;
}