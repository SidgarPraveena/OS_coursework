#!/usr/bin/env python3
import sys
import os
import os.path
import difflib
import hashlib
import time
import re
#for calculating md5hash
#taking command line arguments in python


#unix methods start
def add_the_difference(accu_file,diff_file):#arguments cointain the complete path
    cd='patch '+accu_file+' '+diff_file
    #print('we are executing the following command',cd)
    global q
    q=os.popen(cd)
    
    
    
def create_diff(old_file_name,new_file_name,diff_file_name):
    #arguments cointain the complete path
    cd='diff '+old_file_name+' '+new_file_name
    #print('we are executing the following command',cd)
    p=os.popen(cd)
    fp=open(diff_file_name,'w')
    for line in p.read():
        fp.write(line)
    fp.close()
#unix methods ends

def delete_instant(file_name):
    match_obj=re.match(r'tempdiff[0123456789]*',file_name)
    if match_obj:
        os.remove('.mygit/'+file_name)

#helper method
def log_entry(comment):
    log_file=open(".mygit/log",'a')
    master_file=open('.mygit/master_file.txt','r')
    current_commit_num=master_file.read()
    master_file.close()
    entry=str(int(current_commit_num)+1)+'  '+comment+'\n'
    log_file.write(entry)
    log_file.close()

def log_read():
    lfd=open('.mygit/log','r')
    for i in lfd:
        print(i)
    lfd.close()
                   
def abc(path1):
    l1=path1.split('/')
    file_name=l1[len(l1)-1]
    del l1[len(l1)-1]
    path2="/".join(l1)+'/'
    t1=(path2,file_name)
    return t1
#helper method


def rollback():
    master_file=open('.mygit/master_file.txt','r')
    current_commit_num=int(master_file.read())
    master_file.close()
    check_pointing(current_commit_num)
    

def current_files():
    #check all the file exept from mygit
    res=[]
    forbiden_path=os.getcwd()+'/.mygit'
    for root,dirs,files in os.walk(os.getcwd()):
        temp_root=root
        if root!=forbiden_path:
            for i in files:
                #print(temp_root+'/'+i)
                res.append(temp_root+'/'+i)
    return res



def extract_commit(commit_no):
     #this will return list of file name and there hash and ther diff file n.o
    #master_file=open('.mygit/master_file.txt','r')
    #current_commit_num=master_file.read()
    current_commit_file_name='.mygit/commit_'+str(commit_no)
    current_commit_file=open(current_commit_file_name,'r')
    num_of_file=current_commit_file.readline()#reading the number of file.
    num_of_file=num_of_file[:-1]
    my_map={}
    for i in range(int(num_of_file)):
        #this loop will run num_of_file times.
        key=current_commit_file.readline()       #complete path of the file
        key=key[:-1]
        #print('the path of the file is ',key)
        value_h=current_commit_file.readline()   #hash of the file
        value_h=value_h[:-1]
        #print('the hash of the file is ',value_h)
        value_d=current_commit_file.readline()   #diff index of the file.
        value_d=value_d[:-1]
        #print('the diff index of the file is ')
        value=(value_h,value_d)
        my_map[key]=value
    #master_file.close()
    current_commit_file.close()#closing both the opened file.
    return my_map


def extract_previous_commit_info():
     #this will return list of file name and there hash and ther diff file n.o
    master_file=open('.mygit/master_file.txt','r')
    current_commit_num=master_file.read()
    current_commit_file_name='.mygit/commit_'+current_commit_num
    current_commit_file=open(current_commit_file_name,'r')
    num_of_file=current_commit_file.readline()#reading the number of file.
    num_of_file=num_of_file[:-1]
    my_map={}
    for i in range(int(num_of_file)):
        #this loop will run num_of_file times.
        key=current_commit_file.readline()       #complete path of the file
        key=key[:-1]
        #print('the path of the file is ',key)
        value_h=current_commit_file.readline()   #hash of the file
        value_h=value_h[:-1]
        #print('the hash of the file is ',value_h)
        value_d=current_commit_file.readline()   #diff index of the file.
        value_d=value_d[:-1]
        #print('the diff index of the file is ')
        value=(value_h,value_d)
        my_map[key]=value
    master_file.close()
    current_commit_file.close()#closing both the opened file.
    return my_map

def extract_previous_add_info():
    only_add_file=open('.mygit/addlist_unique.txt')
    num_co_file=open('.mygit/myfile_counter.txt','r')
    number_of_entry=num_co_file.read()
    number_of_entry=int(number_of_entry)
    my_map={}
    for i in range(number_of_entry):
        key=only_add_file.readline()
        key=key[:-1]
        #print('the path of the file is ',key)
        value_h=only_add_file.readline()
        value_h=value_h[:-1]
        #print('the hash of the file is ',value_h)
        value_d=only_add_file.readline()
        value_d=value_d[:-1]
        #print('the temp diff index of the file is ',value_d) 
        value=(value_h,value_d)
        my_map[key]=value
    only_add_file.close()
    num_co_file.close()
    return my_map


