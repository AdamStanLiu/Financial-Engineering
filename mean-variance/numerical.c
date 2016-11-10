#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <math.h>

void numerical(int m, double **Q, double **bounds, double *mean, double lambda, double *x)
{
    int h,k,i,j,iter;
    double sum=0,value=0,compare=0,a,b,e,eplus,eminus;
    for(i=0;i<=m-1;i++){
        if(bounds[i][0]>bounds[i][1]){
            printf("Upper bounds should be greater than lower bounds.\n"); return;
        }

            x[i]=bounds[i][0];
        sum=sum+x[i];
    }
    
    if(sum>1){
        printf("The problem is infeasible (no solutions exist).\n"); return;
    }
    else if(sum<1){
        for(i=0;i<=m-1;i++){
            if((sum+bounds[i][1]-bounds[i][0])<1){
                x[i]=bounds[i][1];
                sum=sum+bounds[i][1]-bounds[i][0];
            }
            else if((sum+bounds[i][1]-bounds[i][0])==1){
                x[i]=bounds[i][1];
                sum=sum+bounds[i][1]-bounds[i][0];
                break;
            }
            else{
                x[i]=1-sum+bounds[i][0];
                sum=1;
                break;
            }
        }
        if(sum<1){
            printf("The problem is infeasible (no solution exist).\n"); return;
            
        }
        else{
            printf("The problem is feasible.\n");
        }
    }
    else{
        printf("The problem is feasible.\n");
    }
    
    for(h=0;h<=m-1;h++){
        value=value+lambda*Q[h][h]*x[h]*x[h]-mean[h]*x[h];
    }
    for(h=0;h<=m-1;h++){
        for(k=0;k<h;k++){
            value=value+2*lambda*Q[h][k]*x[h]*x[k];
        }
    }
    printf("The initial object value: %f\n",value);
	iter = 0;
    while(fabs(value-compare)>1e-10){
		iter++;
        compare=value;
        for(h=0;h<=m-1;h++){
            for(k=h+1;k<=m-1;k++){
                
                a=lambda*(Q[h][h]+Q[k][k]-2*Q[h][k]);
                //printf("%f\n",a);
                sum=0;
                for(i=0;i<=m-1;i++){
                    if(i!=h && i!=k){
                        sum=sum+(Q[i][k]-Q[i][h])*x[i];
                    }
                }
                b=2*lambda*(-Q[h][h]*x[h]+Q[k][k]*x[k]+Q[h][k]*(x[h]-x[k])+sum)+mean[h]-mean[k];
                if(a==0){
                    if(b>0)
                        e=-1;
                    else if (b==0)
                        e=0;
                    else
                        e=1;
                }
                else{
                    e=-b/(2*a);
                }
                if((x[h]-bounds[h][0])>=(bounds[k][1]-x[k])){
                    eplus=bounds[k][1]-x[k];
                }
                else{
                    eplus=x[h]-bounds[h][0];
                }
                if((bounds[h][1]-x[h])>=(x[k]-bounds[k][0])){
                    eminus=x[k]-bounds[k][0];
                }
                else{
                    eminus=bounds[h][1]-x[h];
                }
                if(e>=-eminus && e<=eplus){
                    x[h]=x[h]-e;
                    x[k]=x[k]+e;
                }
                else if(e<-eminus){
                    e=-eminus;
                    x[h]=x[h]-e;
                    x[k]=x[k]+e;
                }
                if(e>eplus){
                    e=eplus;
                    x[h]=x[h]-e;
                    x[k]=x[k]+e;
                }
            }
        }
        value=0;
        for(i=0;i<=m-1;i++){
            value=value+lambda*Q[i][i]*x[i]*x[i]-mean[i]*x[i];
        }
        for(i=0;i<=m-1;i++){
            for(j=0;j<i;j++){
                value=value+2*lambda*Q[i][j]*x[i]*x[j];
            }
        }
        printf("iteration%d: %f\n",iter,value);
 
    }

    printf("\nobjective value: %f\n",value);
    for(i=0;i<=m-1;i++){
        printf("x%d:%f\n",i,x[i]);
    }
}
