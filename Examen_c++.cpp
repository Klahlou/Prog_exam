#include <iostream> 
#include <cmath> 
#include <cstdlib> 
#include <time.h> 

using namespace std;

// Functions to calculate the price of the bond with respect with the given rate r


// First case : zero-coupon bond (c=0)

double Pzc(double FV, double d, int N, double r) 
{   
    double PV1;
    PV1=FV/pow(1+r,d+N);
    return PV1;
}

// second case : coupon payment day is today (d=0)

double Pd(double FV, double c,int N, double r) 
{
    double PV1;
    PV1=(c*pow(1+r,N+1)-c+r*FV)/(r*pow(1+r,N));
    return PV1;
}

// third case : bond’s price is equal to its par (face) value

double PFV(double c, double d, int N, double r) 
{
    double PV1;
    PV1=(c/r)*((pow(1+r,N+1)-1)/(pow(1+r,N+d)-1));
    return PV1;
}

// General case 

double P(double FV, double d,double c,int N,double r) 
{
    double PV1;
    PV1=(c/r)*pow(1+r,1-d)+(FV-c/r)/pow(1+r,N+d);       
    return PV1;
}

//target function, for which we will search for a zero value

double f(double r,double c, double d, double PV, double FV, int N) // Formula of the target function of which we are searching the 0 value (Newton method)
{   
    double TF;
    TF=(c/r)*pow(1+r,1-d)+(FV-c/r)/pow(1+r,N+d)-PV ;
    return TF;
}


// YTM 
double YTM(double c, double d, double FV, double PV, int N) {

    double r0,r1, PV1;
//r0 = approximate YTM * 0.95 
//r1 = approximate YTM
//starting value in iteration algorithm
    c=c*FV/100; 
    r1=(c+(FV-PV)/(N+d))/((FV+PV)/2); // Approximate YTM
    r0=0.95*r1;

    if (c==0)
    {
        while (fabs(PV1-PV)/PV1 > 0.0001 ) //the relative error should be below 0.01%
        {
            r1-=f(r1,c,d,PV,FV,N)*(r1-r0)/(f(r1,c,d,PV,FV,N)-f(r0,c,d,PV,FV,N)); // calculation of the new approximated rate with respect to the Newton method                                
            PV1=Pzc(FV,d,N,r1); // calculation of the new approximated price of the bond
        }
    }
    else if (d==0)
    { 
        while (fabs(PV1-PV)/PV1 > 0.0001 )
        {
            r1-=f(r1,c,d,PV,FV,N)*(r1-r0)/(f(r1,c,d,PV,FV,N)-f(r0,c,d,PV,FV,N));
            PV1=Pd(FV,c,N,r1);
        } 
    }
    else if (PV==FV)
    { 
        while (fabs(PV1-PV)/PV1 > 0.0001 )
        {
            r1-=f(r1,c,d,PV,FV,N)*(r1-r0)/(f(r1,c,d,PV,FV,N)-f(r0,c,d,PV,FV,N));                 
            PV1=PFV(c,d,N,r1);
        }             
    }
    else
    {
        while (fabs(PV1-PV)/PV1 > 0.0001 )
        {
            r1-=f(r1,c,d,PV,FV,N)*(r1-r0)/(f(r1,c,d,PV,FV,N)-f(r0,c,d,PV,FV,N));
            PV1=P(FV,d,c,N,r1);
        }
    }

    //Result
    return r1;
    
}

int main()
{
    
    double c, d, FV, PV;
    int N;
    double r;
    
    // ask user the data of the bond
    cout << "provide:" << endl;
    cout << "-> the coupon rate" << endl;
    cin >> c ;

    //the coupon rate should be between 0 and 100
    if (c >= 100 || c <= 0) {  
        cout << "Coupon rate should be between 0 and 100!" << endl;
        cin >> c;
    }
     
    cout << "-> the year fraction until next coupon payment:" << endl;
    cin >> d;  

    // d should be between 0 and 1 
  
    if (d > 1 || d <= 0) {
        cout << "year fraction should be between 0 and 1!" << endl;
        cin >> d;
    }
    
      
    cout << "-> the face value of the bond:" << endl;
    cin >> FV;
    // FV should be positive 
    if (FV <= 0) {
        cout << "Face value should be positive!" << endl;
        cin >> FV;
    }
    
     
    cout << "-> the price of the bond:" << endl;
    cin>> PV;

    // PV should be positive
    if (PV <= 0) {
        cout << "Price of the bond should be positive!" << endl;
        cin >> PV;
    }
     
    cout << "-> the number of full years until maturity:" << endl;
    cin >> N;
    // N should be positive 
    if (N <= 0) {
        cout << "Number of full years should be positive!" << endl;
        cin >> N;
    }
    
    r = YTM(c, d, FV, PV, N);
    cout << "The Yield to maturity for these arguments is " << r << " (equivalently " << r*100 << "% )" << endl;
}
