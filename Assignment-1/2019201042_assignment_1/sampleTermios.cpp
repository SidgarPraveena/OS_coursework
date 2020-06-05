#include<stdio.h>
#include<termios.h>
#include<sys/types.h>
#include<unistd.h>
#include<iostream>
#include<unordered_map>
#include<vector>
#include <bits/stdc++.h>
#include<sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include<string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/utsname.h>
#include<vector>
#include<map>
#include<fstream>
#include<signal.h>
using namespace std;
vector<char> store;
vector<string> vec;

struct Trie
{
	bool isEndOfWord;
	unordered_map<char, Trie *>map;
};
Trie* getNewTrieNode() 
{ 
    Trie* node = new Trie; 
    node->isEndOfWord = false; 
    return node; 
} 
void insert(Trie**  root, const string& str) 
{ 
    if (*root == nullptr) 
        *root = getNewTrieNode(); 
  
    Trie* temp = *root; 
    for (int i = 0; i < str.length(); i++) { 
        char x = str[i]; 
        if (temp->map.find(x) == temp->map.end()) 
            temp->map[x] = getNewTrieNode(); 
  
        temp = temp->map[x]; 
    } 
  
    temp->isEndOfWord = true; 
} 


void display(vector<char> store,char arr[5])
{
	cout<<arr;
	for(auto i=store.begin(); i!=store.end();i++)
	{
		cout<<*i;
	}
	cout<<endl;
}

