#include "pse.h"
#include "defs.h"

#define CMD   "client"

int main(int argc, char *argv[]) {
  int sd, arret = FAUX, ret;
  struct sockaddr_in *sa;
  char buf[LIGNE_MAX];

  signal(SIGPIPE, SIG_IGN);

  if (argc != 3) {
    erreur("usage: %s machine port\n", argv[0]);
  }

  printf("%s: creating a socket\n", CMD);
  sd = socket (AF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    erreur_IO("socket");
  }

  printf("%s: DNS resolving for %s, port %s\n", CMD, argv[1], argv[2]);
  sa = resolv(argv[1], argv[2]);
  if (sa == NULL) {
    erreur("adresse %s port %s inconnus\n", argv[1], argv[2]);
  }
  printf("%s: adr %s, port %hu\n", CMD,
	 stringIP(ntohl(sa->sin_addr.s_addr)),
	 ntohs(sa->sin_port));

  /* connexion sur site distant */
  printf("%s: connecting the socket\n", CMD);
  ret = connect(sd, (struct sockaddr *) sa, sizeof(struct sockaddr_in));
  if (ret < 0) {
    erreur_IO("Connect");
  }

  freeResolv();
  
  while (arret == FAUX) {
    printf("ligne> ");
    if (fgets(buf, LIGNE_MAX, stdin) == NULL) {
      printf("Fin de fichier (ou erreur) : arret.\n");
      arret = VRAI;
      continue;
    }
    else {
      ret = ecrireLigne(sd, buf);
      if (ret == -1) {
	erreur_IO("ecrireLigne");
      }
      if (strcmp(buf, "fin\n") == 0) {
	printf("%s: arret demande.\n", CMD);
	arret = VRAI;
      }
      else {
	printf("%s: ligne de %d octets envoyee au serveur.\n", CMD, ret);
      }
    }
  }

  if (close(sd) == -1) {
    erreur_IO("close");
  }

  exit(EXIT_SUCCESS);
}


