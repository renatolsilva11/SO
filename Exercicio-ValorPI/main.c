#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define NUM_POINTS 1000000
#define NUM_PROCESSES 4

pid_t *create_processes() {
  pid_t *pids = calloc(sizeof(*pids), NUM_PROCESSES - 1);

  for (int i = 0; i < NUM_PROCESSES - 1; i++) {
    pids[i] = fork();
    if (pids[i] != 0)
      break;
  }

  return pids;
}

void estimate_pi(int *inside_circle) {
  int num_points_per_process = NUM_POINTS / NUM_PROCESSES;

  for (int i = 0; i < num_points_per_process; ++i) {
    double x = (double)rand() / RAND_MAX;
    double y = (double)rand() / RAND_MAX;

    if (x * x + y * y <= 1.0) {
      (*inside_circle)++;
    }
  }
}

int main() {
  FILE *fp = fopen("tmp.txt", "w");
  if (!fp) {
    perror("error");
    exit(EXIT_FAILURE);
  }
  fclose(fp);

  pid_t *pids = create_processes();

  srand(time(NULL) ^ (getpid() << 16));

  int inside_circle = 0;
  estimate_pi(&inside_circle);

  while (wait(NULL) > 0) {
  }

  printf("inside_circle = %d\n", inside_circle);

  fp = fopen("tmp.txt", "r");

  if (!fp) {
    perror("error");
    exit(EXIT_FAILURE);
  }

  int file_in = 0;
  fscanf(fp, "%d", &file_in);

  inside_circle += file_in;

  if (freopen("tmp.txt", "w", fp) == NULL) {
    perror("error");
    exit(EXIT_FAILURE);
  }

  fprintf(fp, "%d", inside_circle);

  fclose(fp);

  if (pids[0] == 0) {
    exit(EXIT_SUCCESS);
  }

  double pi = 4.0 * inside_circle / NUM_POINTS;

  printf("Processo %d valor estimado de PI: %f\n", getpid(), pi);

  remove("tmp.txt");

  return 0;
}
