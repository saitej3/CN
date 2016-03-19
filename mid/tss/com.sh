gcc server.c -o server
gcc service1.c -o service1 -pthread
gcc service2.c -o service2 -pthread
gcc service3.c -o service3 -pthread
gcc client.c -o client