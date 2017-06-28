#include "interpolation.h"
#include <cstring>

Newton::Newton():n(0)
{
    memset(f, 0, sizeof(f));   //均差形式
}

Newton::Newton(int n, double *x, double *y)//差分形式
{
    this->n = n;
    L = R = x[0];
    for(int i = 0; i <= n; i++)
    {
        this->x[i] = x[i];
        f[i][0] = y[i];
        L = L < x[i] ? L : x[i];
        R = R > x[i] ? R : x[i];
    }
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
    double h = 10/n;
    double t = (X+5)/h;
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

QVector<VPoint> Newton::getFunc(){
    QVector<VPoint> vec;
    this->h = 1;
    int len = (int)((R-L)/h);
    for(int i = 0; i <= len; i++){
        VPoint point(L+i*h, this->calNew(L+i*h));
        vec.push_back(point);
    }
    return vec;
}
