#include<bits/stdc++.h>
using namespace std;

const char *train_file_dir = "../../../../dataset/wine/wine_train.data";
const char *test_file_dir = "../../../../dataset/wine/wine_train.test";
vector<int> train_lable, test_lable;
vector< vector<double> > train_data, test_data;
int N, dim=13;
/**
wine.names describes the format of wine.data
*/
bool read_wine(FILE* fin,vector<int>&lable, vector< vector<double> >&data)
{
    int tmplable;
    if ( fscanf(fin, "%d", &tmplable) != 1 ) return true;
    vector<double> tmpdata(dim);
    for (int i=0; i<dim; i++){
        if ( fscanf(fin, ",%lf", &tmpdata[i] ) != 1 ) return false;
    }
    lable.push_back( tmplable );
    data.push_back(tmpdata);
    return true;
}
void output_data(){
    for (int i=0; i<N; i++){
        printf("%d", train_lable[i]);
        for (int j=0; j<dim; j++){
            printf(",%.3f", train_data[i][j]);
        }
        puts("");
    }
    printf("%d", train_lable.size());
}

class Distance_Metric_Learning_Diagonal{
    int n, d;///data size & data dimension
    vector< vector<double> > X;
    vector< pair<int,int> > S,D;
    double C;
    vector<double> A;
    vector<double> S_sqrsum;///square sum of data in S set, the first part of ||X'AX||'s gradient
    const double eps = 1e-9;
public:
    template<class T>T sqr(T x){
        return x*x;
    }
    double distance2(const vector<double>& x,const vector<double>& y,const vector<double>& A){
        double sum = 0;
        for (int i=0; i<d; i++){
            sum += sqr( x[i]-y[i] ) * A[i];
        }
        return sum;
    }
    ///sum of dist(x_u,x_v) iff (u,v) in D
    double D_sum_func(const vector<double>& A){
        double tmpsum = 0;
        for (int i=D.size()-1; i>=0; i--){
            int u=D[i].first, v=D[i].second;
            tmpsum += sqrt( distance2(X[u], X[v], A) );
        }
        return tmpsum;
    }
    double g(const vector<double>& A){
        double sum=0;
        for (int i=S.size()-1; i>=0; i--){
            int u=S[i].first, v=S[i].second;
            sum += distance2(X[u], X[v], A);
        }
        sum = sum - C*log( D_sum_func(A) );
        return sum;
    }
    vector<double> gradient(vector<double> A){
        vector<double> grad(d,0);

        for (int i=D.size()-1; i>=0; i--){
            int u=D[i].first, v=D[i].second;
            double dist = sqrt( distance2(X[u], X[v], A) );

            if (dist<eps) dist+=eps;

            for (int j=0; j<d; j++){
                grad[j] += sqr( X[u][j]-X[v][j] )/dist;
            }
        }
        double D_dist_sum = D_sum_func(A);
        assert(D_dist_sum>eps);
        for (int j=0; j<d; j++){
            grad[j] = -0.5 * C * grad[j] / D_dist_sum;
        }
        for (int j=0; j<d; j++){
            grad[j] += S_sqrsum[j];
        }
        return grad;
    }
    vector<double> solve( const vector< vector<double> >&_X, const vector< pair<int,int> >&_S, const vector< pair<int,int> >&_D, const double _C )
    {
        X=_X, S=_S, D=_D, C=_C;
        if (X.empty() || S.empty() || D.empty() ) return vector<double>(d,1);

        n = X.size();
        d = X[0].size();
        A = vector<double>(d, 1);
        S_sqrsum = vector<double>(d, 0);

        for (int i=S.size()-1; i>=0; i--){
            for (int j=0; j<d; j++){
                int u = S[i].first, v = S[i].second;
                S_sqrsum[j] += sqr(X[u][j]-X[v][j]);
            }
        }

//        auto gg = gradient(A);
//        for (int i=0; i<d; i++){
//            printf("%.7f ", gg[i]);
//        }
//        exit(0);
        double lambda = 1;
        int iter = 0;
        while(lambda>eps*eps && iter<30000){
            ++iter;
            auto grad = gradient(A);
            vector<double> tmpA(A);

            double checksum = 0;
            for (int i=0; i<d; i++){
                tmpA[i] -= lambda*grad[i];
                if ( tmpA[i]<0 ) {
                    tmpA[i]=0;
                }
                checksum += tmpA[i];
            }
            if (checksum<eps){
                lambda *= 0.5;
                continue;
            }
            double gA = g(A);
            double gtmpA = g(tmpA);
            {
                printf("pre: %.6f  new: %.6f  iter:%d lambda=%e\n", gA, gtmpA, iter,lambda );
//                char str[200];
//                puts("a:");
//                for (int i=0; i<13; i++){
//                    printf("%f ", tmpA[i]);
//                }
//                puts("");
//                puts("gradient:");
//                for (int i=0; i<13; i++){
//                    printf("%f ", grad[i]);
//                }
//                puts("");
//                puts("fd_1st_D");
//                for (int i=0; i<13; i++){
//                    printf("%f ", -(grad[i]-S_sqrsum[i]) );
//                }
//                puts("");
                //gets(str);
            }

            if ( gtmpA<gA ){
                A = tmpA;
                lambda *= 1.05;
            }else{
                lambda *= 0.5;
            }
            //printf("iterator %d, lambda=%.15f score:%.6f\n", iter, lambda, g(A) );
//            for (int i=0; i<d; i++) printf("%.4f ", grad[i]);
//            puts("");
            //break;
        }
        return A;
    }
};

