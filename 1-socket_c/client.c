/*
 * Name: Asim Kalim
 * Roll Number: IIT2023219
 *
 * Description of experimental set-up:
 * Code developed on Fedora Linux 43 (Workstation Edition) using GCC 15.2.1
 * Compilation flags: -o server server.c and -o client client.c
 *
 * Remarks/Observations:
 * The server handles 3 clients sequentially or concurrently.
 * Handshake uses TCP sockets. Random seed based on PID/Time ensures uniqueness.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    if (argc != 2) {
        printf("Usage: ./client <Client_ID>\n");
        return -1;
    }

    int client_id = atoi(argv[1]);
    int sock = 0;
    struct sockaddr_in serv_addr;

    // Unique random seed per client
    srand(time(NULL) ^ (getpid() << 16));

    // 1. Generate ONE number (1-100)
    int my_number = (rand() % 100) + 1;
    printf("[Client ID %d] Generated number: %d\n", client_id, my_number);

    // 2. Create Socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 3. Connect to Server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed. Ensure Server is running first.\n");
        return -1;
    }

    // 4. Send Number
    send(sock, &my_number, sizeof(int), 0);
    printf("[Client ID %d] Sent number (%d) to Server. Waiting for result...\n", client_id, my_number);

    // 5. Wait for Response (BLOCKING STEP)
    // The client will hang here until the server has received input from ALL 3 clients.
    float response;
    int valread = read(sock, &response, sizeof(float));

    if (valread > 0) {
        if (response == -1.0) {
            printf("[Client ID %d] I did not win. Terminating.\n", client_id);
        } else {
            // [cite: 6] "The minimum number is then printed by this client"
            printf("[Client ID %d] I WON! The Average is: %.2f\n", client_id, response);
            printf("[Client ID %d] My Minimum Number was: %d\n", client_id, my_number);
        }
    } else {
        printf("[Client ID %d] Server disconnected.\n", client_id);
    }

    close(sock);
    return 0;
}