#we have to comment the file changed code
def file_changed():  #bugs to be fixed
    my_commit_map=extract_previous_commit_info()
    prev_map=""
    my_bug_fix_flag=False
    if os.path.exists('.mygit/addlist_unique.txt'):
        #print('add info already exist')
        prev_map=extract_previous_add_info()
        my_bug_fix_flag=True
        #print('printing the add info data')
        #print(prev_map)
    else:
        #print('code should be here')
        prev_map=extract_previous_commit_info()#returns a map wiht following structure. 
        #print('printing the commit info data')
        #print(prev_map)
    cf=current_files()#return complete path of all the files in he directory.
    changed=[]
    no_changed=[]
    for e_f in cf:
        if e_f in prev_map.keys():
            #e_f_hash_value=hashlib.md5(e_f).hexdigest()
            jjj=open(e_f,'rb')
            e_f_hash_value=hashlib.md5(jjj.read()).hexdigest()#this mthods needs to improve. 
            jjj.close()
            if e_f_hash_value==prev_map[e_f][0]:
                no_changed.append(e_f)
            else:
                changed.append(e_f)
                pass
        else:
            if my_bug_fix_flag and e_f in my_commit_map.keys():
                #here we have to write plenty of code.
                jjj=open(e_f,'rb')
                e_f_hash_value=hashlib.md5(jjj.read()).hexdigest()#this mthods needs to improve. 
                jjj.close()
                if e_f_hash_value==my_commit_map[e_f][0]:
                    no_changed.append(e_f)
                else:
                    changed.append(e_f)
                    pass
            else:
                changed.append(e_f)
    t=(changed,no_changed)
    return t


def recover_file_from_commit(file_path,created_file_name):#this will cointain the complete file path
    master_file=open('.mygit/master_file.txt','r')
    current_commit_num=int(master_file.read())
    master_file.close()
    temp_commit_num=current_commit_num
    temp_commit_map=extract_commit(temp_commit_num)
    mystack=[]
    if file_path not in temp_commit_map.keys():
        #this file has not been commited even once
        return False
    else:
        while file_path in temp_commit_map.keys() and temp_commit_num>0:
            ttt=int(temp_commit_map[file_path][1])
            if ttt!=0:
                mystack.append(ttt)
            temp_commit_num=temp_commit_num-1
            temp_commit_map=extract_commit(temp_commit_num)
        #make a empty file
        j=open(created_file_name,'w')
        j.close()
        while len(mystack)!=0:
            target_diff_index=mystack.pop()
            add_the_difference(created_file_name,'.mygit/diff'+str(target_diff_index))
            #write a report about the complete strucure first.
        return True
    
def recover_file_from_commit_g(commit_no,file_path,created_file_name):#this will cointain the complete file path
    temp_commit_num=commit_no
    temp_commit_map=extract_commit(temp_commit_num)
    mystack=[]
    if file_path not in temp_commit_map.keys():
        #this file has not been commited even once
        return False
    else:
        while file_path in temp_commit_map.keys() and temp_commit_num>0:
            ttt=int(temp_commit_map[file_path][1])
            if ttt!=0:
                mystack.append(ttt)
            temp_commit_num=temp_commit_num-1
            temp_commit_map=extract_commit(temp_commit_num)
        #make a empty file
        print('i want to create the followin file ',created_file_name)
        j=open(created_file_name,'w')
        j.close()
        while len(mystack)!=0:
            target_diff_index=mystack.pop()
            add_the_difference(created_file_name,'.mygit/diff'+str(target_diff_index))
            #write a report about the complete strucure first.
        return True
        

def satus():
    t=file_changed()
    changed_file=t[0]
    not_changed_file=t[1]
    print('file which have changed')
    for i in changed_file:
        print(i)
    print('file which have not change')
    for i in not_changed_file:
        print(i)
 

def check_pointing(commit_no):
    #we have to change all the file which are present in this commit and 
    #generate them back in the  respective directory
    target_env=extract_commit(commit_no)
    for target_file_path in target_env.keys():
        if os.path.exists(target_file_path):
            print('no need to create the path.')
            os.remove(target_file_path)
            recover_file_from_commit_g(commit_no,target_file_path,target_file_path)
            pass
        else:
            #create the path.
            #retrive the file.
            t=abc(target_file_path)
            t_path=t[0]
            #t_file=tt[1]
            print('also creating the path')
            #os.popen('mkdir -p '+t_path)
            #time.sleep(1) #this bug needs to be fixed.
            os.makedirs(t_path, exist_ok=True)
            print('the path of the file is ',t_path)
            print('the tuple is ',t)
            print('the file which we want to create is ',target_file_path)
            recover_file_from_commit_g(commit_no,target_file_path,target_file_path)
            pass
        pass
    


       