int main(){
    FILE *ftrain = fopen(train_file_dir,"r");
    FILE *ftest = fopen(test_file_dir,"r");
    while( !feof(ftrain) ){
        read_wine(ftrain, train_lable, train_data);
    }

    train_lable.clear();
    train_data.clear();
    train_lable = {1,1,1,1,2,2,2,2};
    train_data = { {3,20},{3,30},{2,40},{2,50},
    {7,50},{7,60},{6,70},{6,80} };

    N = train_data.size();
    dim = train_data[0].size();
    vector< pair<int,int> > S,D;
    for (int i=0; i<N; i++){
        for (int j=i+1; j<N; j++){
            if ( train_lable[i] == train_lable[j] ){
                S.push_back( make_pair(i,j) );
            }else{
                D.push_back( make_pair(i,j) );
            }
        }
    }

    Distance_Metric_Learning_Diagonal test;
    auto A = test.solve(train_data, S, D, 1);



    for (int i=0; i<dim; i++){
        printf("%.12f\n", A[i]);
    }

//    A = {  17.1348,
//         0,
//         0,
//         0,
//         0,
//         0,
//   78.7429,
//         0,
//         0,
//    4.9630,
//         0,
//   46.8067,
//    0.0002};
//    A = {
//   9.393331359993496,
//                   0,
//                   0,
//   0.175414620486221,
//                   0,
//                   0,
//  20.149933035014229,
//                   0,
//                   0,
//   2.655630120871724,
//                   0,
//  11.704827180701225,
//   0.000109877997622
//    };
//    A = {51.943762417332515,0};
//    printf("score: %.9f\n", test.g(A) );

//    for (int i=0; i<N; i++){
//        for (int j=0; j<N; j++){
//            printf("%.6f ",test.distance2(train_data[i], train_data[j], A) );
//        }
//        puts("");
//    }

    double sum=0;
    for (int i=S.size()-1; i>=0; i--){
        int u=S[i].first, v=S[i].second;
        sum += test.distance2(train_data[u], train_data[v], A);
    }

    printf("S_sum2 = %.6f\n", sum );
    printf("D_sum = %.6f\n", test.D_sum_func(A) );
    printf("g(A) = %.6f\n", test.g(A) );
    double S_max=-1e25, D_min=1e25;
    for ( auto t:S ){
        int u=t.first, v=t.second;
        S_max = max(S_max, test.distance2(train_data[u], train_data[v], A) );
    }
    for ( auto t:D ){
        int u=t.first, v=t.second;
        D_min = min(D_min, test.distance2(train_data[u], train_data[v], A) );
        //if( test.distance2(train_data[u], train_data[v], A) < 1e-9 ) printf("(%d,%d)\n",u,v);
    }
    printf("S_max=%e  D_min=%e\n", S_max, D_min);

    for (int i=0; i<dim; i++){
        printf("%e\n", A[i]);
    }

    int u,v;
    while( scanf("%d%d", &u, &v)!=EOF ){
        printf("%.6f\n", test.distance2(train_data[u], train_data[v], A) );
    }

    fclose(ftrain);
    fclose(ftest);
    return 0;
}
