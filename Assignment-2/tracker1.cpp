#include <iostream> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#include <pthread.h>
#include <vector>
#include <map>
#include "sha1.h"
#define BUFF_SIZE 524288
#define PORT 8080 
#define NUM_THREADS 10
#define SIZE 200
#define S1 1000 //size of sha1
using namespace std;
struct user
{
    int u_id;
    char user_name[SIZE];
    char password[SIZE];
    long port;
};
struct group
{
    int u_id;
    char group_name[SIZE];
    char owner_name[SIZE];
    vector<int> pending;
    vector<int> users;
    map<string,string> files;
};
struct files
{
    int u_id;
    char sha1[S1];
    char group_name[SIZE];
    char path[SIZE];
    long group_port;
    int size;
    
};
map<int, user> user_data;
map<int, group> group_data;
map<int, files> file_data;
int check(char cmd[SIZE])
{
    char strg[SIZE];
    int i;
    for(i=0;i<strlen(cmd);i++)
    {
        strg[i]=cmd[i];
        if(strg[i]==' ')
        {
            strg[i]='\0';
            break;
        }
    }
    strg[i]='\0';
    //cout<<strg<<endl;
    if(strcmp(strg,"create_user")==0)
    {
        return 1;
    }
    else if(strcmp(strg,"login")==0)
    {
        return 2;
    }
    else if(strcmp(strg,"logout")==0)
    {
        return 3;
    }
    else if(strcmp(strg,"create_group")==0)
    {
        return 4;
    }   
    else if(strcmp(strg,"list_groups")==0)
    {
        return 5;
    }
    else if(strcmp(strg,"join_group")==0)
    {
        return 6;
    }
    else if(strcmp(strg,"list_request")==0)
    {   
        return 7;
    }
    else if(strcmp(strg,"accept_request")==0)
    {
        return 8;
    }
    else if(strcmp(strg,"leave_group")==0)
    {
        //cout<<"strg "<<strg<<endl;
        return 9;
    }
    else if(strcmp(strg,"upload_file")==0)
    {
        return 10;
    }
    else if(strcmp(strg,"list_files")==0)
    {
        return 11;
    }
    else if(strcmp(strg,"download_file")==0)
    {  
        return 12;
    }
}
void prnt_user_data()
{
    map<int, user>::iterator itr;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
        cout <<" "<< itr->first <<" "<< itr->second.user_name<<" "<<itr->second.password<<" "<<itr->second.port<<" "<<itr->second.u_id<<endl; 
    }
}
int user_data_isthere( char name[SIZE],char passd[SIZE], long port)
{
    int temp;
    map<int, user>::iterator itr;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr)
    {
        temp=0;
        for(int p=0;p<strlen(itr->second.user_name);p++)
        {
            if(itr->second.user_name[p] != name[p])
            {
                temp=1;
                break;
            }
        }
        if(temp == 0)
        {   
            if(itr->second.port == -1)
            {
                temp=0;
                for(int p=0; p<strlen(itr->second.password);p++)
                {
                    if(itr->second.password[p] != passd[p])
                    {
                        temp=1;
                        break;
                    }
                }
                if(temp == 0)
                {
                    itr->second.port = port;
                    return 2;
                }
                else
                {
                    return 4;
                }
            }
            else
            {
                return 3;
            }
        }
    }
    return 0;
}
void create_user(char cmd[SIZE] , long port )
{
    struct user value;
    char name[SIZE];
    char passd[SIZE];
    int temp=0,e=0,f=0;
    int u_id;
    for(int z=12;z<strlen(cmd);z++)
    {
        if(cmd[z] == ' ')
        {
            temp=1;
        }
        else
        {
            if(temp == 0)
            {
                name[e]=cmd[z];
                e++;
            }
            else if(temp == 1 && cmd[z]!=' ')
            {
                passd[f]=cmd[z];
                f++;
            }
        }
    }
    name[e]='\0';
    passd[f]='\0';
    if(user_data_isthere(name,passd,port) == 0)
    {
        u_id = rand()%100;
        int o;
        for(o=0;o<strlen(name);o++)
        {
            value.user_name[o] = name[o];
        }
        value.user_name[o]='\0';
        for(o=0;o<strlen(passd);o++)
        {
            value.password[o] = passd[o];
        }
        value.password[o]='\0';
        value.port = -1;
        value.u_id = u_id;
        user_data[u_id] = value;
    }
    else
    {
        cout<<"Username already exist.. try other user name"<<endl;
    }
}
int check_port(long port)
{
    map<int, user>::iterator itr;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                return 1;
            }
        }
        return 0;
}
void login_user(char cmd[SIZE], long port)
{
    char name[SIZE];
    char passd[SIZE];
    
    int u_id;
    int e=0,f=0,o;
    int temp=0;
    for(int z=6;z<strlen(cmd);z++)
    {
        if(cmd[z] == ' ')
        {
            temp=1;
        }
        else
        {
            if(temp == 0)
            {
                name[e]=cmd[z];
                e++;
            }
            else if(temp == 1 && cmd[z]!=' ')
            {
                passd[f]=cmd[z];
                f++;
            }
        }
    }
    name[e]='\0';
    passd[f]='\0';
    int res1=check_port(port);
    if(res1 == 1)
    {
        cout<<"ALready a client logged in... logout it first"<<endl;
    }
    else
    {
    int res=user_data_isthere(name,passd,port);
    if(res == 0)
    {
        cout<<"Enter correct username and password"<<endl;
    }
    else if(res == 2)
    {
        cout<<"Login succesfull"<<endl; 
    }
    else if(res == 3)
    {
        cout<<"Currently running somewhere please logout there"<<endl;
    }
    else if(res == 4)
    {
        cout<<"Enter correct password"<<endl;
    }
    }
}
void logout_user(char cmd[SIZE],long port)
{
    map<int, user>::iterator itr;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
        if(itr->second.port == port)
        {
            itr->second.port = -1;
        }
    }
}
void create_group(char cmd[SIZE], long port)
{
    struct group value;
    
    int u_id = rand()%SIZE;
    
    int e=0;
    for(int z=13; z<strlen(cmd); z++)
    {
        value.group_name[e]=cmd[z];
        e++;
    }
    value.group_name[e]='\0';
    int p=0,temp=0;
    map<int, user>::iterator itr;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                for( p=0;p<strlen(itr->second.user_name); p++)
                {
                    value.owner_name[p] = itr->second.user_name[p];
                }
                value.owner_name[p] = '\0';
                
                temp=1;
                break;
            }
        }
        if(temp == 0)
        {
            cout<<"Login first"<<endl;
        }
        else
        {
            int u_id = rand()%SIZE;
        
            cout<<"Group created"<<endl;
            cout<<value.group_name<<" "<<value.owner_name<<endl;
            value.u_id = u_id;
            group_data[u_id] = value;
        }
}
void list_groups(char cmd[SIZE], long port)
{
    map<int, group>::iterator itr;
    for(itr = group_data.begin(); itr != group_data.end(); ++itr) 
    { 
            cout<< itr->second.group_name<<" "<<itr->second.owner_name<<" "<< itr->second.u_id <<endl; 
            cout<<"Users"<<endl;
            for(auto i=0;i<itr->second.users.size();i++)
            {
                if(itr->second.users[i] != -1)
                {
                    cout<<itr->second.users[i]<<" ";
                }
            }
            cout<<endl;
            cout<<"requests"<<endl;
            for(auto i=0;i<itr->second.pending.size();i++)
            {
                if(itr->second.pending[i] != -1)
                {   
                    cout<<itr->second.pending[i]<<" ";
                }
            }
            cout<<endl;
            cout<<"map size "<<itr->second.files.size()<<endl;
            map<string, string>::iterator it;
            for(auto it=itr->second.files.begin(); it != itr->second.files.end(); it++)
            {
                cout<<it->first<<" "<<it->second<<endl;
            }
    }
}
void join_groups(char cmd[SIZE], long port)
{
    map<int, user>::iterator itr;
    map<int, group>::iterator itr1;
    int temp=0;
    int p=0,l;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
        if(itr->second.port == port)
        {
            temp=1;
            break;
        }
    }
    if(temp == 0)
    {
        cout<<"Login and join"<<endl;
    }
    else
    {
        char name[SIZE];
        char owner[SIZE];
        int user_id;
        int k=0;
        for(p=11; p<strlen(cmd);p++)
        {
            name[k]=cmd[p];
            k++;
        }
        name[k]='\0';
        temp=0;
        for(itr1 = group_data.begin() ; itr1 != group_data.end(); ++itr1)
        {
            temp=0;
            for(l=0;l<strlen(itr1->second.group_name);l++)
            {
                if(itr1->second.group_name[l] != name[l])
                {
                    temp=1;
                    break;
                }
            }
            if(temp == 0)
            {
                temp=0;
                for(itr = user_data.begin(); itr != user_data.end(); ++itr)
                {
                    if(itr->second.port == port)
                    {
                        temp=1;
                        break;
                    }
                }
                if(temp == 1)
                {
                    int ins = itr->second.u_id;
                    temp=0;
                    for(auto q=0;q<itr1->second.pending.size();q++)
                    {
                        if(itr1->second.pending[q] == ins)
                        {
                            temp=1;
                            break;
                        }
                    }
                    if(temp == 1)
                    {
                        cout<<"Already requests"<<endl;
                    }
                    else
                    {
                        itr1->second.pending.push_back(itr->second.u_id);
                    }
                }
                break;
            }
        }
    }
}
void list_requests(char cmd[SIZE], long port)
{
    map<int, user>::iterator itr;
    map<int, group>::iterator itr1;
    char gname[SIZE];
    int p=0,q=0,temp=0;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
        if(itr->second.port == port)
        {
            temp=1;
            break;
        }
    }
    if(temp == 0)
    {
        cout<<"Login and request"<<endl;
    }
    else
    {
        for(p=13;p<strlen(cmd);p++)
        {
            gname[q] = cmd[p];
            q++;
        }
        gname[q]='\0';
        for(itr1 = group_data.begin() ; itr1 != group_data.end(); ++itr1)
        {
            temp=0;
            for(p=0;p<strlen(itr1->second.group_name);p++)
            {
                if(itr1->second.group_name[p] != gname[p])
                {   
                    temp=1;
                    break;
                }
            }
            if(temp == 0)
            {
                for(q=0;q<itr1->second.pending.size();q++)
                {
                    //cout<<itr1->second.pending[q]<<" ";
                    map<int, user>::iterator itr2;
                    for(itr2 = user_data.begin(); itr2 != user_data.end();++itr2)
                    {
                        if(itr2->second.u_id == itr1->second.pending[q]) 
                        {
                            cout<<itr2->second.user_name<<endl;
                        }
                    }
                }
            }
        }
        cout<<endl;
    }
}
void accept_request (char cmd[SIZE], long port)
{
    map<int, user>::iterator itr;
    map<int, group>::iterator itr1;
    
    int temp=0,p,q=0,r=0;
    char gname[SIZE], uname[SIZE],name[SIZE];
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                temp=1;
                break;
            }
        }
        if(temp == 0)
        {
            cout<<"Login and request"<<endl;
        }
        else
        {
            temp=0;
            for(p=15;p<strlen(cmd);p++)
            {
                if(cmd[p] == ' ')
                {
                    temp=1;
                }
                else
                {
                    if(temp == 0)
                    {
                        gname[q]=cmd[p];
                        q++;
                    }
                    else if(temp == 1 && cmd[p] != ' ')
                    {
                    uname[r]=cmd[p];
                    r++;                
                    }
                }
            }
            gname[q] = '\0';
            uname[r] = '\0';
            cout<<gname<<" "<<uname<<endl;
            int temp1=0,port_num,u_id_num;
            for(itr1 = group_data.begin() ; itr1 != group_data.end(); ++itr1)
            {
                temp=0;
                //cout<<itr1->second.group_name<<endl;
                for(q=0;q<strlen(itr1->second.group_name);q++)
                {
                    if(itr1->second.group_name[q] != gname[q])
                    {
                        temp=1;
                        break;
                    }
                }
                if(temp == 0)
                {
                    temp1=1;
                    temp=0;
                    for(r=0;r<strlen(itr1->second.owner_name);r++)
                    {
                        name[r]=itr1->second.owner_name[r];
                    }
                    name[r]='\0';
                    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
                    {
                        temp=0;
                        for(p=0;p<strlen(itr->second.user_name);p++)
                        {
                            if(itr->second.user_name[p] != name[p])
                            {
                                temp=1;
                                break;
                            }
                        }
                        if(temp == 0)
                        {
                            port_num = itr->second.port;
                            break;
                        }
                    }
                    //cout<<name<<" "<<port_num<<endl;
                    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
                    {
                        temp=0;
                        for(p=0;p<strlen(itr->second.user_name);p++)
                        {
                            if(itr->second.user_name[p] != uname[p])
                            {
                                temp=1;
                                break;
                            }
                        }
                        if(temp == 0)
                        {
                            u_id_num = itr->second.u_id;
                        }
                    }   
                    //cout<<"u_id_num "<<u_id_num<<endl;
                    if(port_num != port)    
                    {
                        cout<<"You are not the owner"<<endl;
                    }
                    else
                    {
                        vector<int>::iterator it;
                        temp=0;
                        for(it = itr1->second.pending.begin() ; it < itr1->second.pending.end(); ++it)
                        {
                            temp=0;
                            if((*it) == u_id_num)
                            {
                                temp=1;
                                itr1->second.pending.erase(it);
                                itr1->second.users.push_back(u_id_num);
                                break;
                            }
                        }
                        if(temp == 0)
                        {
                            cout<<"Request not made "<<endl;
                        }
                    }
                }
            }
            if(temp1 == 0)
            {
                cout<<"Enter correct group name in accept_request"<<endl;
            }
        }
}
void leave_group(char cmd[SIZE], long port)
{
    map<int, user>::iterator itr;
    int temp,p,r;
    char gname[SIZE];
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                temp=1;
                break;
            }
        }
        if(temp == 0)
        {
            cout<<"Login and request"<<endl;
        }
        else
        {
            r=0;
            for(p=12;p<strlen(cmd);p++)
            {
                gname[r] = cmd[p];
                r++;
            }
            gname[r] = '\0';
            map<int, group>::iterator itr1;
            for(itr1 = group_data.begin(); itr1 != group_data.end(); ++itr1)
            {
                temp=0;
                for(p=0;p<strlen(itr1->second.group_name);p++)
                {
                    if(itr1->second.group_name[p] != gname[p])
                    {
                        temp=1;
                        break;
                    }
                }
                if(temp == 0)
                {
                    
                    for(auto l=0;l<itr1->second.pending.size();l++)
                    {
                        if(itr1->second.pending[l]==itr->second.u_id)
                        {
                            itr1->second.pending[l]=-1;
                        }
                    }
                    for(auto l=0;l<itr1->second.users.size();l++)
                    {
                        if(itr1->second.users[l]==itr->second.u_id)
                        {
                            itr1->second.users[l]=-1;
                        }
                    }
                    break;
                }
            }
        }
}   
void upload_file(char cmd[SIZE], long port)
{
    char path[SIZE], gname[SIZE], name[SIZE];
    string fname="";
    map<int, user>::iterator itr;
    int temp=0;
    int p=0,r=0,q=0,id;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                temp=1;
                id=itr->second.u_id;
                break;
            }
        }
        if(temp == 0)
        {
            cout<<"Login and request"<<endl;
        }
        else
        {   temp=0;
            for(p=12;p<strlen(cmd);p++)
            {
                if(cmd[p] == ' ')
                {
                    temp=1;
                }
                else
                {
                    if(temp == 0)
                    {   
                        path[r] = cmd[p];
                        r++;
                    }
                    else
                    {
                        gname[q] = cmd[p];
                        q++;
                    }
                }
            }
            path[r] = '\0';
            gname[q]= '\0';
            int z;
            for(z=0;z<strlen(path);z++)
            {
                fname=fname+path[z];
            }
            fname[z]='\0';
            //cout<<fname<<endl;
            map<int, group>::iterator itr1;
            int temp1=0,port_num,n;
            int u_id;
            for(itr1 = group_data.begin(); itr1 != group_data.end(); ++itr1)
            {
                temp=0;
                for(p=0;p<strlen(itr1->second.group_name);p++)
                {
                    if(itr1->second.group_name[p] != gname[p])
                    {
                        temp=1;
                        break;
                    }
                }
                if(temp == 0)
                {
                    for(p=0;p<strlen(itr1->second.owner_name);p++)
                    {
                        name[p] = itr1->second.owner_name[p];
                    }
                    name[p]='\0';
                    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
                    {
                        temp=0;
                        for(p=0;p<strlen(itr->second.user_name); p++)
                        {
                            if(itr->second.user_name[p] != name[p])
                            {
                                temp=1;
                            }
                        }
                        if(temp == 0)
                        {
                            port_num = itr->second.port;
                            break;
                        }
                    }
                    if(port_num != port)
                    {
                        cout<<"You are not owner"<<endl;
                    }
                    else
                    {
                        cout<<"You are owner... you can upload the file"<<endl;
                        struct files value;
                        FILE *fp = fopen(path, "rb" );
                        if(fp == NULL)
                        {
                            cout<<"Error in opening file"<<endl;
                        }
                        else
                        {
                            fseek(fp,0,SEEK_END);
                            int size_1 = ftell ( fp );
                            int size=size_1;
                            rewind(fp);
                        
                            char Buffer[BUFF_SIZE]; 
                            string sh,sh1;
                            while((n=fread(Buffer,sizeof(char),BUFF_SIZE,fp))>0 && size_1>0)
                            {
                                Buffer[BUFF_SIZE]='\0';
                                sh=sha1(Buffer);
                                sh1.append(sh,20);
                                memset ( Buffer,'\0', BUFF_SIZE);
                                size_1 = size_1 - n ;
                            }
                            itr1->second.files[fname] = sh1;
                            copy(sh1.begin(),sh1.end(),value.sha1);

                            value.sha1[sh1.size()]='\0';
                            for(p=0;p<strlen(gname);p++)
                            {
                                value.group_name[p] = gname[p];
                            }
                            value.group_name[p]='\0';
                        
                            for(p=0;p<strlen(path);p++)
                            {
                                value.path[p] = path[p];
                            }
                            value.path[p] = '\0';
                            value.group_port = port;
                            value.u_id = id;
                            value.size=size_1;
                            u_id = rand()%SIZE;
                            file_data[u_id] = value;

                        }
                    }
                    temp1=1;
                    break;
                }
            }
            if(temp1 == 0)
            {
                cout<<"group do not exist"<<endl;
            }
        }
}
void list_files(char cmd[SIZE], long port)
{
    map<int, user>::iterator itr;
    map<int, group>::iterator itr1;
    int p,r=0,z;
    char gname[SIZE];
    int ele,temp;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                temp=1;
                ele=itr->second.u_id;
                break;
            }
    }
    if(temp == 0)
    {
        cout<<"Login and request"<<endl;
    }
    else
    {
        for(p=11;p<strlen(cmd);p++)
        {
            gname[r] = cmd[p];
            r++;
        }
        gname[r] = '\0';
       for(itr1 = group_data.begin(); itr1 != group_data.end(); ++itr1)
       {
            int t=0;
            for(z=0;z<strlen(itr1->second.group_name);z++)
            {
                if(itr1->second.group_name[z] != gname[z])
                {
                    t=1;
                    break;
                }
            }
            if(t == 0)
            {
                map<string, string>::iterator mit;
                for(auto mit=itr1->second.files.begin(); mit != itr1->second.files.end(); mit++)
                {
                    cout<<mit->first<<" "<<mit->second<<endl;
                }
                break;
            }
       }
    }
}
void download_file(char cmd[SIZE], long port,long sock_fd)
{
    map<int, user>::iterator itr;
    map<int, group>::iterator itr1;
    map<int, files>::iterator itr2;
    string sh;
    string fname="";
    int temp=0,temp1=0,id;
    int p=0,x=0,y=0,z=0,r=0;
    char gname[SIZE], dpath[SIZE], spath[SIZE];
    for(itr = user_data.begin(); itr != user_data.end(); ++itr) 
    { 
            if(itr->second.port == port)
            {
                temp=1;
                id = itr->second.u_id;
                break;
            }
    }
    if(temp == 0)
    {
        cout<<"Login and request"<<endl;
    }
    else
    {   temp=0,temp1=0;
        for(p=14;p<strlen(cmd);p++)
        {
            if(cmd[p] == ' ')
            {
                if(temp == 1)
                {
                    temp1=1;
                }
                temp=1;
            }
            else
            {
                if(temp == 0 && temp1 == 0)
                {
                    gname[x] = cmd[p];
                    x++;
                }
                else if(temp == 1 && temp1 == 0)
                {
                    spath[y] = cmd[p];
                    fname=fname+cmd[p];
                    y++;
                }
                else if(temp == 1 && temp1 == 1)
                {
                    dpath[z] = cmd[p];
                    z++;
                }
            }
        }
        gname[x] = '\0';
        spath[y] = '\0';
        dpath[z] = '\0';
        fname[y] = '\0';
        int size_1;
        FILE *fp = fopen (spath, "r" );
        fseek(fp,0,SEEK_END);
        size_1 = ftell ( fp );
        rewind(fp);
        struct files value;
        int z;
        int t;
        for(itr1 = group_data.begin(); itr1 != group_data.end(); ++itr1)
        {
            t=0;
            for( z=0;z<strlen(itr1->second.group_name);z++)
            {
                if(gname[z]!=itr1->second.group_name[z])
                {
                    t=1;
                    break;
                }
            } 
            if(t == 0)
            {
                int t2=0;
                for(auto m=0;m<itr1->second.users.size();m++)
                {
                    if(itr1->second.users[m] == id)
                    {
                        t2=1;
                        break;
                    }
                }
                if(t2 == 0)
                {
                    cout<<"Join group and download files"<<endl;
                    return;
                }
                sh=itr1->second.files[fname];
                //cout<<sh<<endl;
                int t1,k;
                long file_count=0;
                for(itr2 = file_data.begin(); itr2 != file_data.end(); ++itr2)
                {
                    //cout<<itr2->second.sha1<<" "<<itr2->second.path<<endl;
                    t1=0;
                    for(k=0;k<strlen(itr2->second.sha1);k++)
                    {
                        if(itr2->second.sha1[k] != sh[k])
                        {
                            t1=1;
                            break;
                        }
                    }
                    if(t1 == 0)
                    {
                        file_count++;
                    }
                }
                if(file_count == 0)
                {
                    cout<<"Enter correct file name "<<endl;
                    return;
                }
                //cout<<"File is present "<<file_count<<" times"<<endl;
                send(sock_fd,&port,sizeof(port),0);
                send(sock_fd,&file_count, sizeof(file_count),0);
                send(sock_fd,dpath,sizeof(dpath),0);
                send(sock_fd,&size_1,sizeof(size_1),0);

                file_count=0;
                for(itr2 = file_data.begin(); itr2 != file_data.end(); ++itr2)
                {
                    //cout<<itr2->second.sha1<<" "<<itr2->second.path<<endl;
                    t1=0;
                    for(k=0;k<strlen(itr2->second.sha1);k++)
                    {
                        if(itr2->second.sha1[k] != sh[k])
                        {
                            t1=1;
                            break;
                        }
                    }
                    if(t1 == 0)
                    {
                        //cout<<itr2->second.path<<" "<<itr2->second.u_id<<endl;
                        send(sock_fd,itr2->second.path,sizeof(itr2->second.path),0);
                        int l;
                        value.u_id = id;
                        value.group_port = itr2->second.group_port;
                        for(l=0;l<strlen(itr2->second.sha1);l++)
                        {
                            value.sha1[l] = itr2->second.sha1[l];
                        }
                        value.sha1[l]='\0';
                        for(l=0;l<strlen(itr2->second.group_name);l++)
                        {
                            value.group_name[l] = itr2->second.group_name[l];
                        }
                        value.group_name[l]='\0';
                        for(l=0;l<strlen(dpath);l++)
                        {
                            value.path[l]=dpath[l];
                        }
                        value.path[l]='\0';
                        //cout<<value.u_id<<" "<<value.group_port<<" "<<value.sha1<<" "<<value.group_name<<" "<<value.path<<endl;
                    }
                }
                //Write condition for download successful
                //cout<<value.u_id<<" "<<value.group_port<<" "<<value.sha1<<" "<<value.group_name<<" "<<value.path<<endl;
                file_data[rand()%SIZE] = value;
                break;
            }
        }
    }
}
void remove_all(long port)
{
    map<int, user>::iterator itr;
    for(itr = user_data.begin(); itr != user_data.end(); ++itr)
    {
        if(itr->second.port == port)
        {
            break;
        }
    }
    char a[100];
    int i;
    for(i=0;i<strlen(itr->second.user_name);i++)
    {
        a[i]=itr->second.user_name[i];
    }
    a[i]='\0';
    int id=itr->second.u_id;
    int ele=itr->first;
    user_data.erase(ele);
    int temp;
    map<int, group>::iterator itr1;
    for(itr1 = group_data.begin();itr1 != group_data.end();itr1++)
    {
        temp=0;
        for(i=0;i<strlen(itr1->second.owner_name);i++)
        {
            if(a[i] != itr1->second.owner_name[i])
            {
                temp=1;
                break;
            }
        }
        if(temp == 0)
        {
            ele=itr1->first;
            group_data.erase(ele);
        }
        else
        {
            for(auto i=0;i<itr1->second.pending.size();i++)
            {
                if(itr1->second.pending[i]==id)
                {
                    itr1->second.pending[i]=-1;
                }
            }
            for(auto i=0;i<itr1->second.users.size();i++)
            {
                if(itr1->second.users[i]==id)
                {
                    itr1->second.users[i]=-1;
                }
            }
        }
    }

}
void *funcExe(void *sock)
{
    long sock_fd;
    sock_fd=(long)sock;
    long m; //m is port
    recv(sock_fd,&m,sizeof(m),0);
    //cout<<"port "<<m<<endl;
    char cmd[100];
    int l;
    while(1)
    {
        recv(sock_fd, &cmd, sizeof(cmd),0);
        cout<<cmd<<endl;
        if(cmd[0] == 'e' && cmd[1] == 'x' && cmd[2] == 'i' && cmd[3] == 't')
        {
            remove_all(m);
            cout<<"*************User data***************"<<endl;
            prnt_user_data();   
            cout<<"*************************************"<<endl;
            cout<<"*************************************"<<endl;
            list_groups(cmd,m);
            cout<<"*************************************"<<endl;
            break;
        }   
        int res=check(cmd);
        //cout<<res<<endl;
        if(res == 1)
            create_user(cmd,m);
        else if(res == 2)
            login_user(cmd,m);
        else if(res == 3)
            logout_user(cmd,m);
        else if(res == 4)
            create_group(cmd,m);
        else if(res == 5)
            list_groups(cmd,m);
        else if(res == 6)
            join_groups(cmd,m);
        else if(res == 7)
            list_requests(cmd,m);
        else if(res == 8)
            accept_request(cmd,m);
        else if(res == 9)
            leave_group(cmd,m);
        else if(res == 10)
            upload_file(cmd,m);
        else if(res == 11)
            list_files(cmd,m);
        else if(res == 12)
            download_file(cmd,m,sock_fd);
        memset(&cmd[0], 0, sizeof(cmd));
        cout<<"*************User data***************"<<endl;
        prnt_user_data();   
        cout<<"*************************************"<<endl;
        cout<<"*************************************"<<endl;
        list_groups(cmd,m);
        cout<<"*************************************"<<endl;
    }
}
int main()
{
    int server_fd,n;
    server_fd = socket (AF_INET, SOCK_STREAM, 0);
    if(server_fd == 0)
    {
        cout<<"Error 1"<<endl;
    }
    struct sockaddr_in   addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
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
    int z=0;
    pthread_t threads[NUM_THREADS];
    int rc;
    while(1)
    {
        long sock_fd = (long)accept( server_fd , (struct sockaddr *)&addr , (socklen_t*)&addrlen);
        if(sock_fd != -1)
        {
            rc = pthread_create(&threads[z], NULL, funcExe, (void *)sock_fd );
            if(rc)
            {
                cout<<"Error in creating threads"<<endl;
            }
            z++;
        }
    }
    pthread_exit(NULL);
    close(server_fd);
}