#now we have to add comment in the commit functionality.                
def commit_routine(mycomment):
    #this will be our commit routine.
    if os.path.exists('.mygit/addlist_unique.txt'):
        log_entry(mycomment)
        #read the addlist_unique file in a map
        add_file_map=extract_previous_add_info()
        #now change the diff file name from temporary to permanent.
        for e_f in add_file_map.keys():
            hash_value=add_file_map[e_f][0]
            diff_index=add_file_map[e_f][1]
            #code to take out the diff counter
            diff_count_filed=open('.mygit/diff_counter','r')
            new_diff_index=int(diff_count_filed.read())
            diff_count_filed.close()
            diff_count_filed=open('.mygit/diff_counter','w')
            diff_count_filed.write(str(new_diff_index+1))
            diff_count_filed.close()
            #rename the file
            os.rename('.mygit/tempdiff'+str(diff_index),'.mygit/diff'+str(new_diff_index))
            #now modifie the entry
            t=(hash_value,new_diff_index)
            add_file_map[e_f]=t
        commit_file_map=extract_previous_commit_info()
        current_file_list=current_files()
        res_map={}
        #and add file map is already here.
        for e_f in commit_file_map.keys():
            if e_f in current_file_list:
                res_map[e_f]=commit_file_map[e_f]
                pass
            else:
                print('this file is deleted :: ',e_f)
        for e_f in add_file_map:
            if e_f in current_file_list:
                res_map[e_f]=add_file_map[e_f]
            else:
                print('this file is deleted ::',e_f)
        #now we have to write the data back.
        number_of_element=len(res_map)
        master_filed=open('.mygit/master_file.txt','r')
        commit_no=int(master_filed.read())
        commit_no=commit_no+1
        master_filed.close()
        commit_filed=open('.mygit/commit_'+str(commit_no),'w')
        commit_filed.writelines(str(number_of_element)+'\n')
        for e_f in res_map.keys():
            path_m=e_f
            hash_m=res_map[e_f][0]
            diff_m=res_map[e_f][1]
            commit_filed.writelines(path_m+'\n')
            commit_filed.writelines(hash_m+'\n')
            commit_filed.writelines(str(diff_m)+'\n')
        commit_filed.close()
        #delete the add file routiness
        os.remove('.mygit/addlist_unique.txt')
        os.remove('.mygit/myfile_counter.txt')
        os.remove('.mygit/add_diff.txt')
        #increase the counter of master file.
        master_filed=open('.mygit/master_file.txt','w')
        master_filed.write(str(commit_no))
        master_filed.close()
    else:
        print('nothing to commit')
    pass
    

