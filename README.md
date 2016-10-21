# What's this?
A Socket programming practice written by `c`.
Just a simple file transfer from server to client.
***
## Program restriction
### 1. Image file title limit
My program only accept `PNG` image file, and file name must follow format   `test + Number.png`, `Number` must be under 10.  
For example, `test1.png` will work fine.

### 2. Use Mac OS as client BUG
If you use Mac OS as client, and server side is NOT Mac OS, then the receive data will corrupt(unknown reason).  
But if you run both client and server on Mac OS, it will work fine.  
***
## How to compile
These program were written with `C`, and i compile it with `gcc`.
Use `gcc` command to compile, e.g. 
```
gcc server.c -o server
```
***
## How to execute
### Server side
1. Switch to server file's location(the result after compile).
2. Type 
```
./server PortNumber
```

> Note: PortNumber is what port you want server to listen.
***
### Client side
1. Switch to client file's location(the result after compile).
2. Type 
```
./client IP PortNumber
```

> Note 1: IP is depends on where you place the server file(after compile).  

> Note 2: PortNumber is depends on server, you have to type the port number that server is listening.
***
## Result
Output file: `receive.png`

When client program ask you to type `image number`, it will generate the image file that correspond to your input.
The file which will be generate is a png file name `receive.png`.
***
## Test
OS environment: Mac OS 10.11.5, Ubuntu 14.04
Testing image file: test1.png, test2.png

### Local testing
Both client and server running on same machine
- Machine OS: Mac OS 10.11.5
- Server open port: 3000
- Result: success receive correct image file.

***

### Remote testing
  Execute two program on different place
- Client: running on Ubuntu 14.04
- Server: running on Mac OS 10.11.5
  - Open port: 3000
- Result: success receive correct image file.
