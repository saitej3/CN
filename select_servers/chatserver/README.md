#Chat Server

Steps:

1. Compile the server.c and client.c ``` gcc server.c -o server; gcc client.c -lpthread -o clientprog```
2. Depending on the number of clients start a client as ```./clientprog client1 ```
3. Pass the clients to server ``` ./server client1 client2 ....... clienti ```
4. Chat away
