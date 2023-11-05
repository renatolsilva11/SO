#include <errno.h>
#include <stdio.h>
#include <sys/utsname.h>

int main() {
  struct utsname uts;

  if (uname(&uts) == -1) {
    perror("uname");
    return 1;
  }

  printf("Sistema: %s\n", uts.sysname);
  printf("Nodename: %s\n", uts.nodename);
  printf("Release: %s\n", uts.release);
  printf("Versão: %s\n", uts.version);
  printf("Máquina: %s\n", uts.machine);

  return 0;
}


// Comandos no Linux

// gcc -o programa_nome programa.c
//./programa_nome
// strace -o output.txt ./programa_nome
