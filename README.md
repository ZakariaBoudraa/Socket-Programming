# Socket-Programming
This project is a multi-user chat room that is implemented using C and socket programming. It is designed over TCP and IP protocols, and it includes a server socket as well as a client socket. The program also makes use of polling and the fcntl() function to enable the O_NONBLOCK setting on file descriptors.

## File Structure
<pre>
  +--- socket-programming 
      +---.gitignore
      +---client.c
      +---Makefile
      +---README.md
      +---server.c
</pre>

## Objective

The purpose of this small project is to serve as a practical demonstration of core concepts in C, including pointers, structs, and file reading and manipulation. 