
#include"Gradient_Descent.h"


Gradient_Descent::Gradient_Descent(){
    dim = 2;
}
/**
3*x^2 + 4*y^2 + 5*x*y - 6*x + 7*y;
*/
double Gradient_Descent::f0( vec p ){
    return 3*p[0]*p[0] + 4*p[1]*p[1] + 5*p[0]*p[1]
            - 6*p[0] + 7*p[1];
}
/**
(6x+5y-6, 8y+5x+7)
*/
vec Gradient_Descent::f1( vec p ){
    double x=p[0], y=p[1];
    return vec{ 6*x+5*y-6, 8*y+5*x+7 };
}
vec Gradient_Descent::solve(){
    vec pre = vec(dim, 1);
    int iter = 0;
    double lambda = 1, error = 1;
    while ( lambda>1e-9 && error>1e-12 ){
        vec grad = f1(pre);
        vec nxt(pre);
        for (int i=0; i<dim; i++){
            nxt[i] -= grad[i]*lambda;
            ///if(nxt[i]<0) nxt[i]=0;   //Positive
        }
        double f0nxt = f0(nxt), f0pre=f0(pre);
        if ( f0nxt < f0pre ){
            pre = nxt;
            lambda *= 1.05;
            error = f0pre-f0nxt;
        }else{
            lambda *= 0.5;
        }
        ++iter;
        printf("iter=%d  (%.4f, %.4f)=%.6f lambda=%.4f error=%.8f\n",
            iter, pre[0], pre[1], f0pre, lambda, error);
    }
    printf("f(%.6f, %.6f) = %.6f\n", pre[0], pre[1], f0(pre) );
    return pre;
}
