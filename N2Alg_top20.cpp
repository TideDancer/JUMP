#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string.h>
#include <queue>

#define INF 999999999999.0            //Pseudo Infitinte number for this code
using namespace std;
long long TIMESERIES;                 //The length of the Long Time Series.
int LENGTH;                           //Length of each time series
double bsf;                           //best-so-far

typedef std::pair<long long, long long> idx_pair;
typedef std::pair<idx_pair, double> tup;
class Comparator_tup
{
    public:
    bool operator() (const tup& lhs, const tup&rhs) const
    { return (lhs.second < rhs.second); }
};
    
int main(  int argc , char *argv[] )
{
    double d;
    long long i, j, p, q;
    long long length;
    double t1,t2;
    double tmp, tmp_val;

    bsf = INF;
    i = 0;
    j = 0;

    length = atol(argv[2]);
    LENGTH = atoi(argv[3]);
    TIMESERIES = length-LENGTH+1;

    double* data = (double*)malloc(sizeof(double)*length);
    ifstream in(argv[1]);
    for(i=0; i<length; i++)
        in>>data[i];
    in.close();

    priority_queue<tup, std::vector<tup>, Comparator_tup> remainQ;
    int QSIZE = 20;
    
    //////////////////////////////////////////////////////////////////////////
    t1 = clock();
    //////////////////////////////////////////////////////////////////////////
    
    // build tables:
    double* sum_table = (double*)malloc(sizeof(double)*TIMESERIES);
   
    long long cnt, cnt1, loc1, loc2;
    cnt = 0; cnt1 = 0;
    // initialize first row of sum_table
    for(i=1; i<TIMESERIES; i++){
        tmp = 0.0;
        for(j=0; j<LENGTH; j++)
            tmp += (data[j]-data[j+i])*(data[j]-data[j+i]);
        sum_table[i] = tmp;
				if(tmp < bsf){
				    bsf = tmp;
            loc1 = 0; loc2 = i; 
        }
    }

    // start processing rest
		for(long long k=LENGTH; k<TIMESERIES; k++){
        i = 1; // start of first block
        tmp = sum_table[i+k-1] - (data[i-1]-data[i+k-1])*(data[i-1]-data[i+k-1]);

				while(i+k < TIMESERIES){
            tmp += (data[i+LENGTH-1]-data[i+k+LENGTH-1])*(data[i+LENGTH-1]-data[i+k+LENGTH-1]);
            // priority queue to store top values
            if(remainQ.size() < QSIZE) remainQ.push({{i, i+k}, tmp});
						else if (tmp < remainQ.top().second){
                remainQ.push({{i, i+k}, tmp});
                remainQ.pop();
						}
            if(tmp < bsf){
                bsf = tmp; loc1 = i; loc2 = i+k; 
            }
            tmp -= (data[i]-data[i+k])*(data[i]-data[i+k]);
            i++;
				}
    }

    t2 = clock();
    cout<<"Closest dist is "<<bsf<<" for pair ("<<loc1<<","<<loc2<<")"<<endl;
    cout<<"cnt is "<<cnt<<", cnt1 is "<<cnt1<<endl;
    printf("Execution Time was : %lf seconds\n",(t2-t1)/CLOCKS_PER_SEC);

    tup tup;
		while(remainQ.size()>0){
        tup = remainQ.top(); remainQ.pop();
        std::cout<<tup.second<<" for "<<tup.first.first<<","<<tup.first.second<<std::endl;
		}

    //// brute force to check correctness
    //bsf = INF;
    //for(i=0;i<TIMESERIES;i++)
    //  for(j=i+1;j<TIMESERIES;j++){
    //    tmp = 0.0;
    //    for(int k=0;k<LENGTH;k++)
    //      tmp += (data[i+k]-data[j+k])*(data[i+k]-data[j+k]);
    //    if(tmp <= bsf){
    //      bsf = tmp; loc1 = i; loc2 = j;
    //  }}
    //cout<<bsf<<endl<<loc1<<endl<<loc2<<endl;
}
