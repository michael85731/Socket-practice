#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *);

int main(int argc, char *argv[])
{
   int sockfd, newsockfd, servlen;
   socklen_t clilen;
   struct sockaddr_in  cli_addr, serv_addr;
   char buffer[2];

   //建立socket
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
       error("creating socket");
   memset((char *)&serv_addr, 0, sizeof(serv_addr));

   //設定連線參數
   int portNum = atoi(argv[1]);

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   serv_addr.sin_port = htons(portNum);

   //Bind socket
   if(bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
       error("binding socket");

   //開始listen
   listen(sockfd, 5);
   clilen = sizeof(cli_addr);

   //Accept socket communication
   newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
   if (newsockfd < 0)
        error("accepting");

   printf("Receive connect request from %s port\n", argv[1]);

   //把client傳來的資料讀進buffer陣列中
   memset(buffer, 0, sizeof(buffer) * strlen(buffer));  //清空buffer
   read(newsockfd, buffer, sizeof(buffer));

   printf("Read image %c and send back to client\n", buffer[0]);

   //讀取client指定的圖片編號，並讀取到fb這個char array中
   char fileName[] = "test .png";
   fileName[4] = buffer[0];

   FILE *fp = fopen(fileName, "rb");
   char *fb;
   int fileSize = 0;

   if(fp){
     //取得檔案大小
     fseek(fp, 0, SEEK_END);
     fileSize = ftell(fp);
     fseek(fp, 0, SEEK_SET);

     //分配給fb欲讀取檔案的空間，並讀取檔案到fb
     fb = (char *)malloc(sizeof(char) * fileSize);
     fread(fb, sizeof(char), fileSize, fp);
   }

   //送資料回client端
   int first = 1; //是否為第一次傳資料
   int left = fileSize;  //記錄尚未傳送的資料數
   int bufferWritten = 0;  //紀錄已傳送的資料數
   char *readPointer = fb; //未送出的資料位置

   while(left > 0){
      if(first){
        //送檔案大小給client
        int fileSizeForTranfer[1] = { fileSize }; //因應write函式的內容，所以才用array
        write(newsockfd, fileSizeForTranfer, sizeof(int));
        printf("要傳送的檔案大小為：%d\n", fileSize);
        first = 0;
      } else{
        //送資料出去，每次從read_pointer送left筆資料出去(還沒送出去的資料數量)
        bufferWritten = write(newsockfd, readPointer, left);

        left -= bufferWritten;
        readPointer += bufferWritten; //把pointer移到上一次寫完後的位置
      }
   }

   printf("Disconnected");
   close(newsockfd);
   close(sockfd);
   fclose(fp);
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
