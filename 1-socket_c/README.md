
# Socket Programming Assignment 1: Min-Number Game

## Project Overview
This project implements a multi-process client-server architecture using C sockets. The system consists of one server (ID 0) and three clients (IDs 1-3). Clients generate random numbers, and the server determines the winner (lowest number) and returns the average of the submitted values.

## System Requirements
* **OS:** Linux (Developed/Tested on Fedora Linux 43)
* **Compiler:** GCC (GNU Compiler Collection)

## File Structure
* `server.c` - Server logic (Handle connections, compute average, determine winner).
* `client.c` - Client logic (Generate numbers, send data, receive result).
* `README.md` - Documentation and execution steps.

## Compilation Instructions
Open your terminal and run the following commands to compile the source code:

```bash
# Compile the Server
gcc -o server server.c

# Compile the Client
gcc -o client client.c

```

## Execution Instructions

This assignment requires **four separate terminal windows** to simulate distinct processes. Follow this strict order:

**1. Terminal 1 (The Server)**
Start the server first. It will listen for incoming connections.

```bash
./server

```

*Status: Server waits for 3 clients to connect.*

**2. Terminal 2 (Client 1)**

```bash
./client 1

```

*Status: Connects, sends data, and **waits** (blocks) until other clients join.*

**3. Terminal 3 (Client 2)**

```bash
./client 2

```

*Status: Connects, sends data, and **waits**.*

**4. Terminal 4 (Client 3)**

```bash
./client 3

```

*Status: Connects and sends data. This triggers the server to process logic. All terminals will now update with results and terminate.*

## Protocol Logic

1. **Handshake:** The Server accepts connections from exactly 3 clients.
2. **Data Transmission:** Each client generates a random integer (1-100) and sends it to the server.


3. **Processing:**
* The Server collects all 3 numbers.
* It identifies the **minimum number** (Winner).


* It calculates the **average** of the three numbers.




4. **Response:**
* The Winner receives the calculated average.
* Losers receive a termination signal (-1.0).


5. 
**Termination:** The winning client prints the average and its own minimum number, and all processes terminate.

