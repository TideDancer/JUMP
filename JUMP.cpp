#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <iostream>
#include <fstream>
#include <string.h>

#define INF 999999999999.0            //Pseudo Infitinte number for this code
using namespace std;
long long TIMESERIES;                 //The length of the Long Time Series.
int LENGTH;                           //Length of each time series
double bsf;                           //best-so-far
    
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
    
    //////////////////////////////////////////////////////////////////////////
    t1 = clock();
    //////////////////////////////////////////////////////////////////////////
    
    // build tables:
    double* sum_table = (double*)malloc(sizeof(double)*TIMESERIES);
    for(i=0; i<TIMESERIES; i++)
        sum_table[i] = -1;
   
    long long cnt, loc1, loc2;
    cnt = 0; 
    // initialize first row of sum_table
    for(i=LENGTH; i<TIMESERIES; i++){
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

        i = 1; // pointer i: start of first block
        j = i+LENGTH-1; // pointer j: end of first block + 1
        tmp = sum_table[i+k-1] - (data[i-1]-data[i+k-1])*(data[i-1]-data[i+k-1]);

        // initialize two blocks
        while(tmp >= bsf && i<=j-2){
            tmp -= (data[i]-data[i+k])*(data[i]-data[i+k]);
            i++; 
        }
        p = i+LENGTH-1; // pointer p: start of second block 
        if(p+k > TIMESERIES-1) break;
        tmp += (data[p]-data[p+k])*(data[p]-data[p+k]); 
        q = p+1; // pointer q: end of second block + 1

        while(1){

            if(i+k > TIMESERIES-LENGTH) break;
					  else if(tmp >= bsf && i < j){
                i ++;
                tmp -= (data[i-1]-data[i+k-1])*(data[i-1]-data[i+k-1]);
						}
						else if(tmp >= bsf && i >= j){
                cnt += p - j;
                i = p;
                j = q;
                p = i+LENGTH-1;
                tmp += (data[p]-data[p+k])*(data[p]-data[p+k]); 
                q = p+1;
						}
						else if(tmp < bsf && q <= i+LENGTH-1){
                tmp += (data[q]-data[q+k])*(data[q]-data[q+k]); 
                q++;
						}
						else if(tmp < bsf && p > j){
                p--;
                tmp += (data[p]-data[p+k])*(data[p]-data[p+k]); 
						}
            
            if(p == j && q == i+LENGTH){
                if(tmp < bsf) {bsf = tmp; loc1 = i; loc2 = i+k;}
                tmp -= (data[i]-data[i+k])*(data[i]-data[i+k]);
                i++; 
                p = i+LENGTH-1; // start of second block 
                j = p;
                tmp += (data[p]-data[p+k])*(data[p]-data[p+k]); 
                q = p+1; // end of second block + 1
						}
        } 
    }

    t2 = clock();
    cout<<"Closest dist is "<<bsf<<" for pair ("<<loc1<<","<<loc2<<")"<<endl;
    cout<<"Skip Number="<<cnt<<endl;
    cout<<"Skip Fraction="<<double(cnt)/(TIMESERIES*TIMESERIES/2)<<endl;
    printf("Execution Time was : %lf seconds\n",(t2-t1)/CLOCKS_PER_SEC);

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
