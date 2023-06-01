#include "a2_helper.h"
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

typedef struct {
  int processId;
  int threadId;
} threadInfo;

int finished = 0;

sem_t *sem5_2 = NULL;
sem_t *sem4_3 = NULL;
sem_t *sem5_5 = NULL;

sem_t sem4_1, sem4_2, sem, sem12, sem3;
pthread_mutex_t mutex;

void *thread_fn2_3(void *arg) {
  threadInfo *thisThread = (threadInfo *)arg;

  if (thisThread->threadId == 1 && thisThread->processId == 4) {
    sem_wait(&sem4_1);
  }

  if (thisThread->threadId == 2 && thisThread->processId == 4) {
    sem_wait(&sem4_2);
  }

  if (thisThread->threadId == 3 && thisThread->processId == 4) // 2.5
  {
    sem_wait(sem4_3);
  }

  if (thisThread->threadId == 5 && thisThread->processId == 5) // 2.5
  {
    sem_wait(sem5_5);
  }

  info(BEGIN, thisThread->processId, thisThread->threadId);

  if (thisThread->processId == 4 && thisThread->threadId == 1) {
    sem_post(&sem4_2);
    sem_wait(&sem4_1);
  }

  info(END, thisThread->processId, thisThread->threadId);
  if (thisThread->threadId == 2 && thisThread->processId == 4) {
    sem_post(&sem4_1);
  }
  if (thisThread->processId == 5 && thisThread->threadId == 2) // 2.5
  {
    sem_post(sem4_3);
  }

  if (thisThread->threadId == 3 && thisThread->processId == 4) // 2.5
  {
    sem_post(sem5_5);
  }
  return NULL;
}

void *thread_fn2_4(void *arg) {
  threadInfo *thisThread = (threadInfo *)arg;
  if (thisThread->threadId != 12 && thisThread->processId==3) {
    sem_wait(&sem);
  }
  info(BEGIN, thisThread->processId, thisThread->threadId);
  if (thisThread->threadId == 12 && thisThread->processId==3) {
    for (int i = 0; i < 3; i++){
      sem_post(&sem);}
      finished = 0;
    sem_wait(&sem12);
  }
  
  if (finished == 0 && thisThread->threadId != 12 && thisThread->processId==3) {
    sem_post(&sem12);
    sem_wait(&sem3);
  }
  
  info(END, thisThread->processId, thisThread->threadId);
  if (thisThread->threadId == 12 && thisThread->processId==3) {
    sem_post(&sem3);
    sem_post(&sem3);
    sem_post(&sem3);
  }
  pthread_mutex_lock(&mutex);
  finished++;
  pthread_mutex_unlock(&mutex);
  if (finished % 4 == 0 && thisThread->processId==3) {
    for (int i = 0; i < 4; i++) {
      sem_post(&sem);
    }
  }
  return NULL;
}

int main() {
  init();

  info(BEGIN, 1, 0);

  sem_init(&sem4_1, 0, 1);
  sem_init(&sem4_2, 0, 0);
  sem_init(&sem, 0, 0);
  sem_init(&sem12, 0, 0);
  sem_init(&sem3, 0, 0);

  sem5_2 = sem_open("/sem5_2", O_CREAT, 0666, 0);
  sem4_3 = sem_open("/sem4_3", O_CREAT, 0666, 0);
  sem5_5 = sem_open("/sem5_5", O_CREAT, 0666, 0);

  pthread_mutex_init(&mutex, NULL);

  pthread_t tid3[46];
  threadInfo tP3[46];

  for (int i = 0; i < 46; i++) {
    tP3[i].processId = 3;
    tP3[i].threadId = i + 1;
  }

  pthread_t tid4[4];
  threadInfo tP4[4];

  for (int i = 0; i < 4; i++) {
    tP4[i].processId = 4;
    tP4[i].threadId = i + 1;
  }

  pthread_t tid5[5];
  threadInfo tP5[5];
  for (int i = 0; i < 5; i++) {
    tP5[i].processId = 5;
    tP5[i].threadId = i + 1;
  }

  int pid2 = -1, pid3 = -1, pid4 = -1, pid5 = -1, pid6 = -1, pid7 = -1;
  int status = 0;

  pid2 = fork();
  if (pid2 == 0) {
    info(BEGIN, 2, 0);
    info(END, 2, 0);
    return 0;
  }
  waitpid(pid2, &status, 0);

  pid3 = fork();
  if (pid3 == 0) {
    info(BEGIN, 3, 0);

    for (int i = 0; i < 46; i++) {
      pthread_create(&tid3[i], NULL, thread_fn2_4, &tP3[i]);
    }
    for (int i = 0; i < 46; i++) {
      pthread_join(tid3[i], NULL);
    }

    pid4 = fork();
    if (pid4 == 0) {
      info(BEGIN, 4, 0);

      for (int i = 0; i < 4; i++) {
        pthread_create(&tid4[i], NULL, thread_fn2_3, &tP4[i]);
      }

      pid5 = fork();
      if (pid5 == 0) {
        info(BEGIN, 5, 0);

        for (int i = 0; i < 5; i++) {
          pthread_create(&tid5[i], NULL, thread_fn2_3, &tP5[i]);
        }

        for (int i = 0; i < 5; i++) {
          pthread_join(tid5[i], NULL);
        }

        pid7 = fork();
        if (pid7 == 0) {
          info(BEGIN, 7, 0);
          info(END, 7, 0);
          return 0;
        }
        waitpid(pid7, &status, 0);

        info(END, 5, 0);
        return 0;
      }
      waitpid(pid5, &status, 0);

      for (int i = 0; i < 4; i++) {
        pthread_join(tid4[i], NULL);
      }

      info(END, 4, 0);
      return 0;
    }
    waitpid(pid4, &status, 0);

    pid6 = fork();
    if (pid6 == 0) {
      info(BEGIN, 6, 0);
      info(END, 6, 0);
      return 0;
    }
    waitpid(pid6, &status, 0);

    info(END, 3, 0);
    return 0;
  }
  waitpid(pid3, &status, 0);

  sem_destroy(&sem4_1);
  sem_destroy(&sem4_2);

  sem_destroy(&sem);
  sem_destroy(&sem12);
  sem_destroy(&sem3);

  sem_close(sem5_2);
  sem_unlink("/sem5_2");
  sem_close(sem5_5);
  sem_unlink("/sem5_5");
  sem_close(sem4_3);
  sem_unlink("/sem4_3");

  pthread_mutex_destroy(&mutex);
  info(END, 1, 0);
  return 0;
}