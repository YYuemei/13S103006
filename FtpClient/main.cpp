#include <iostream>
#include <winsock.h>
#include <stdio.h>
#include <string.h>
#include<fstream>
using namespace std;
#define DEFAULT_SERV_PORT 2416
#define DEFAULT_BUFFER_SIZE      2048
int getfun(SOCKET ss,string FileName);
int main()
{
    struct sockaddr_in server;
    struct hostent	*host;
    char *hostName="localhost";
    char recBuf[DEFAULT_BUFFER_SIZE];
    SOCKET ss;
    WSADATA wsadata;

    if (WSAStartup(MAKEWORD(2,2), &wsadata) != 0)  //winsock≥ı ºªØ
    {
        cout<<"Failed to load Winsock library!\n";
        return 1;
    }

    server.sin_family = AF_INET;
    server.sin_port=htons(DEFAULT_SERV_PORT);
    host = gethostbyname(hostName);
    memcpy(&server.sin_addr, host->h_addr, host->h_length);

    ss=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ss==INVALID_SOCKET)
    {
        cout<<"creating socket failed\n";
        return 1;
    }

    if(connect(ss,(struct sockaddr *)&server,sizeof(server))==SOCKET_ERROR)
    {
        cout<<"connecting error\n";
        return 1;
    }

    int ret;
    ret=recv(ss,recBuf,DEFAULT_BUFFER_SIZE,0);
    if (ret == 0)   return 0;
    else if (ret == SOCKET_ERROR)
    {
        printf("recv function failed,error_num: %d\n", WSAGetLastError());
        return 0;
    }
    recBuf[ret]='\0';
    cout<<recBuf<<endl;
    string cmd1,cmd2;

    while(1)
    {
        cout<<"-------------------"<<endl;
        cin>>cmd1;

        if(cmd1=="get")
        {
            cin>>cmd2;
            getfun(ss,cmd2);
            continue;
        }
        else if(cmd1 == "quit")
        {
            char senBuf[80];
             char reBuf[80];
             int ret;
            senBuf[0]='\0';
            strcat(senBuf,"quit$");
            send(ss,senBuf,strlen(senBuf)+1,0);
            ret=recv(ss,reBuf,80,0);
            reBuf[ret]='\0';
            cout<<reBuf<<endl;
            break;
        }
    }
    closesocket(ss);
    WSACleanup( );
    return 0;
}

int getfun(SOCKET ss,string FileName)
{
    ofstream file;
    string path;
    path="C:\\Users\\Administrator\\Desktop\\";
    path+=FileName;
    file.open( path, ios::out );
    char reBuf[80];
    char senBuf[80];
    senBuf[0]='\0';
    strcat(senBuf,"get$");
    strcat(senBuf,FileName.c_str());
    int ret;
    ret =send(ss,senBuf,strlen(senBuf)+1,0);
    cout<<senBuf<<endl;
    if (ret == 0)  return 1;
    else if (ret == SOCKET_ERROR)
    {
        printf("send function failed,error_num: %d\n", WSAGetLastError());
        return 1;
    }
    cout<<"send"<<ret<<"bytes successfully!!\n";
    ret=recv(ss,reBuf,80,0);
    reBuf[ret]='\0';
    file<<reBuf;
    cout<<reBuf<<endl;
    while(1)
    {
        ret=recv(ss,reBuf,80,0);
        reBuf[ret]='\0';
        if(strncmp(reBuf,"finished!!!",strlen("finished!!!"))==0)
            break;
        file<<reBuf;
        cout<<reBuf<<endl;
        cout<<"receive"<<ret<<"bytes successfully!!\n";

    }
    file.close();
    return 0;
}
