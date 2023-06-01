#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define RESP "RESP_PIPE_42590"
#define REQ "REQ_PIPE_42590"
#define SHM "/bZusaZ"
int main() {

  if (mkfifo(RESP, 0600) != 0) {
    perror("ERROR\ncannot create the response pipe | cannot open the request "
           "pipe");
    return -1;
  }

  int fd1 = -1;
  int fd2 = -1;

  fd1 = open(REQ, O_RDONLY | O_NONBLOCK);
  if (fd1 == -1) {
    perror("ERROR\ncannot create the response pipe | cannot open the request "
           "pipe");
    return -1;
  }

  fd2 = open(RESP, O_WRONLY);
  if (fd2 == -1) {
    perror("ERROR\ncannot create the response pipe | cannot open the request "
           "pipe");
    return -1;
  }

  char start[6] = "START$";

  if (write(fd2, start, sizeof(char) * 6) < 0) {
    return -1;
  }

  printf("SUCCESS");

  char request[255];
  unsigned int size = -1;
  int shmFd = -1;
  int *shmPtr = NULL;
  int fd = -1;
  char *shmFile = NULL;

  while (1) {
    char requestChar;
    read(fd1, &requestChar, sizeof(char));
    strncat(request, &requestChar, 1);
    
    if (strstr(request, "PING") != NULL) {
      write(fd2, "PING$", 5);
      write(fd2, "PONG$", 5);
      unsigned int version = 42590;
      write(fd2, &version, sizeof(unsigned int));
      strcpy(request, "");
    }

    if (strstr(request, "CREATE_SHM") != NULL) {
      read(fd1, &size, sizeof(unsigned int));
      shmFd = shm_open("/bZusaZ", O_CREAT | O_RDWR, 0664);
      if (shmFd < 0) {
        write(fd2, "CREATE_SHM$", sizeof(char) * 11);
        write(fd2, "ERROR$", sizeof(char) * 6);
        return -1;
      }

      if (ftruncate(shmFd, sizeof(char) * size) == -1) {
        write(fd2, "CREATE_SHM$", sizeof(char) * 11);
        write(fd2, "ERROR$", sizeof(char) * 6);
        return -1;
      }

      shmPtr = mmap(0, sizeof(char) * size, PROT_READ | PROT_WRITE, MAP_SHARED,
                    shmFd, 0);

      if (shmPtr == (void *)-1) {
        write(fd2, "CREATE_SHM$", sizeof(char) * 11);
        write(fd2, "ERROR$", sizeof(char) * 6);
        return -1;
      }
      write(fd2, "CREATE_SHM$", sizeof(char) * 11);
      write(fd2, "SUCCESS$", sizeof(char) * 8);
      strcpy(request, "");
    }

    if (strstr(request, "WRITE_TO_SHM") != NULL) {
      unsigned int offset;
      unsigned int value;
      read(fd1, &offset, sizeof(unsigned int));
      read(fd1, &value, sizeof(unsigned int));

      
 if (offset > 0 && offset < size - 4) {
        shmPtr[offset] = value;
        write(fd2, "WRITE_TO_SHM$", 13);
        write(fd2, "SUCCESS$", sizeof(char) * 8);
      } else {
        write(fd2, "WRITE_TO_SHM$", 13);
        write(fd2, "ERROR$", sizeof(char) * 6);
        }
      strcpy(request, "");
    }

    if (strstr(request, "MAP_FILE") != NULL) {
    char file[255];
    read(fd1, &requestChar, sizeof(char));
    read(fd1, file, 255);
    file[strlen(file)-1] = '\0';
    fd = open(file, O_RDONLY);
    if(fd == -1)
    {
      write(fd2, "MAP_FILE$", 9);
      write(fd2, "ERROR$", sizeof(char) * 6);
      exit(0);
    }
    struct stat stat;
    fstat(fd, &stat);
    int fileSize = stat.st_size;
    shmFile = mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd, 0);
    if(shmFile == (void*)-1)
    {
      write(fd2, "MAP_FILE$", 9);
      write(fd2, "ERROR$", sizeof(char) * 6);
    }else
    {
      write(fd2, "MAP_FILE$", 9);
      write(fd2, "SUCCESS$", sizeof(char) * 8);
    }
      strcpy(request, "");
    }

    if (strstr(request, "READ_FROM_FILE_OFFSET") != NULL) {
    unsigned int offset;
      unsigned int bytes;
      read(fd1, &offset, sizeof(unsigned int));
      read(fd1, &bytes, sizeof(unsigned int));
      char *shmTextFromFile = NULL;
      shmTextFromFile = shmFile + offset;
      if(shmTextFromFile != NULL)
    {
      write(fd2, "READ_FROM_FILE_OFFSET$", 22);
      write(fd2, "ERROR$", sizeof(char) * 6);
    }else
    {
     write(fd2, "READ_FROM_FILE_OFFSET$", 22);
      write(fd2, "SUCCESS$", sizeof(char) * 8);
    }
      strcpy(request, "");
      
    }

    if (strstr(request, "READ_FROM_FILE_SECTION") != NULL) {
      exit(0);
    }
    if (strstr(request, "READ_FROM_LOGICAL_SPACE_OFFSET") != NULL) {
      exit(0);
    }

    if (strstr(request, "EXIT") != NULL) {
      strcpy(request, "");
      close(fd1);
      close(fd2);
      close(fd);
      unlink(RESP);
      munmap((void*)shmPtr, size);
      unlink(REQ);
      exit(0);
    }
  }

  close(fd1);
  close(fd2);
  unlink(RESP);
  unlink(REQ);
  return 0;
}