void traverse(Trie** root,char arr[5])
{
	Trie * temp,*temp1;
	temp=(*root);
		
	if(temp == nullptr)
	{
		return;
	}
	else
	{
		if((temp->map.size())!=0)
		{
			for (auto it = temp->map.cbegin(); it != temp->map.cend(); ++it) 
			{
				char z=(*it).first;
				store.push_back(z);
				temp1=temp->map[z];
				if(temp1->isEndOfWord == true)
				{
					display(store,arr);
				}
				traverse(&(temp->map[z]),arr);
				store.pop_back();
			}
		}
	}
}
void init(Trie ** root)
{
	insert(root, "ls"); 
    	insert(root, "ln"); 
	insert(root, "kill"); 
	insert(root, "dir"); 
    	insert(root,"dif");
    	insert(root, "cp"); 
    	insert(root,"ps");
}
struct Trie * search(char arr[5],Trie *root)
{
	Trie * temp=root;
	
	
	for(int i=0;i<strlen(arr);i++)
	{
		if(temp->map.find(arr[i])==temp->map.end())
		{
			return nullptr;
		}
		else
		{
			temp=temp->map[arr[i]];
		}
	}
	if(temp->isEndOfWord==true)
	{
		cout<<arr<<"\t";
	}
	return temp;
}

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( 0, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON );
    tcsetattr( 0, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( 0, TCSANOW, &oldattr );
    return ch;
}
void parse(char *arr, char ** argv)
{
	int i=0;
	char * tok;
	char *array[500];
	tok=strtok(arr," ");
	while(tok!=NULL)
	{
		argv[i++]=strdup(tok);
		tok=strtok(NULL," ");
		argv[i]=NULL;
	}
}	
void pipTok(char * arr, char **argv)
{
	int i=0;
	char * tok;
	char *array[500];
	tok=strtok(arr,"|");
	while(tok!=NULL)
	{
		array[i++]=strdup(tok);
		tok=strtok(NULL,"|");
	}
	for(int j=0;j<i;j++)
	{
		argv[j]=array[j];
		argv[i]=NULL;
	}
	/*for(int j=0;j<i;j++)
	{
		cout<<"j "<<argv[j]<<" ";
	}*/
}
void sinRed(char * arr, char ** argv)
{
	int i=0;
	char * tok;
	char *array[500];
	tok=strtok(arr,">");
	while(tok!=NULL)
	{
		array[i++]=strdup(tok);
		tok=strtok(NULL,">");
	}
	for(int j=0;j<i;j++)
	{
		argv[j]=array[j];
		argv[i]=NULL;
	}
}
void dobRed(char * arr,char ** argv)
{
	int i=0;
	char * tok;
	char *array[500];
	tok=strtok(arr,">>");
	while(tok!=NULL)
	{
		array[i++]=strdup(tok);
		tok=strtok(NULL,">>");
	}
	for(int j=0;j<i;j++)
	{
		argv[j]=array[j];
		argv[i]=NULL;
	}
}
int checkPipe(char * arr)
{
	int count=0;
	for(int i=0;i<strlen(arr);i++)
	{
		if(arr[i]=='|')
			count++;
	}
	return count;
}
int check(char *arr)
{
	//cout<<"Hello"<<arr<<endl;
	if(arr[0]=='$' && arr[1]=='\0')
		return 6;
	if(arr[0]=='a' && arr[1]=='l' && arr[2]=='i' && arr[3]=='a' && arr[4]=='s')
		return 5;
	if(arr[0]=='c' && arr[1]=='d' && arr[2]==' ' && arr[3]=='~' && arr[4]==' ')
		return 4;
	if(arr[0]=='c' && arr[1]=='d' && arr[2]==' ' && arr[3]=='~' && arr[4]!=' ')
		return 7;
	if(arr[0]=='c' && arr[1]=='d')
		return 3;
	if(arr[0]=='e' && arr[1]=='c' && arr[2]=='h' && arr[3]=='o' && arr[4]==' ' && arr[5]=='$'&&arr[6]=='P' && arr[7]=='A' && arr[8]=='T' && arr[9]=='H' && arr[10]=='\0')
		return 8;
	if(arr[0]=='e' && arr[1]=='c' && arr[2]=='h' && arr[3]=='o' && arr[4]==' ' && arr[5]=='$'&&arr[6]=='H' && arr[7]=='O' && arr[8]=='M' && arr[9]=='E' && arr[10]=='\0')
		return 9;
	if(arr[0]=='e' && arr[1]=='c' && arr[2]=='h' && arr[3]=='o' && arr[4]==' ' && arr[5]=='$'&&arr[6]=='U' && arr[7]=='S' && arr[8]=='E' && arr[9]=='R' && arr[10]=='\0')
		return 10;
	if(arr[0]=='e' && arr[1]=='c' && arr[2]=='h' && arr[3]=='o' && arr[4]==' ' && arr[5]=='$'&&arr[6]=='H' && arr[7]=='O' && arr[8]=='S' && arr[9]=='T')
		return 11;
	if(arr[0]=='e' && arr[1]=='x' && arr[2]=='i' && arr[3]=='t' )
		exit(0);
	
	for(int i=0;i<strlen(arr);i++)
	{
		if(arr[i]=='>')
		{
			if(arr[i+1]=='>')
			{
				return 2;
			}
			else 
			{
				return 1;
			}
		}
	}
	return 0;
}


