#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <winsock.h>

using namespace std;
#define	WSVERS		MAKEWORD(2, 0)
#define UdpFtpPort 2345
void getFoo(SOCKET s,string fileName);
int main()
{
    char *host="localhost";
    SOCKET s;
    struct sockaddr_in sin;
    struct protoent *ppe;
    struct hostent	*phe;



    WSADATA wsadata;
    if (WSAStartup(WSVERS, &wsadata))
        cout<<"start error"<<endl;
    memset(&sin,0,sizeof(sin));
    sin.sin_family=AF_INET;
    sin.sin_port=htons(UdpFtpPort);
    phe = gethostbyname(host);
    memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    ppe = getprotobyname("udp");
    s=socket(PF_INET,SOCK_DGRAM,ppe->p_proto);
   if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) ==
	    SOCKET_ERROR)
	    cout<<"connect error"<<endl;

    string cmd1,cmd2;
    char sendBuf[100];
    while(true)
    {
        cin>>cmd1;
        if(cmd1=="quit")
        {
            char sendBuf[100]="quit";
            send(s,sendBuf,strlen(sendBuf)+1,0);
            break;
        }
        else if(cmd1=="get")
        {
            cin>>cmd2;
            char sendBuf[100]="get$";
            strcat(sendBuf,cmd2.c_str());
            send(s,sendBuf,strlen(sendBuf)+1,0);
            getFoo(s,cmd2);
        }
    }
    cout << "Hello world!" << endl;
    return 0;
}

void getFoo(SOCKET s,string fileName)
{
    FILE *p;
    string path;
    path="C:\\Users\\Administrator\\Desktop\\";
    path=path+fileName;
    p=fopen(path.c_str(),"wb");
    char recBuf[100];
    int rec_len;
    while(1)
    {
        rec_len=recv(s,recBuf,100,0);
        cout<<"bbbbbb "<<recBuf<<" "<<rec_len<<endl;
        if(rec_len<=0)
        {
            cout<<"recv finish"<<endl;
            break;
        }
        fwrite(recBuf,1,rec_len,p);
    }
    fclose(p);
}
