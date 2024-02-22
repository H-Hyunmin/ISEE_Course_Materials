/*
 * @Author: ZJUHuangXM hxm15726861286@gmail.com
 * @Date: 2023-10-16 15:16:57
 * @LastEditors: ZJUHuangXM hxm15726861286@gmail.com
 * @LastEditTime: 2023-10-16 15:28:14
 * @FilePath: \HW2\pro1.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE_
 */
//using Newton's method to find the root of f(x)=-x^3-cos(x)
#include<iostream>
#include<cmath>
using namespace std;
double f(double x)
{
    return -x*x*x-cos(x);
}
double df(double x)
{
    return -3*x*x+sin(x);
}



int main()
{
    double x0,x1;
    cout<<"Please input the initial value x0:"<<endl;
    cin>>x0;
    x1=x0-f(x0)/df(x0);
    int t=1;
    while(fabs(x1-x0)>1e-5)
    {
        x0=x1;
        x1=x0-f(x0)/df(x0);
        cout<<"The "<<t<<"th iteration: ";
        printf("%.10f\n",x1);
        t++;
    }
    cout<<"The root is ";
    printf("%.10f\n",x1);
    return 0;
}