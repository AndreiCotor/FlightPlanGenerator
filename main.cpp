#include<iostream>
#include<fstream>
#include<math.h>
#include<map>

using namespace std;

const double PI=3.14159265359;
const double earthRadius=6371;
const double maxDist=100.0;

double toRadians(double x)
//converts an angle from degrees to radians
{
    x=(PI*x)/180.0;
    return x;
}

double toDegrees(double x)
//converts an angle from radians to degrees
{
    x=(x*180.0)/PI;
    return x;
}

class point
{
    public:
        double latitude,longitude;

        void init(double a, double b)
        {
            latitude=toRadians(a);
            longitude=toRadians(b);
        }
};

class waypoint
{
    public:
        point coord;
        string name;

        void init(string s, double a, double b)
        {
            coord.latitude=a;
            coord.longitude=b;
            name=s;
        }

        void init(string s, point x)
        {
            coord=x;
            name=s;
        }
};

int nrW;
waypoint W[200005];
map<string,point> Waypoints;

void importNavData()
//imports the navigation data from earth_fix.dat
{
    ifstream fi("earth_fix.dat");

    while(1)
    {
        double x,y;
        fi>>x>>y;
        if(x>180.0)
            break;

        string s,trash1,trash2,trash3;
        fi>>s>>trash1>>trash2>>trash3;

        point p;
        p.init(x,y);
        Waypoints[s]=p;

        W[++nrW].init(s,x,y);
    }
    fi.close();
}

bool cmp(waypoint a, waypoint b)
//compares 2 points by their latitude
{
    return a.coord.latitude<b.coord.latitude;
}

void buildGraph()
//build a graph of waypoints which are at a distance<maxDist
//to be called only once
{
    ofstream fo("precalc.dat");
    sort(W+1,W+nrW+1,cmp);

    for(int i=1; i<=nrW; i++)
    {
        fo<<W[i].name<<" ";

        int ind=i-1;
        while(1)
        {
            if(ind==0)
                ind=nrW;

            point aux;
            aux.latitude=W[ind].latitude;
            aux.longitude=W[i].longitude;
            if(greatCircleDistance(W[i].coord,aux)>maxDist)
                break;

            if(greatCircleDistance(W[i].coord,W[ind].coord)<maxDist)
                fo<<ind<<" ";

            ind--;
        }

        ind=i+1;
        while(1)
        {
            if(ind==nrW+1)
                ind=1;

            point aux;
            aux.latitude=W[ind].latitude;
            aux.longitude=W[i].longitude;
            if(greatCircleDistance(W[i].coord,aux)>maxDist)
                break;

            if(greatCircleDistance(W[i].coord,W[ind].coord)<maxDist)
                fo<<ind<<" ";

            ind++;
        }

        fo<<"\n";
    }

    fo.close();
}

double greatCircleDistance(point a, point b)
//computes the great circle distance (distance in the air) between 2 points
{
    double angleToCenter=acos(sin(a.latitude)*sin(b.latitude)+cos(a.latitude)*cos(b.latitude)*cos(a.longitude-b.longitude));
    double dist=angleToCenter*earthRadius;
    return dist;
}

int main()
{
    importNavData();
    //buildGraph();
    //importGraph();

    return 0;
}
