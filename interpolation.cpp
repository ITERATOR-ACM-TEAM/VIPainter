#include "interpolation.h"
#include <cstring>
#include <QDebug>

Newton::Newton():n(0)
{
    memset(f, 0, sizeof(f));   //均差形式
}

Newton::Newton(int n, double *x, double *y)//差分形式
{
    this->n = n;
    L = R = x[0];
    memset(f, 0, sizeof(f));
    for(int i = 0; i <= n; i++)
    {
        this->x[i] = x[i];
        f[i][0] = y[i];
        L = L < x[i] ? L : x[i];
        R = R > x[i] ? R : x[i];
        qDebug()<<"### "<<x[i]<<" "<<y[i]<<endl;
    }
    qDebug()<<"L-> "<<L<<" R->"<<R<<endl;
    for(int i = 1; i <= n; i++)
        for(int j = i; j <= n; j++)
            f[j][i] = f[j][i-1]-f[j-1][i-1];
}

Newton::~Newton(){

}

void Newton::addPoint(double X, double Y)
{
    n++;
    this->x[n] = X;
    this->f[n][0] = Y;
    for(int i = 1; i <= n; i++)
    {
        f[n][i] = ((f[n][i-1]-f[n-1][i-1])/(x[n]-x[n-i]));
    }
}

//差分形式计算
double Newton::calNewDiffer(double X)
{
    double ans = f[0][0];
    double h = (R-L)/n;
    double t = (X+(R-L)/2)/h;
    double N = 1;
    double tmp = t;
    for(int i = 1; i <= n; i++)
    {
        ans += tmp*f[i][i]/N;
        N *= (i+1);
        tmp *= (t-i);
    }
    return ans;
}

//均差形式计算
double Newton::calNew(double X)
{
    double ans = f[0][0];
    double w = 1;
    for(int i = 1; i <= n; i++)
    {
        w *= (X-x[i-1]);
        ans += f[i][i]*w;
    }
    return ans;
}

void Newton::print_f()
{
//    for(int i = 0; i <= n; i++)
//    {
//        for(int j = 0; j <= n; j++)
//            cout<<f[i][j]<<" ";
//        cout<<endl;
//    }
}

QVector<VPoint> Newton::getFunc(double hh){
    QVector<VPoint> vec;
    this->h = 1*(R-L)/hh;
    int len = (int)(hh);
    for(int i = 0; i <= len; i++){
        VPoint point(L+i*h, this->calNewDiffer(L+i*h));
        vec.push_back(point);
    }
    return vec;
}

Lagrange::Lagrange():n(0), x(nullptr), f(nullptr){
}

Lagrange::Lagrange(int n = 0, double *x = nullptr, double *f = nullptr)
{
    this->n = n;
    if(n==0)return ;
    this->x = new double[n+1];
    this->f = new double[n+1];
    for(int i = 0; i <= n; i++)
    {
        this->x[i] = x[i];
        this->f[i] = f[i];
    }
}

Lagrange::~Lagrange(){}

void Lagrange::Init(QVector<VPoint> points)
{
    this->n = points.size()-1;
    this->x = new double[n+1];
    this->f = new double[n+1];
    L = R = points[0].x;
    for(int i = 0; i <= n; i++)
    {
        this->x[i] = points[i].x;
        this->f[i] = points[i].y;
        L = L < x[i] ? L : x[i];
        R = R > x[i] ? R : x[i];
        //qDebug()<<"### "<<x[i]<<" "<<f[i]<<endl;
    }
    //qDebug()<<"L->"<<L<<" R->"<<R<<endl;
}
double Lagrange::calLag(double X)
{
    double ans = 0;
    for(int k = 0; k <= n; k++)
    {
        double l = 1.0;
        for(int i = 0; i <= n; i++)
            if(i != k)
                l *= (X-x[i])/(x[k]-x[i]);

        ans += f[k]*l;
    }
    return ans;
}

QVector<VPoint> Lagrange::getFunc(double hh){
    QVector<VPoint> vec;
    this->h = 1*(R-L)/hh;
    int len = floor(hh+0.5);
    for(int i = 0; i <= len; i++){
        VPoint point(L+i*h, this->calLag(L+i*h));
        vec.push_back(point);
    }
    return vec;
}
