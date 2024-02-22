//use fix pointer to find roots of f(x)
#include <iostream>
#include <cmath>
#define Pi 3.14159265358979323846264
#define e 2.7182818284590452353602874713527
using namespace std;

double fixpoint_f11(double x) {
  //x+2sin(pi*x)=0
  //-x/2=sin(pi*x)=cos(pi*x-pi/2)
  //x=asin(-x/2)/pi+2=acos(-x/2)/pi+1/2

  double f11=asin(-x/2)/Pi+2;
   return f11  ;
}

double fixpoint_f12(double x) {
  //x+2sin(pi*x)=0
  //-x/2=sin(pi*x)=cos(pi*x-pi/2)
  //x=asin(-x/2)/pi+2=acos(-x/2)/pi+1/2

  double f12=acos(-x/2)/Pi+1.0/2;
   return f12  ;
}
double fixpoint_f21(double x) {
  //3x^2-e^x=0
  //x=sqrt(e^x/3)
  //x=-sqrt(e^x/3)
  //x=ln(e^x/3)/2

  double f21=sqrt(pow(e,x)/3);
   return f21  ;
}

double fixpoint_f22(double x){
    double f22=-sqrt(pow(e,x)/3);
    return f22;
}

double fixpoint_f23(double x) {
  double f23=log(3*x*x);
   return f23  ;
}

void fixpoint_Prob1() {
  double x1 = 0;
  double x0 = 1.0;
  int t=0;
  double err;
  do
  {
    t++;
    x1 = fixpoint_f11(x0);
    err=abs(x1-x0);
    x0=x1;
  }while (err > 1e-5);
  cout <<"fixpoint_problem1:x+2sin(¦Ðx) = 0"<<endl<< "one root is x1 = " ;
  printf("%.7f",x1);

  x0=1.0;
    do
  {
    t++;
    x1 = fixpoint_f12(x0);
    err=abs(x1-x0);
    x0=x1;
  }while (err > 1e-5);
  cout << "   another root is x2 = " ;
  printf("%.7f\n",x1);
  printf("-----------------------------------------------------------------------------------------------------\n");

}

void fixpoint_Prob2() {
  double x1 = 0;
  double x0 = 0;
  int t=0;
  double err;
  do
  {
    t++;
    x1 = fixpoint_f21(x0);
    err=abs(x1-x0);
    x0=x1;
  }while (err > 1e-5);
  cout <<"fixpoint_problem2:3x^2-e^x = 0"<<endl<< "one root is x1 = " ;
  printf("%.7f",x1);

  x0=0.0;
    do
  {
    t++;
    x1 = fixpoint_f22(x0);
    err=abs(x1-x0);
    x0=x1;
  }while (err > 1e-5);
  cout << " the second root is x2 = " ;
  printf("%.7f",x1);

    x0=3.0;
    do
  {
    t++;
    x1 = fixpoint_f23(x0);
    err=abs(x1-x0);
    x0=x1;
  }while (err > 1e-5);
  cout << " the third root is x3 = " ;
  printf("%.7f\n",x1);



}

double bisection_f1(double x){
    double f1=exp(x)-x*x+3*x-2;
    return f1;
}

double bisection_f2(double x){
    double f2=x*cos(x)-2*x*x+3*x-1;
    return f2;
}

void bisection_prob1(){
    double a=0.0;
    double b=1.0;
    double c;
    double err;
    int t=0;
    do{
        t++;
        c=(a+b)/2;
        if(bisection_f1(a)*bisection_f1(c)<0){
            b=c;
        }
        else{
            a=c;
        }
        err=abs(b-a);
    }while(err>1e-5);
    cout<<"bisection_problem1:exp(x)-x^2+3x-2=0"<<endl<<" one root is x1 = ";
    printf("%.7f\n",c);
    printf("-----------------------------------------------------------------------------------------------------\n");

}

void bisection_prob2(){
    double a=0.2;
    double b=0.3;
    double c;
    double err;
    int t=0;
    do{
        t++;
        c=(a+b)/2;
        if(bisection_f2(a)*bisection_f2(c)<0){
            b=c;
        }
        else{
            a=c;
        }
        err=abs(b-a);
    }while(err>1e-5);
    cout<<"bisection_problem2:x*cos(x)-2x^2+3x-1=0"<<endl<<" one root is x1 = ";
    printf("%.7f",c);

    a=1.2;
    b=1.3;
    do{
        t++;
        c=(a+b)/2;
        if(bisection_f2(a)*bisection_f2(c)<0){
            b=c;
        }
        else{
            a=c;
        }
        err=abs(b-a);
    }while(err>1e-5);
    cout<<" the second root is x2 = ";
    printf("%.7f\n",c);
    printf("-----------------------------------------------------------------------------------------------------\n");

}
int main() {
  bisection_prob1();
  bisection_prob2();
  fixpoint_Prob1();
  fixpoint_Prob2();

  system("pause");
  return 0;
  
}