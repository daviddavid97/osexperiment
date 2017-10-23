#include<stdio.h>
#include<string.h>
#include <stdlib.h> 
 
#define Size 30 //最大允许输入进程个数 30
#define Max_QueueNumb 6//最大允许多级反馈队列级数 6
 
int SFtype=0;//调度算法类型
int TCPU=0;//记录CPU主频时间
int TimePice=100;   //记录时间片大小,默认为100,表示无时间片（时间片轮转算法涉及）
int QueueNumb=3;//默认多级反馈队列级数（TimePice=1、2、4……）

_Bool QueueEndLine_Flag=0;//标记是否到达最后一个反馈队列
int Drop_numb=0;//标记降级个数             
 
int Num=30;    //记录用户输入的进程数,初始值30
//进程PCB结构体
struct Process {
               char ProcessName;//进程名字  *
               int Need_Time;                  //进程需要时间 *
               int Arrive_time;           //进程到达CPU时间 *
               int Level;                           //进程优先级  [*]
 
               int LeftTime;               //进程运行一段时间后还需要的时间
               int Finish_time;           //进程完成时间
              int Turn_time;              //周转时间
               float Qturn_time;         //带权周转时间
         /*周转时间=完成时间-到达CPU时间；带权周转时间=周转时间/进程需要时间*/
 }; 
struct Process wait_process[Size]; //声明进程结构体数组
struct Process Mqueue_process[Max_QueueNumb][Size];//声明多级反馈队列
#define Process  struct Process 
//函数声明,用在多级反馈算法
void Move(Process PCB_out, Process process[],int length) //把PCB_out降级给process[]
{   
       process[length].ProcessName=PCB_out.ProcessName;  
       process[length].Need_Time=PCB_out.Need_Time;  
       process[length].Level=0;
       process[length].Arrive_time=PCB_out.Arrive_time;
 
       process[length].LeftTime=PCB_out.LeftTime;
       process[length].Finish_time=-1;
       process[length].Turn_time=-1;
       process[length].Qturn_time=-1;
} 
void Sort_By_ArriveTime( Process  process[], int size)  //按进程到达时间从低到高排序
{    
       //  直接插入排序 
       for( int i=1;i<size;i++)
       { 
              Process temp;    
              temp = process[i];    
              int j=i;      
      
              while(j>0 && temp.Arrive_time<process[j-1].Arrive_time) 
              {   
                     process[j]=process[j-1];   
                     j--;  
              } 
          
              process[j] = temp;
       }
}
 
void init_PCB();//初始化进程结构体数组
void init_Mqueue();
void Run_PCB(Process  process[],int num,int Timepice);//当前进程执行函数，用于计算、更改、进程属性值，并显示
 
void Sort_ByLvl(Process  pr[], int size) ; //此排序后按优先级从大到小排列(优先级降序排序)
void Sort_ByTime(Process  pr[], int size) ; // 此排序后按需要的cpu所需时间从小到大排列（短作业优先）
void Fcfs(Process process[],int num); //先来先服务算法
void TimeTurn(Process process[], int num, int Timepice); //时间片轮转算法
void MultQueue(int num, int QueueNumb);//多级反馈队列调度算法
        
int main(void)
{
       int end_circle_flag=0;    //循环结束标志    
       while(!end_circle_flag)
       {
            printf("\n");
               printf("选择调度算法：\n");
               printf("1: FCFS  2:时间片轮换(RR) 3: 最短作业优先 4:动态优先级调度 5: 多级反馈队列调度  6:其他(退出)\n" ); 
               printf("输入你选择调度算法：\n");
               scanf("%d",&SFtype); //记录模拟的调度算法
               fflush(stdin);
               switch(SFtype)
               {
                      case 1:{
                                          init_PCB();//初始化进程结构体数组
                                          Fcfs(wait_process,Num);//假设先进先出是非抢占式的
                                          printf("\n-----------------FcFs进程调度模拟结束----------------------");
                                          break;
                                   }
                      case 2:{
                                          init_PCB();//初始化进程结构体数组         
                                          TimeTurn(wait_process,Num,TimePice);//时间片轮转算法
                                          printf("\n----------------时间片轮转算法模拟结束----------------------");
                                          break;
                                   }
                      case 3:{
                                          init_PCB();     //初始化进程结构体数组                                              
                                          Sort_ByTime(wait_process,Num); //按服务时间排序（从低到高）
                                          Fcfs(wait_process,Num);//调用先进先出算法
                                          printf("\n-----------------最短作业优先算法模拟结束----------------------");
                                          break;
                                   }
                      case 4:{ 
                                          init_PCB();//初始化进程结构体数组
                                          while (Num!=0)//执行进程调度
                                          {
                                                 Sort_ByLvl(wait_process,Num);//按优先级对进程组排序
                                                 Run_PCB(wait_process,Num,TimePice);//执行一次进程块
                                                 Num--;
             
                                                 printf("\n");               
                                                 system("pause");   //暂停          
                                                 printf("\n");
                                          }
 
                                          printf("\n-----------------动态优先级算法模拟结束----------------------");
                                          break;
                                   }
                      case 5:{                                   
                                          init_Mqueue();//初始化多级反馈队列的第一个队列
                                          MultQueue(Num,QueueNumb);//多级反馈队列算法
                                          break;
                                   }    
             default:{
                                          end_circle_flag=1;
                                          break;
                                    }
               }
       }
       return 0;
}
 