# a above this is fixed now we have to fix the diff counter if we want not nessary
def add(x):
    x=os.path.abspath(x)
    my_map=extract_previous_commit_info()
    changed_fil=file_changed()[0]
    add_file_dis=""
    add_map=""
    if x in changed_fil:
        if os.path.exists('.mygit/addlist_unique.txt'):
            #opening the file in just append mode
            add_map=extract_previous_add_info()
            add_file_dis=open('.mygit/addlist_unique.txt','a')
            #here we will check he path x already exist or not if yes then we will create a new file
            #and so on.
            if x in add_map.keys():
                #i.e the file is already present
                #first rename this file.
                #copy the content of this file into a new .mygit/addlist_unique.txt exept x
                #decrement the counter of myfile_counter.txt
                #os.rename('.mygit/addlist_unique.txt','.mygit/spiderman.txt')
                #os.remove('.mygit/addlist_unique.txt')
                add_file_dis.close()#remeber to reopen it
                extra_file=open('.mygit/addlist_unique.txt','w')
                for ee in add_map.keys():
                    if ee!=x:
                        extra_file.writelines(ee+'\n')
                        extra_file.writelines(add_map[ee][0]+'\n')
                        extra_file.writelines(add_map[ee][1]+'\n')
                pass
                #now we have to decrement the counter
                temp_counter_file=open('.mygit/myfile_counter.txt','r')
                t=int(temp_counter_file.read())
                t=t-1
                temp_counter_file.close()
                temp_counter_file=open('.mygit/myfile_counter.txt','w')
                temp_counter_file.write(str(t));
                temp_counter_file.close()
                extra_file.close()
                add_file_dis=open('.mygit/addlist_unique.txt','a')
        else:
            #creating new set of files
            add_file_dis=open('.mygit/addlist_unique.txt','w')
            mytempt=open('.mygit/myfile_counter.txt','w')
            mytempt.write('0')
            mytempt.close()
            mytempt=open('.mygit/add_diff.txt','w')
            mytempt.write('1')
            mytempt.close()
    
        
        
        #this code block will change
        #incremnting the diff counter
        #current_diff_counter=open('.mygit/diff_counter','r')#does rw works.
        #diff_val=int(current_diff_counter.readline())+1
        #current_diff_counter.close()
        #current_diff_counter=open('.mygit/diff_counter','w')
        #current_diff_counter.write(str(diff_val))
        #current_diff_counter.close()
        
        
        
        #adding the path of the file
        add_file_dis.writelines(x+'\n') # writing the path of the file tuple 1.
        
        
        #adding the hash value of the file.
        jjj=open(x,'rb')
        hash_val=hashlib.md5(jjj.read()).hexdigest()#this mthods needs to improve. 
        jjj.close()
        add_file_dis.writelines(hash_val+'\n')
        
        #taking out the new index for the temporty diff file
        current_diff_counter=open('.mygit/add_diff.txt','r')
        diff_val=int(current_diff_counter.read())
        current_diff_counter.close()
        current_diff_counter=open('.mygit/add_diff.txt','w')
        current_diff_counter.write(str(diff_val+1))
        current_diff_counter.close()
        #diff_val cointain the value of the index to be used.
        
        if x in my_map.keys():
            print('the file already exist in previous commit')
            print('this will be implimented in future')
            print('all diff to the current add will be with respect to the prevous/current commit')
            #now we have to work around the general case.
            #create the tempory file
            recover_file_from_commit(x,'.mygit/marvel_is_better_than_dc.txt')
            #now we need to create the temp diff file
            temp_diff_file_name=".mygit/tempdiff"+str(diff_val)
            create_diff('.mygit/marvel_is_better_than_dc.txt',x,temp_diff_file_name)
            add_file_dis.writelines(str(diff_val)+'\n')
            os.popen('rm .mygit/marvel_is_better_than_dc.txt')
            
        else:
            #base case.
            
            #creating tempdiff<index> file
            temp_diff_file_name=".mygit/tempdiff"+str(diff_val)
            temp_diff_file=open(temp_diff_file_name,'w')
            temp_diff_file.close()
            #do he diffing here
            create_diff('.mygit/glob_empty',x,temp_diff_file_name)
            add_file_dis.writelines(str(diff_val)+'\n')
            
            
            
            
        #increamenting the value of mycounter.    
        temp_counter_file=open('.mygit/myfile_counter.txt','r')
        t=int(temp_counter_file.read())
        t=t+1
        temp_counter_file.close()
        temp_counter_file=open('.mygit/myfile_counter.txt','w')
        temp_counter_file.write(str(t));
        temp_counter_file.close()    
        add_file_dis.close()        
    else:
        print('there is no change in file ',x)
                    
                  
                    












#execuion of code start from this file.
a=sys.argv
n=len(a)
if a[1]=='init':
    if len(a)!=2:
        print("not correct number of arguments")
    else:
        print('correct implementation starts from here')
        #we need to create a master file
        os.mkdir('.mygit')
        if os.path.exists('.mygit/glob_empty'):
            pass
        else:
            my_emp=open('.mygit/glob_empty','w')
            my_emp.close()
        master_file=open('.mygit/master_file.txt','w')
        master_file.write('0')
        #now we have to do the zero't commit
        zeroth_commit=open('.mygit/commit_0','w')
        zeroth_commit.writelines('0'+'\n')
        current_diff_counter=open('.mygit/diff_counter','w')
        current_diff_counter.write('1')#take the number out and then increment
        master_file.close()
        zeroth_commit.close()
        current_diff_counter.close()
elif a[1]=='status':
        if n>2:
            print("Too many arguments")
            break
        print('calling the status code')
        satus()
elif a[1]=='add':
    print('add being called')
    i=2
    while(i<n):
        add(a[i])
        i+=1
    #print(a[2])
    #add(a[2])
elif a[1]=='commit':
    if n>3:
        print("Too many arguments")
        break
    commit_routine(a[2])
    path=os.getcwd()+'/.mygit'              #now deleting the temporary files "tempdiff*"
    for (root,dirs,files) in os.walk(path):
        for i in files:
            delete_instant(i)
    
elif a[1]=='checkpoint':
    if n>3:
        print("Too many arguments")
        break
    check_pointing(int(a[2]))
elif a[1]=='log':
    if n>2:
        print("Too many arguments")
        break
    #here we will print our log file.
    log_read()
    pass

elif a[1]=='revert':
   if n>2:
        print("Too many arguments")
        break
   rollback()
    
else:
    print('this command is not known.')
#current_files()
     
