#include <iostream> 
#include <cstring>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <cstdlib>
#include <pthread.h>
#include <typeinfo>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <math.h>
#include "sha1.h"
#include<fcntl.h>
#define PORT 8080
#define BUFF_SIZE 524288
//#define BUFF_SIZE 200
#define BUFF 200
#define SIZE 200

using namespace std;

struct file_list
{
	vector<string>paths;
	char dpath[SIZE];
	long port_num;
	int i;
	int size_z;
};
struct file_list data;
int SIZE_1;
void work(struct file_list val)
{
	int sock_fd1;
	sock_fd1 = socket(AF_INET, SOCK_STREAM,0);
	if(sock_fd1 == 0)
	{
		cout<<"Error sock_fd1"<<endl;
	}
	struct sockaddr_in serv_addr1;
    serv_addr1.sin_family = AF_INET;
	serv_addr1.sin_port = htons(val.port_num);
    serv_addr1.sin_addr.s_addr=inet_addr("127.0.0.1");
    if(connect(sock_fd1,(struct sockaddr *)&serv_addr1,sizeof(serv_addr1))<0)
    {
    	cout<<"Error connect"<<endl;
    }
    
    int count=val.paths.size();
	send(sock_fd1,&count,sizeof(count),0);
	for(int i=0;i<count;i++)
	{
		
		int j;
		char p[SIZE];
		for(j=0;j<val.paths[i].size();j++)
		{
			p[j]=val.paths[i][j];
		}
		p[j]='\0';
		send(sock_fd1,p,sizeof(p),0);
	}
	int j;
	char dpath[SIZE];
	for(j=0;j<sizeof(val.dpath);j++)
	{
		dpath[j]=val.dpath[j];
	}
	dpath[j]='\0';
	send(sock_fd1,dpath,sizeof(dpath),0);
	int si=val.size_z;
	send(sock_fd1,&si,sizeof(si),0);
}
void *cliFunc(void *p)
{	
	long port;
	port=(long)p;
	
	int sock_fd,n;
	sock_fd=socket(AF_INET, SOCK_STREAM,0);
	if(sock_fd==0)
	{
		cout<<"Error 1"<<endl;
	}
    	struct sockaddr_in serv_addr;
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_port = htons(PORT);
    	serv_addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    	if(connect(sock_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
    	{
    		cout<<"Error 2 client"<<endl;
    	}

	send(sock_fd,&port,sizeof(port),0);
	int len1,len2;
	string input;
	struct file_list f1;
	while(1)
	{
		cout<<"Enter string: ";
		getline(cin,input);
		send(sock_fd, input.data(), input.size(), 0);
		if(input.find("exit") == 0)
		{
			break;
		}
		if(input.find("download_file") == 0)
		{
			long result=0;
			recv(sock_fd,&f1.port_num, sizeof(f1.port_num),0);
			recv(sock_fd,&result, sizeof(result),0);
			f1.i=result;
			recv(sock_fd,f1.dpath,sizeof(f1.dpath),0);
			//cout<<"f1.dpath "<<f1.dpath<<endl;
			recv(sock_fd,&f1.size_z,sizeof(f1.size_z),0);
			f1.paths.clear();
			if(result>0)
			{
				char path[SIZE];
				for(int i=0;i<result;i++)
				{
					recv(sock_fd,path,sizeof(path),0);
					f1.paths.push_back(path);
				}
			}
			work(f1);
		}
	}
	close(sock_fd);
}
void do_work(int c)
{
	//cout<<"In_do work"<<endl;
	char arr[SIZE];

  	int size_1=data.size_z;
  	//cout<<"size is "<<size_1<<endl;

  	pthread_t server;
	int cli;

  	//cout<<size_1<<endl;
  	int no_of_threads=ceil(size_1*1.0/BUFF_SIZE);
  	//cout<<"No of threads"<<no_of_threads<<endl;
  	//cout<<data.i<<endl;
  	for(long i=0;i<no_of_threads;i++)
  	{
  		int file_num=i%data.i;
  		char file_name[SIZE];
		int j;
		for(j=0;j<data.paths[file_num].size();j++)
		{
			file_name[j]=data.paths[file_num][j];
		}
		file_name[j]='\0';
		cout<<"chunck number "<<i+1<<" from "<<file_name<<endl;
		int f_read = open(file_name, O_RDONLY);
		int ret=lseek(f_read,sizeof(char)*BUFF_SIZE*i,SEEK_SET);
		char todo[BUFF_SIZE];
		int ret1=read(f_read, todo, sizeof(todo));
		close(f_read); 
		//cout<<data.dpath<<"data path 123"<<endl;
		if(i == 0)
		{
			int ot=open(data.dpath,O_CREAT,S_IRWXU);
			close(ot);
		}
		int outfile=open(data.dpath,O_APPEND|O_RDWR,S_IRWXU);
		write(outfile,todo,ret1);
		close(outfile);
  	}
	
}
void *serFunc(void *port)
{
	long p;
	p=(long)port;

	int server_fd,sock_fd;
	server_fd = socket (AF_INET, SOCK_STREAM, 0);
	if(server_fd == 0)
	{
		cout<<"Error 1"<<endl;
	}

	struct sockaddr_in   addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(p);
    addr.sin_addr.s_addr=inet_addr("127.0.0.1");
    int addrlen = sizeof(sockaddr);
    
    if(bind (server_fd,(struct sockaddr *)&addr,sizeof(addr))<0)
    {
    	cout<<"Error 2"<<endl;
    }
    
	if(listen (server_fd, 3)<0)
	{
		cout<<"Error 3"<<endl;
	}
	
	while(1)
	{
		sock_fd = accept( server_fd , (struct sockaddr *)&addr , (socklen_t*)&addrlen);
		int c,i;
		data.paths.clear();
		recv(sock_fd,&c,sizeof(c),0);
		data.i=c;
		char path[SIZE];
		//cout<<endl;
		//cout<<"*******************************"<<endl;
		for(int i=0;i<c;i++)
		{
			recv(sock_fd,path,sizeof(path),0);
			data.paths.push_back(path);
			//cout<<path<<endl;
		}
		//cout<<"*******************************"<<endl;
		recv(sock_fd,path,sizeof(path),0);
		//cout<<path<<"path is"<<endl;
		for(i=0;i<sizeof(path);i++)
		{
			data.dpath[i]=path[i];
		}
		data.dpath[i]='\0';	
		//cout<<data.dpath<<"data path"<<endl;
		int siz;
		recv(sock_fd,&siz,sizeof(siz),0);
		data.size_z=siz;
		do_work(c);
	}
	close(sock_fd);
    close(server_fd);
}
int main(int argc, char **argv)
{

	long port=(int)strtoul(argv[1], NULL, 0);
	
	pthread_t client,server;
	int cli;
	cli = pthread_create(&client, NULL , cliFunc,(void *)port);
	if(cli)
	{
		cout<<"Error in creating client thread"<<endl;
	}
	if(pthread_detach(client))
	{
		cout<<"Error detach client";
	}
	
	int ser;
	ser = pthread_create(&server, NULL, serFunc, (void *)port);
	if(ser)
	{
		cout<<"Error in create server thread"<<endl;
	}
	if(pthread_detach(server))
	{
		cout<<"Error detach server";
	}
	pthread_exit(NULL);
	return 0;
}