///////////////////////////////////////////////////////////////////////////////////
void init_PCB()
{
               if (SFtype==2)//时间片轮换调度算法
               {
                      printf("输入进程调度算法时间片大小: ");
                      scanf("%d",&TimePice);
                      fflush(stdin);
               }
 
               if (SFtype==5)//多级反馈队列调度
               {
                      printf("输入多级反馈队列级数: ");
                      scanf("%d",&QueueNumb);
                      fflush(stdin);
               }
               
               printf("输入进程个数:");
               scanf("%d",&Num);
               fflush(stdin);
 
               //Process wait_process[Size];  //进程结构体就绪数组
               TCPU=0;      //初始化CPU主频时间
 
               for( int i=0; i<Num; i++) //对进程依次输入属性
               {   
                      char name='0'; 
                      int cpuTime=0;
                      int level=0;
                      int arrive_time=0;//默认值
 
                      printf("\n输入第%d个进程的名字:",i+1); 
                      scanf("%c",&name);
                      fflush(stdin);
 
                      printf("输入第%d个进程的cpu执行所需时间:",i+1); 
                      scanf("%d",&cpuTime);
                      fflush(stdin);
 
                      printf("输入第%d个进程到达CPU时间:",i+1);
                      scanf("%d",&arrive_time);
                      fflush(stdin);
 
                      if (SFtype==3)
                      {
                             printf("输入第%d个进程的进程优先级:",i+1); 
                             scanf("%d",&level);
                      }
                      
                      //初始化进程结构体
                      wait_process[i].ProcessName=name;  
                      wait_process[i].Need_Time =cpuTime;  
                      wait_process[i].Level =level;
                      wait_process[i].Arrive_time =arrive_time;
                      
                      printf("\n");
               }
 
               for ( int k=0;k<Num;k++)//对进程剩余时间、完成时间初始化、周转时间、带权周转时间 
               {   
                      wait_process[k].LeftTime=wait_process[k].Need_Time ;
                      wait_process[k].Finish_time=-1;
                      wait_process[k].Turn_time=-1;
                      wait_process[k].Qturn_time=-1;
               }
               
               printf("进程名 优先级  到达时间 占用时间  还需时间 完成时间  周转时间 带权周转时间\n");                            
               printf("------------------------------------------------------------------------------\n");
}
 
/////////////////////////////begin/////////////////////////////////////////////               
void Run_PCB(Process  process[],int num,int Timepice)//当前进程执行函数，用于计算、更改、进程属性值，并显示
{    
              //printf("\n"); 
              if (process[0].Arrive_time>TCPU)//进程均未到达
              {
                     if (Timepice==100)//如果非时间片调度
                     {
                            TCPU+=process[0].Arrive_time;//CPU等待执行到第一个进程到达
                     }
                     else
                            TCPU+=TimePice;//执行下一个时间片
                    
                     Run_PCB(process,num,Timepice);//嵌套调用Run_PCB                   
              }
              else if (process[0].LeftTime <= TimePice)//如果时间片够用且进程已经到达，则执行完毕
              {
                     TCPU+=process[0].LeftTime;//修改CPU时间                
                     process[0].LeftTime=0;//修改剩余时间为0                                       
                     process[0].Finish_time=TCPU;//修改进程结束时间
 
                    process[0].Turn_time=process[0].Finish_time-process[0].Arrive_time;//修改周转时间
 
                     process[0].Qturn_time=process[0].Turn_time*1.0/process[0].Need_Time;//修改带权周转时间
 
                     //printf("进程名字   优先级            到达时间      占用CPU时间");  
                   //printf("还需CPU时间        完成时间    周转时间        带权周转时间");
                     printf("   %c     %d         %d      %d            %d        %d          %d        %f",
                     process[0].ProcessName,process[0].Level,process[0].Arrive_time,process[0].Need_Time,
                     process[0].LeftTime,process[0].Finish_time,process[0].Turn_time,process[0].Qturn_time);
              
                     for (int i=0;i<--num;i++)   
                     {
                            process[i]=process[i+1]; //让首进程出数组   
                     }                 
              }
              else//时间片调度，且规定时间片内未执行完毕
                     {
                            process[0].LeftTime  -=  Timepice; //修改剩余时间
                           
                            TCPU+=TimePice;//修改CPU时间
 
                            Process temp;                    //将首进程置尾    
                            temp = process[0];                           
                            for( int j=0;j<(num-1);j++)                            
                                   process[j] = process[j+1];   
                           
                            process[num-1] = temp;   //将首进程置尾
 
                            if (SFtype==3)//优先级调度
                            {
                                   process[0].Level--;//首进程的优先级减一（动态优先级调度）                                 
                            }
                            Run_PCB(process,num,Timepice);//嵌套调用Run_PCB                   
                     }
              /*          
              for(int s=1;s<num;s++)                                
              {     
                     cout<<" "<<process[s].ProcessName <<"          "<<process[s].Need_Time <<"                 ";                       
                     cout<<process[s].LeftTime    <<"                 "<<process[s].Level<<"     等待"<<endl;                    
              }              
           */
}
/////////////////////////////////end/////////////////////////////////////////           
 
