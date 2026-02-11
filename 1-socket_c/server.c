/*
 * Name: Asim Kalim
 * Roll Number: IIT2023219
 *
 * Description of experimental set-up:
 * Code developed on Fedora Linux 43 (Workstation Edition) using GCC 15.2.1
 * Compilation flags: -o server server.c
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
#include <limits.h>

#define PORT 8080
#define MAX_CLIENTS 3

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    int client_sockets[MAX_CLIENTS];
    int client_numbers[MAX_CLIENTS]; // Stores 1 number per client

    // --- 1. Socket Creation ---
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("[Server ID 0] Server Started. WAITING for 3 clients to connect...\n");
    printf("[Server ID 0] Note: I will not proceed until ALL 3 are connected.\n");

    // --- 2. Accept 3 Clients (BLOCKING STEP) ---
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        client_sockets[i] = new_socket;
        printf("[Server ID 0] Client %d connected. (%d/%d)\n", i+1, i+1, MAX_CLIENTS);
    }
    
    printf("[Server ID 0] All 3 clients connected. Receiving data...\n");

    // --- 3. Receive 1 Number from Each Client ---
    for (int i = 0; i < MAX_CLIENTS; i++) {
        int valread = read(client_sockets[i], &client_numbers[i], sizeof(int));
        if (valread > 0) {
            printf("[Server ID 0] Received %d from Client %d\n", client_numbers[i], i+1);
        }
    }

    // --- 4. Logic: Find Min and Average ---
    int min_val = INT_MAX;
    int winner_idx = -1;
    int sum = 0;

    for (int i = 0; i < MAX_CLIENTS; i++) {
        sum += client_numbers[i];
        if (client_numbers[i] < min_val) {
            min_val = client_numbers[i];
            winner_idx = i;
        }
    }

    float average = sum / 3.0; // Average of "the three numbers"
    printf("[Server ID 0] Minimum: %d (Client %d). Average: %.2f. Sending results...\n", min_val, winner_idx+1, average);

    // --- 5. Send Results ---
    for (int i = 0; i < MAX_CLIENTS; i++) {
        float msg;
        if (i == winner_idx) {
            msg = average; // Winner gets the average
        } else {
            msg = -1.0; // Losers get -1.0
        }
        send(client_sockets[i], &msg, sizeof(float), 0);
        close(client_sockets[i]);
    }

    close(server_fd);
    printf("[Server ID 0] Terminating.\n");
    return 0;
}
