#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include "vpoint.h"
#include <QVector>

class Newton//牛顿插值
{
private:
    static const int N = 20;
    int n;
    double x[N];
    double f[N][N];
    double h = 0.1;
public:
    double L, R;//x的左右区间范围
    Newton();
    Newton(int n, double *x, double *y);//差分形式
    ~Newton();
    void addPoint(double X, double Y);
    double calNewDiffer(double X);//差分形式计算
    double calNew(double X);//均差形式计算
    void print_f();
    QVector<VPoint> getFunc(double h);//返回区间[L, R]上的函数点对
};

#endif // INTERPOLATION_H
