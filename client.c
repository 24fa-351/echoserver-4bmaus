#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <IP> <port>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  char *ip = argv[1];
  int port = atoi(argv[2]);

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server_addr = {0};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(port);
  if (inet_pton(AF_INET, ip, &server_addr.sin_addr) <= 0) {
    perror("Invalid address");
    close(sock);
    exit(EXIT_FAILURE);
  }

  if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("Connection failed");
    close(sock);
    exit(EXIT_FAILURE);
  }

  printf("Connected to server. Type your message and press enter to send.\n");

  char buffer[BUFFER_SIZE];
  while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
    send(sock, buffer, strlen(buffer), 0);
    ssize_t bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0)
      break;

    buffer[bytes_received] = '\0';
    printf("Echo from server: %s", buffer);
  }

  close(sock);
  return 0;
}
