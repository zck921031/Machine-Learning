#ifndef GRADIENT_DESCENT_H_INCLUDED
#define GRADIENT_DESCENT_H_INCLUDED
#include<bits/stdc++.h>
using namespace std;
typedef vector<double> vec;
class Gradient_Descent{
public:
    int dim;
    Gradient_Descent();
    /**
    return f(p);
    */
    virtual double f0( vec p );
    /**
    1st gradient of f(p)
    */
    virtual vec f1( vec p );
    vec solve();
};

#endif // GRADIENT_DESCENT_H_INCLUDED
