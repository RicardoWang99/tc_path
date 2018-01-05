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

struct Info
{
  int xid,yid,dateid,hour,model;
  double wind;
};

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

  //cnt=1; numtmp[1]=0.0;
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
int main()
{
  ifstream CityData ("../data_path/CityData.csv");
  ifstream TestingData ("/home/ricardowang/tianchi/data_path/ForecastDataforTesting_201712.csv");
  ifstream TrainingData ("/home/ricardowang/tianchi/data_path/ForecastDataforTraining_201712.csv");
  ifstream TrainingRealData ("/home/ricardowang/tianchi/data_path/In_situMeasurementforTraining_201712.csv");

  int mystop=0;
  int maxx=-1,minx=9999;
  int maxy=-1,miny=9999;
  while(!TestingData. eof())
  {
    TestingData.getline(chartmp,M);
    if(mystop==0)
    {
      mystop++;
      cout<<chartmp<<endl;
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

  return 0;
}
