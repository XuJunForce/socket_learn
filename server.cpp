#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdio.h>
int main(){

    //创建socket
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd==-1){
        perror("socket");
        return -1;
    }

    //sockaddr_in储存服务器的addr and port 
    //sockaddr_in ----->   sockaddr
    struct sockaddr_in address;
    bzero(&address,0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(9998);

    //bind 用于绑定address中的信息
    int ret = bind(listenfd, (struct sockaddr*)&address, sizeof(address));
    if(ret == -1){
        perror("bing");
        return -1;
    }

    ret = listen(listenfd, 128);
    if(ret == -1){
        perror("listen");
        return -1;
    }

    struct sockaddr_in caddr;
    socklen_t addrlen = sizeof(caddr);
    int cfd = accept(listenfd, (struct sockaddr*)&caddr, &addrlen);
    

    if(cfd == -1){
        perror("accept");
        return -1;
    }

    //连接成功，接下来打印client's ip and port    
    char ip[32];
    printf("clinet IP: %s,  port:%d \n",inet_ntop(AF_INET,&caddr.sin_addr.s_addr,ip,sizeof(ip)));

    //With Clinet communicate
    while(1){
        //接收数据
        char buff[1024];
        int len = recv(cfd, buff, sizeof(buff), 0);
        if(len>0){
            printf("clinet says:%s\n",buff);
            send(cfd, buff, len, 0);
        }
        else if(len ==0){
            printf("clinet disconnected\n");
            break;
        }
        else if(len < 0){
            perror("recv");
            break;
        }
    }

    //关闭描述符
    close(cfd);
    close(listenfd);



    return 0;
}


