#include<iostream>
#include <fstream>
#include <pthread.h>
#include <unistd.h>
#include <cmath>
#include<vector>
#include<stdio.h>
#include<sys/types.h>
#include<fcntl.h>
//#define BUFF_SIZE 200
#define BUFF_SIZE 524288
//#define BUFF_SIZE 5242880
#define SIZE 100
using namespace std;


struct thread_info
{
	vector<string> info;
	string dpath;
	int i;
};


FILE *fp1;
struct thread_info in;

int size_1;
void *oddFunc(void *arg)
{
	long index=(long)arg;

	//cout<<index<<" "<<in.i<<endl;
	int file_num=index%in.i;
	//cout<<file_num<<" "<<in.info[file_num]<<endl;

	char file_name[SIZE];
	int i;

	//cout<<in.info[file_num].size()<<endl;
	for(i=0;i<in.info[file_num].size();i++)
	{
		file_name[i]=in.info[file_num][i];
	}
	file_name[i]='\0';

	int f_read = open(file_name, O_RDONLY);
	//FILE *fp;
	//fp=fopen(file_name,"r");
	cout<<endl;
	int ret=lseek(f_read,sizeof(char)*BUFF_SIZE*index,SEEK_SET);
	cout<<ret<<" ";

	char todo[BUFF_SIZE];
	int ret1=read(f_read, todo, sizeof(todo));
	cout<<ret1<<endl;
	close(f_read); 

	char open_file[SIZE];
	for(i=0;i<in.dpath.size();i++)
	{
		open_file[i]=in.dpath[i];
	}
	open_file[i]='\0';
	if(index == 0)
	{
		int ot=open(open_file,O_CREAT,S_IRWXU);
		close(ot);
	}
	//cout<<todo<<endl;
	int outfile=open(open_file,O_APPEND|O_RDWR,S_IRWXU);
	cout<<write(outfile,todo,ret1);
	close(outfile);
}
int main()
{
	
	string p1="/home/praveena/important/stl's/refer/text";
	string p2="/home/praveena/important/stl's/text";
	string p3="/home/praveena/important/stl's/sample/text";

	in.info.push_back(p1);
	in.info.push_back(p2);
	in.info.push_back(p3);
	in.dpath="/home/praveena/important/stl's/refer/patch";
	in.i=3;

	FILE *fp = fopen ("/home/praveena/important/stl's/refer/text", "r" );
	fseek(fp,0,SEEK_END);
  	size_1 = ftell ( fp );
  	rewind(fp);
  	cout<<size_1<<endl;

	pthread_t server;
	int cli;
	long m=1,n=2;

  	int no_of_threads=ceil(size_1*1.0/BUFF_SIZE);
  	//cout<<no_of_threads<<endl;

  	for(long i=0;i<no_of_threads;i++)
  	{
  		cli = pthread_create(&server, NULL, oddFunc, (void *)i);
		if(cli)
		{
			cout<<"Error in creating client thread"<<endl;
		}
		//break;
		pthread_join(server,NULL);
		
  	}
  	pthread_exit(NULL);
	
	return 0;
}