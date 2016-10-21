#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void error(const char *);

int main(int argc, char *argv[])
{
   int sockfd, servlen;
   struct sockaddr_in serv_addr; //宣告了一個sockaddr_un型態的變數為serv_addr
   char *buffer = (char *)malloc(sizeof(char));

   // 清空serv_addr這塊記憶體空間，其用意不明
   bzero((char *)&serv_addr, sizeof(serv_addr)); //把serv_addr這塊記憶體區間清掉

   //設定連線參數(IP, Port Num)
   char *ip = argv[1];
   int portNum = atoi(argv[2]);

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr(ip);
   serv_addr.sin_port = htons(portNum);

   //建立socket連線到server端，並回傳file descriptor編號
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
       error("Creating socket");
   if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
       error("Connecting");

   printf("Connect %s:%s success!\n", argv[1], argv[2]);
   printf("Please enter image number: ");

   //送要選取的圖片編號給server端
   memset(buffer, 0, sizeof(buffer) * strlen(buffer));  //清空buffer

   fgets(buffer, 2, stdin);
   write(sockfd, buffer, sizeof(buffer));

   //接收server傳來的資料
   int first = 1;
   int left = 0; //紀錄還有多少沒有讀
   int fileSize = 0;
   char *fb; //存放server端傳來的資料

   while(left > 0 || first == 1){
      if(first){
        //接收server傳來的檔案大小資料
        int receiveFileSize[1]; //記錄檔案大小，因應read所以才用array
        read(sockfd, receiveFileSize, sizeof(int));

        fileSize = receiveFileSize[0];
        left = fileSize;
        printf("要接收的檔案大小為： %d\n", fileSize);

        //開記憶體空間，儲存server端傳來的檔案資料
        fb = (char *)malloc(sizeof(char) * fileSize);
        first = 0;
      }else{
        //把資料讀到fb中
        int nowRead = read(sockfd, fb, left); //紀錄實際讀多少
        left -= nowRead; //剩多少還沒有讀
      }
   }

   //把server端傳回的binary資料轉成png檔案
   FILE *fp = fopen("receive.png", "wb");
   fwrite(fb, sizeof(char), fileSize, fp);
   fclose(fp);

   close(sockfd);
   printf("Store image file as receive.png\n");
   printf("Disconnected");
   return 0;
}

void error(const char *msg)
{
    perror(msg);
    exit(0);
}