////////////////////////////////begin//////////////////////////////////////////        
/*FCFS(先来先服务)算法的实现（不涉及时间片）*/     
void Fcfs(Process process[], int size)
{ 
       // process[] 是输入的(进程数组)首进程，num是进程的数目，Timepice是时间片大小(默认非抢占式，手动取100)
       while (size!=0)
       {
              Run_PCB(process,size,100);
              size--;
             
              printf("\n");          
              system("pause");   //暂停           
              printf("\n");
       }     
}
///////////////////////////////////end///////////////////////////////////////
 
 
///////////////////////////////////begin///////////////////////////////////////
/* 最短作业优先算法的实现（不涉及时间片）*/           
void Sort_ByTime ( Process  process[], int size)  //按进程所需时间从低到高排序
{    
       //  直接插入排序 
       for( int i=1;i<size;i++)
       { 
              Process temp;    
              temp = process[i];    
              int j=i;      
             
              while(j>0 && temp.Need_Time<process[j-1].Need_Time ) 
              {   
                     process[j]=process[j-1];   
                     j--;  
              }            
              process[j] = temp;
       }
}
/////////////////////////////end/////////////////////////////////////////////
 
////////////////////////////////begin//////////////////////////////////////////
/* 时间片轮转调度算法实现*/                       
void TimeTurn( Process process[], int num, int Timepice)//跟FCFS类似
{           
       while (num!=0)
              {
                     Run_PCB(process,num,Timepice);
                     num--;
 
                     printf("\n");                                 
                     system("pause");   //暂停                                
                     printf("\n");
              }         
}
//////////////////////////////////end////////////////////////////////////////
 
 
////////////////////////////////begin//////////////////////////////////////////
//动态优先级调度算法（涉及时间片）           
void Sort_ByLvl( Process process[], int size)  //以进程优先级高低排序
{
       //直接插入排序
       for( int i=1;i<size;i++) 
       {
              Process temp;   //进程类对象：temp
              temp = process[i];   
              int j=i;   
              while(j>0 && temp.Level>=process[j-1].Level) //依次比较之前已经排序好的数组，若小，则不动
              {      
                     process[j] = process[j-1];
                     j--;  
              }          
              process[j] = temp;
       }
 
       /*from other  
              while(j>0 && temp.Level<pr[j-1].Level) //依次比较之前已经排序好的数组，若大，则不动
              {      
                     pr[j] = pr[j-1];
                     j--;  
              } 
              pr[j] = temp;
       }                    
        // 直接插入排序后，进程按优先级从小到大排列                      
        for( int d=size-1;d>size/2;d--)
        { 
               Process temp;  
               temp=pr [d];    
               pr [d] = pr [size-d-1]; 
               pr [size-d-1]=temp;
        } 
        // 此排序后按优先级从大到小排列 (order by value)
       */
}
//////////////////////////////////end////////////////////////////////////////
 
