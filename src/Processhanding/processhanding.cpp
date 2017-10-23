#include<iostream>  
#include<cstdio>  
#include<cstring>  
#include<string>  
#include<cstring>  
#include<malloc.h>  
#include<algorithm>  
#include<vector>  
  
using namespace std;  
  
char c[5][10]={{"Run"},{"Ready"},{"Finish"}};  
class PCB  //进程控制块  
{  
private:  
    string name;  
    int flag;//进程标识符  
    int importance;//优先数  
    int occupy_time;//占用的CPU时间片数  
    int need_time;//进程所需时间片数  
    int now_condition;//进程状态  
public:  
    PCB *next;//连接指针  
    static int pcd_sta;  
    PCB(string _name,int _importance,int _need_time,int _now_condition):importance(_importance),occupy_time(0),need_time(_need_time),now_condition(_now_condition)  
    {  
        next=NULL;flag=pcd_sta++;name=_name;  
    }  
    ~PCB(){}  
    void run()  
    {  
        occupy_time++;need_time--;now_condition=0;  
        importance-=3;  
        print_condition();  
        now_condition=1;  
    }  
    void run(int time)  
    {  
        occupy_time+=time;need_time-=time;now_condition=0;  
        if(need_time<=0) {occupy_time+=need_time;need_time=0;}  
        //importance-=3;  
        print_condition();  
        now_condition=1;  
    }  
    void setFinish(){now_condition=2;cout<<name<<":"<<c[now_condition]<<endl;}  
    int getImportance(){return importance;}  
    int getNeedTime(){return need_time;}  
    void print_condition()  
    {  
        cout<<name<<":"<<c[now_condition]<<"   已经运行了："<<occupy_time<<"  当前重要程度为："<<importance<<endl;  
    }  
    string getName(){return name;}  
};  
int PCB::pcd_sta=1;  
PCB *ready=NULL;  
PCB *run=NULL;  
PCB *finish=NULL;  
PCB *tail=NULL;  
void addPCB(PCB *now) //  
{  
    if(ready==NULL)  
    {  
        ready=now;tail=now;now->next=NULL;  
    }  
    else  
    {  
        PCB *play=ready;  
        PCB *last=NULL;  
        while(play!=NULL)  
        {  
            if(now->getImportance()>play->getImportance())  
            {  
                if(last==NULL){ready=now;now->next=play;}  
                else{last->next=now;now->next=play;}  
                return;  
            }  
            last=play;  
            play=play->next;  
        }  
        tail->next=now;  
        now->next=NULL;  
        tail=now;  
    }  
    return;  
}  
void printName(PCB *start)  
{  
    PCB* play=start;  
    while(play!=NULL)  
    {  
        cout<<play->getName()<<endl;  
        play=play->next;  
    }  
    return;  
}  
void addPCB_Simple(PCB *now)  
{  
    if(ready==NULL)  
    {  
        ready=now;tail=now;now->next=NULL;  
    }  
    else  
    {  
        tail->next=now;  
        now->next=NULL;  
        tail=now;  
    }  
}  
int main()  
{  
    PCB *pcb[10];  
    pcb[0]=new PCB("Play_1",100,40,1);  
    pcb[1]=new PCB("Play_2",90,40,1);  
    pcb[2]=new PCB("Play_3",80,40,1);  
    pcb[3]=new PCB("Play_4",70,40,1);  
    pcb[4]=new PCB("Play_5",60,40,1);  
    printf("请输入要执行的调度算法（1.优先数，2.简单轮转）:");  
    int t;  
    scanf("%d",&t);  
    if(t==1){  
    for(int i=0;i<5;i++)  
    {  
        addPCB(pcb[i]);  
    }  
    while(ready!=NULL)  
    {  
        run=ready;  
        ready=ready->next;  
        run->next=NULL;  
        run->run();  
        if(run->getNeedTime()<=0)  
        {  
            if(finish==NULL) finish=run;  
            else{  
            PCB *f=finish;  
            while(f->next!=NULL&&f!=NULL) f=f->next;  
            f->next=run;}  
            run->setFinish();  
        }  
        else  
        {  
            addPCB(run);  
        }  
    }  
    printName(finish);  
    }  
    else  
    {  
        int time=10;  
        for(int i=0;i<5;i++)  
        {  
             addPCB_Simple(pcb[i]);  
        }  
        while(ready!=NULL)  
        {  
            run=ready;  
            ready=ready->next;  
            run->next=NULL;  
            run->run(time);  
            if(run->getNeedTime()<=0)  
            {  
                if(finish==NULL) finish=run;  
                else{  
                 PCB *f=finish;  
                 while(f->next!=NULL) {f=f->next;}  
                 f->next=run;}  
                 run->setFinish();  
            }  
            else  
            {  
                addPCB_Simple(run);  
            }  
  
        }  
    }  
    return 0;  
}  