void trim(char arr[500],int cas)
{
	int k,temp=0;
	if(cas==1)
	{
		for(int i=0;i<strlen(arr);i++)
		{
			if(arr[i]==' ' && arr[i+1]!='\0')
			{
				k=i+1;
				temp=1;
			}
		}
		if(temp==1)
		{
			int j=0;
			for(k;k<strlen(arr);k++)
			{
				arr[j]=arr[k];
				j++;
			}
			arr[j]='\0';
		}
	}
	else if(cas==2)
	{
		temp=0;
		for(int i=0;i<strlen(arr);i++)
		{
			if(arr[i]=='/')
			{
				temp=1;
				k=i+1;
			}
		}
		if(temp==1)
		{
			int j=0;
			for(k;k<strlen(arr);k++)
			{
				arr[j]=arr[k];
				j++;
			}
			arr[j]='\0';
		}
	}
}
void handleSignal(int signal_no)
{
	signal(SIGINT, handleSignal);
	return;
}
int main()
{

	int c,res,res1,d;
	char temp;
	char arr[500],b[500];
	char *argv[500],*argv1[500],*argv2[500],*argv3[500],*argv4[500],*argv5[500],*argv6[500],*argv7[500];
	map <vector <char>, vector <char> > al;
	vector<char> m;
	vector<char> n;
	ofstream outFile;
	outFile.open("/home/praveena/assignments/history.txt");
	pid_t pid;
	Trie* root = nullptr,*pt=nullptr;
	signal(SIGINT, handleSignal);
	while(1)
	{
		if(getuid()!=0)
			cout<<"$: ";
		else 
			cout<<"#: ";
		int i=0;
		while(1)
		{
			c=getch();
			//cout<<c;
			if(c==9)
				break;
			else if(c==10)
				break;
			else if(c==127)
			{
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				i=i-1;
				arr[i]='\0';
				
			}
			else if(((c==65 || c==66)&& arr[i-1]!='P')&& ((c==65 || c==66)&& arr[i-1]!='N'))
			{
				
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				cout<<"\b";
				cout<<" ";
				cout<<"\b";
				break;
			}
			else
			{
				temp=(char)c;
				arr[i]=temp;
				i++;
			}
		}
		arr[i]='\0';
		if(c==9)
		{
			root=nullptr;
			init(&root);
			
			char buff[FILENAME_MAX],*temp1;
  			getcwd(buff,FILENAME_MAX);
  			int len = strlen(buff);
			if(buff[len] != '/' && (opendir(buff) != NULL))
				strcat(buff, "/");
  			
  			struct dirent *ptr;
    			DIR *main;
    			main = opendir(buff);
    			if(main)
    			{
    				while (ptr = readdir(main))
    				{
    					insert(&root,ptr->d_name);
    				}
    			}
  			trim(arr,1);
			pt=search(arr,root);
			cout<<endl;
    			traverse(&pt,arr);
		}
		if(c==65 || c==66)
		{
			ifstream in;
			string line;
			in.open("/home/praveena/assignments/history.txt");
			while(!in.eof())
			{
				getline(in,line);
				vec.push_back(line);
			}
			int i,te;
			i=vec.size()-1;
			d=c;
			int sz,lg=4;
			do
			{	
				if(d==65)
				{
					for(int sh=0;sh<lg;sh++)
					{
						cout<<"\b";
						cout<<" ";
						cout<<"\b";
					}
					cout<<"$: "<<vec[i];
					lg=vec[i].size();
					lg=lg+7;
					sz=vec.size();
					i--;
					if(i<0)
						i++;
				}
				if(d==66)
				{
					for(int sh=0;sh<lg;sh++)
					{
						cout<<"\b";
						cout<<" ";
						cout<<"\b";
					}
					cout<<"$: "<<vec[i];
					lg=vec[i].size();
					lg=lg+7;
					sz=vec.size();
					i++;
					if(i==sz-1)
						i--;
				}
				if(d==127)
				{
					cout<<"\b";
					cout<<" ";
					cout<<"\b";
					cout<<"\b";
					cout<<" ";
					cout<<"\b";
					cout<<"\b";
					cout<<" ";
					cout<<"\b";
				}
				te=d;
				d=getch();
			}while(d!=10);
			c=d;
			int sh;
			if(te==65)
				i++;
			if(te==66)
				i--;
			for(sh=0;sh<vec[i].size();sh++)
			{
				arr[sh]=vec[i][sh];
			}
			arr[sh]='\0';
		}
		if(c==10)
		{
			outFile<<arr<<endl;//mandatory
			res=checkPipe(arr);
			
			if(res==0)
			{
				res1=check(arr);
				//cout<<"res 1 : "<<res1<<endl;
				if(res1==1)
				{
					sinRed(arr,argv2);
					parse(argv2[0],argv3);
					int fp;
					if(fork()==0)
					{
						
						fp=open(argv2[1],O_RDONLY | O_WRONLY | O_TRUNC );
						dup2(fp,1);
						int pos=execvp(argv3[0],argv3); //1
						close(fp);
						if(pos==-1)
							cout<<argv2[0]<<endl;  
						
						exit(0);
					}
					else
					{
						wait(NULL);
					}
				}
				else if(res1==2)
				{
					dobRed(arr,argv);
					parse(argv[0],argv1);
					int fp;
					
					if(fork()==0)
					{
						fp=open(argv[1],O_WRONLY | O_APPEND );
						if(fp==-1)
						{
							cout<<"Enter correct file name"<<endl;
						}
						else
						{
							dup2(fp,1);
							int pos=execvp(argv1[0],argv1);  //2
							close(fp);
							if(pos==-1)
								cout<<argv[0]<<endl;
						}
						
						
						exit(0);
					}
					else
					{
						wait(NULL);
					}
				}
				else if(res1==3)
				{
					trim(arr,1);
					chdir(arr);
				}
				else if(res1==4)
				{
					chdir("/home/praveena");

				}
				else if(res1==5)
				{
					
					int l=0,p;
					for(p=6; p<strlen(arr);p++)
					{
						if(arr[p]!='=')
						{
							m.push_back(arr[p]);
						}
						else 
						{
							break;
						}
					}
					p=p+1;
					l=0;
					for(p;p<strlen(arr);p++)
					{
						n.push_back(arr[p]);
					}
					al[m]=n;
					m.clear();
					n.clear();
				}
				else if(res1==6)
				{
					cout<<getpid()<<endl;	
				}
				else if(res1==7)
				{
					chdir("/home/praveena");
					int length=strlen(arr);
					trim(arr,2);
					chdir(arr);
					
				}
				else if(res1==8)
				{
					string line;
					ifstream inFile;
					inFile.open("/etc/environment");
					getline(inFile,line);
					for(int p=6;p<line.size();p++)
					{
						cout<<line[p];
					}
					cout<<endl;
				}
				else if(res1==9)
				{
					cout<<"/home/praveena"<<endl;
				}
				else if(res1==10)
				{
					cout<<"Praveena"<<endl;
				}
				else if(res1==11)
				{
					string line;
					ifstream inFile;
					inFile.open("/etc/hostname");
					getline(inFile,line);
					for(int p=6;p<line.size();p++)
					{
						cout<<line[p];
					}
					cout<<endl;
				}
				else
				{
					strcpy(b,arr);
					vector<char> zu,zp;
					map<vector <char>,vector<char>>::iterator it;
					for(int p=0;p<strlen(b);p++)
					{
						zu.push_back(b[p]);
					}
					it=al.find(zu);
					
					if (al.find(zu) != al.end()) 
            				{
            					zp=al[zu];
            					int p=0;
            					for (auto itz = zp.begin(); itz != zp.end(); itz++) 
            					{
        						b[p]=(*itz);
        						p++;
        					}
            				}
					parse(b, argv);
					if(fork()==0)
					{
						if((execvp(argv[0],argv))==-1)  //3
						{
							cout<<"Enter correct command"<<endl;
						}	
						exit(0);
					}
					else
					{
						wait(NULL);
					}
				}
			}
			else
			{
				
				/*pipTok(arr,argv4);
				int pipFd[2],pipFd1[2];
				pipe(pipFd);
				pipe(pipFd1);
				if(fork()==0)
				{
					
					close(pipFd1[1]);
					close(pipFd[1]);
					dup2(pipFd1[0],0);
					close(pipFd1[0]);
					parse(argv4[0],argv5);
					execvp(argv5[0],argv5);  //4
					exit(0);
				}
				else
				{
					if(fork()==0)
					{
						close(pipFd[1]);
						close(pipFd1[0]);
						dup2(pipFd[0],0);
						close(pipFd[0]);
						dup2(pipFd1[1],1);
						close(pipFd1[1]);
						//cout<<argv4[1];
						parse(argv4[1],argv6);
						execvp(argv6[0],argv6);  //4	
						exit(0);
					}
					else
					{
						//wait(NULL);
						//wait(NULL);
						if(fork()==0)
						{
							close(pipFd[0]);
							close(pipFd1[1]);
							dup2(pipFd[1],1);
							close(pipFd[1]);
							parse(argv4[2],argv7);
							execvp(argv7[0],argv7);
						}
					}
					
				}*/
				
			}
			
		}
	}
	
	return 0;
}