void init_Mqueue()
{
               printf("输入进程调度算法时间片大小: ");
               scanf("%d",&TimePice);
               fflush(stdin);
 
               printf("输入多级反馈队列级数: ");
               scanf("%d",&QueueNumb);
               fflush(stdin);
               
               printf("输入进程个数:");
               scanf("%d",&Num);
               fflush(stdin);
 
               TCPU=0;      //初始化CPU主频时间
               for( int i=0; i<Num; i++) //对进程依次输入属性
               {   
                      char name='0'; 
                      int cpuTime=0;
                      int level=0;
                      int arrive_time=0;//默认值
 
                      printf("\n输入第%d个进程的名字:",i+1); 
                      scanf("%c",&name);
                      fflush(stdin);
 
                      printf("输入第%d个进程的cpu执行所需时间:",i+1); 
                      scanf("%d",&cpuTime);
                      fflush(stdin);
 
                      printf("输入第%d个进程到达CPU时间:",i+1);
                      scanf("%d",&arrive_time);
                      fflush(stdin);
 
                      //初始化进程结构体
                      Mqueue_process[0][i].ProcessName=name;  
                      Mqueue_process[0][i].Need_Time =cpuTime;  
                      Mqueue_process[0][i].Level =level;
                      Mqueue_process[0][i].Arrive_time =arrive_time;
                      
                      printf("\n");
               }

//
               for ( int k=0;k<Num;k++)//对进程剩余时间、完成时间初始化、周转时间、带权周转时间 
                   //4个k原本是i
               {   
                      Mqueue_process[0][k].LeftTime=wait_process[k].Need_Time ;
                      Mqueue_process[0][k].Finish_time=-1;
                      Mqueue_process[0][k].Turn_time=-1;
                      Mqueue_process[0][k].Qturn_time=-1;
               }
               
               printf("进程名 优先级  到达时间 占用时间  还需时间 完成时间  周转时间 带权周转时间\n");
               printf("------------------------------------------------------------------------------\n");
}
 
void Run_Mqueue(Process process[],int num,int Timepice,int Qid)//用于多级反馈队列，当前队列的执行过程模拟，返回进入下一级队列的进程个数
//当前进程执行函数，用于计算、更改、进程属性值，并显示;Qid是当前队列的级数
{    
              printf("\n");
 
              if (process[0].Arrive_time>TCPU)//进程均未到达
              {
                     TCPU+=TimePice;//执行下一个时间片
                           
                     Run_Mqueue(process,num,Timepice,Qid);//嵌套执行
              }
              else if (process[0].LeftTime <= TimePice)//如果时间片够用且进程已经到达，则执行完毕
              {
                     TCPU+=process[0].LeftTime;//修改CPU时间
                    
                     process[0].LeftTime=0;//修改剩余时间为0                       
                    
                     process[0].Finish_time=TCPU;//修改进程结束时间
 
                    process[0].Turn_time=process[0].Finish_time-process[0].Arrive_time;//修改周转时间
 
                     process[0].Qturn_time=process[0].Turn_time*1.0/process[0].Need_Time;//修改带权周转时间
 
                     //printf("进程名字   优先级            到达时间      占用CPU时间");  
                    //printf("还需CPU时间        完成时间    周转时间        带权周转时间");
                     printf("%c      %d  %d  %d  %d  %d  %d  %f",
                     process[0].ProcessName,process[0].Level,process[0].Arrive_time,process[0].Need_Time,
                     process[0].LeftTime,process[0].Finish_time,process[0].Turn_time,process[0].Qturn_time);
                    
                     for (int j=0;j<--num;j++)   
                     {
                            process[j]=process[j+1]; //让首进程出数组   
                     }
                    
              }
              else//时间片调度，且规定时间片内未执行完毕
                     {
                            process[0].LeftTime       -=  Timepice; //修改剩余时间
                           
                            TCPU+=TimePice;//修改CPU时间
 
                            if(QueueEndLine_Flag){
                                   Process temp;                    //如果是最后一个队列，且未能在一个    
                                   temp =process[0];               //时间片内执行完毕，则将首进程置尾           
                                   for( int j=0;j<(num-1);j++)   //                       
                                          process[j] = process[j+1];  // 
                                                                                           //
                                   process[num-1] = temp;        //
 
                                   Run_Mqueue(process,num,Timepice,Qid);//嵌套执行
                            }else
                            {
                                   //出当前队列，进入下个队列
                                   Move(process[0],Mqueue_process[Qid+1],Drop_numb++);
                                   for (int i=0;i<--num;i++)   
                                   {
                                          process[i]=process[i+1]; //让首进程出数组   
                                   }
                            }                
                     }
}
 
 
void MultQueue(int num, int QueueNumb)
{
       Sort_By_ArriveTime(Mqueue_process[0],num);//按进程到达CPU的时间先后顺序排序
      
       Run_Mqueue(Mqueue_process[0],Num,1,0);
       Drop_numb=0;//降级数目置零
       for(int Queue_id=1;Queue_id<QueueNumb;Queue_id++)
       {
              Run_Mqueue(Mqueue_process[Queue_id],Num,2*Queue_id,Queue_id);
              Drop_numb=0;//降级数目置零
       }
 
}
