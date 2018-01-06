#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<queue>
#include<vector>
#include<fstream>
using namespace std;

#define M 1000
#define STOP -1
#define X 548
#define Y 421
#define Date 10
#define Hour 24
#define Model 10
#define Lim 15.0
#define oo 999999999

ifstream CityData ("../data_path/CityData.csv");
ifstream TestingData ("/home/ricardowang/tianchi/data_path/ForecastDataforTesting_201712.csv");
ifstream TrainingData ("/home/ricardowang/tianchi/data_path/ForecastDataforTraining_201712.csv");
ifstream TrainingRealData ("/home/ricardowang/tianchi/data_path/In_situMeasurementforTraining_201712.csv");

struct CITY
{
  int id,xid,yid;
}City[15];

class Indicate
{
public:
  double a[15],v;
  void GetMax()
  {
    v=a[1];
    for(int i=1;i<=Model;i++)
    {
      v=max(v,a[i]);
    }
    return ;
  }
  void Generate()
  {
    GetMax();
  }
}Indicate;

double Training[600][600][11][21];
bool MyMap[600][600][11][21];
int MyDist[600][600][11][21];
char chartmp[M];
double numtmp[M];
int cnttmp;
void myStringCut(char *a)
{
  int Len=strlen(a),cnt=0;
  double NewK=1,OldK=10;

  cnt=1; numtmp[1]=0.0;
  for(int i=0;i<Len;i++)
  {
    if(a[i]==',')
    {
      cnt++;
      numtmp[cnt]=0;
    }
    else if(a[i]=='.')
    {
      NewK=0.1; OldK=1;
    }
    else
    {
      numtmp[cnt]=numtmp[cnt]*OldK+NewK*(a[i]-'0');
      if(NewK<0.9)NewK=NewK/10.0;
    }
  }
  cnttmp=cnt;
  return ;
}
int dx[]={0,0,1,-1};
int dy[]={1,-1,0,0};
void BFS()
{
  queue<pair<int,int> > Q;
  int ix,iy,jx,jy;
  for(int xid=1;xid<=X;xid++)
  {
    for(int yid=1;yid<=Y;yid++)
    {
      for(int date=6;date<=10;date++)
      {
        for(int hour=3;hour<21;hour++)
        {
          MyDist[xid][yid][date][hour]=oo;
        }
      }
    }
  }
  for(int date=6;date<=10;date++)
  {
    if(MyMap[City[0].xid][City[0].yid][date][3]==false){continue;}

    for(int hour=3;hour<21;hour++)
    {
      while(!Q.empty())Q.pop();

      if(hour==3)
      {
        MyDist[City[0].xid][City[0].yid][date][3]=0;
      }
      for(int xid=1;xid<=X;xid++)
      {
        for(int yid=1;yid<=Y;yid++)
        {
          if(MyDist[xid][yid][date][hour]==0)
          {
            Q.push(make_pair(xid,yid));
          }
        }
      }

      while(!Q.empty())
      {
        ix=Q.front().first;iy=Q.front().second;
        Q.pop();
        if(MyMap[ix][iy][date][hour+1]==true)
        {
          MyDist[ix][iy][date][hour+1]=0;
        }
        for(int k=0;k<4;k++)
        {
          jx=ix+dx[k]; jy=iy+dy[k];
          if(MyDist[ix][iy][date][hour]<29)
          {
            if(jx>=1 && jx<=X
               && jy>=1 && jy<=Y
               && MyDist[jx][jy][date][hour]==oo
               && MyMap[jx][jy][date][hour]==true)
            {
              MyDist[jx][jy][date][hour]=MyDist[ix][iy][date][hour]+1;
              Q.push(make_pair(jx,jy));
            }
          }
          else if(MyDist[ix][iy][date][hour]==29)
          {
            if(jx>=1 && jx<=X
               && jy>=1 && jy<=Y
               && MyDist[jx][jy][date][hour+1]==oo
               && MyMap[jx][jy][date][hour+1]==true)
            {
              MyDist[jx][jy][date][hour+1]=0;
            }
          }
        }
      }
    }

  }
  return ;
}
void Out(int xid,int yid,int date,int hour,int CityId,int mi=-1)
{
  if(mi==-1)
  {
    mi=MyDist[xid][yid][date][hour];
  }
  printf("%d,%d,%d:%02d,%d,%d\n",CityId,date,hour,mi*2,xid,yid);
  return ;
}
void Goback(int xid,int yid,int date,int hour,int CityId)
{
  if(MyDist[xid][yid][date][hour]==0)
  {
    if(hour==3)
    {
      Out(xid,yid,date,hour,CityId);
      return ;
    }
    else
    {
      if(MyDist[xid][yid][date][hour-1]!=oo)
      {
        Goback(xid,yid,date,hour-1,CityId);
        for(int i=MyDist[xid][yid][date][hour-1]+1;i<30;i++)
        {
          Out(xid,yid,date,hour-1,CityId,i);
        }
        Out(xid,yid,date,hour,CityId);
      }
      else
      {
        int jx,jy;
        for(int k=0;k<4;k++)
        {
          jx=xid+dx[k];
          jy=yid+dy[k];
          if(jx>=1 && jx<=X && jy>=1 && jy<=Y && MyDist[jx][jy][date][hour-1]!=oo)
          {
            Goback(jx,jy,date,hour-1,CityId);
            for(int i=MyDist[jx][jy][date][hour-1]+1;i<30;i++)
            {
              Out(jx,jy,date,hour-1,i,CityId);
            }
            Out(xid,yid,date,hour,CityId);
            break;
          }
        }
      }
      return ;
    }
  }
  else
  {
    int jx,jy;
    for(int k=0;k<4;k++)
    {
      jx=xid+dx[k];
      jy=yid+dy[k];
      if(jx>=1 && jx<=X &&
        jy>=1 && jy<=Y &&
        MyDist[jx][jy][date][hour]+1 == MyDist[xid][yid][date][hour])
        {
          Goback(jx,jy,date,hour,CityId);
          Out(xid,yid,date,hour,CityId);
          break;
        }
    }
    //cout<<"???"<<endl;
  }
}
void FindPath()
{
  BFS();
  for(int date=6;date<=10;date++)
  {
    for(int CityId=1;CityId<=10;CityId++)
    {
      //printf("Finding day %d City %d\n",date,CityId);
      bool FLAG=false;
      for(int hour=3;hour<21;hour++)
      {
        if(MyDist[City[CityId].xid][City[CityId].yid][date][hour]!=oo)
        {
          FLAG=true;
          Goback(City[CityId].xid,City[CityId].yid,date,hour,CityId);
          break;
        }
      }
      if(!FLAG)
      {
        //FUCK
        //cout<<"FUCK"<<endl;

        int hour=3,mi=0,xid=City[0].xid,yid=City[0].yid;
        for(;xid<=City[CityId].xid;xid+=(City[0].xid<City[CityId].xid ? 1 : -1))
        {
          Out(xid,yid,date,hour,CityId,mi);
          mi++;
          if(mi==30)
          {
            mi=0; hour++;
          }
        }
        for(;yid<=City[CityId].yid;yid+=(City[0].yid<City[CityId].yid ? 1 : -1))
        {
          Out(xid,yid,date,hour,CityId,mi);
          mi++;
          if(mi==30)
          {
            mi=0; hour++;
          }
        }

      }
    }
  }
  return ;
}
void ReadTest()
{
  int mystop=0;

  while(!TestingData. eof())
  {
    TestingData.getline(chartmp,M);
    if(mystop==0)
    {
      mystop++;
      //cout<<chartmp<<endl;
      continue;
    }
    myStringCut(chartmp);

    if(mystop==STOP){break;}
    if(cnttmp==1){break;}

    Indicate.a[(int)numtmp[5]]=numtmp[6];
    if((int)numtmp[5]==10)
    {
      Indicate.Generate();
      Training[(int)numtmp[1]][(int)numtmp[2]][(int)numtmp[3]][(int)numtmp[4]]=Indicate.v;
    }
    mystop++;
  }

  for(int xid=1;xid<=X;xid++)
  {
    for(int yid=1;yid<=Y;yid++)
    {
      for(int date=6;date<=10;date++)
      {
        for(int hour=3;hour<21;hour++)
        {
          MyMap[xid][yid][date][hour]=(Training[xid][yid][date][hour]<=Lim);
        }
      }
    }
  }
  //cout<<"ReadTestDone"<<endl;
  return ;
}
void ReadCity()
{
  int mystop=0;

  while(!CityData. eof())
  {
    CityData.getline(chartmp,M);
    if(mystop==0)
    {
      mystop++;
      continue;
    }
    //cout<<chartmp<<endl;
    myStringCut(chartmp);

    if(mystop==STOP){break;}
    if(cnttmp==1){break;}

    City[(int)numtmp[1]]=(CITY){(int)numtmp[1],(int)numtmp[2],(int)numtmp[3]};
    mystop++;
  }

/*
  for(int i=0;i<=10;i++)
  {
    printf("Id:%d x:%d y:%d\n",City[i].id,City[i].xid,City[i].yid);
  }
  cout<<"ReadCityDone"<<endl;*/
  return ;
}
int main()
{
  freopen("ans.csv","w",stdout);
  ReadCity();
  ReadTest();
  FindPath();
  return 0;
}
