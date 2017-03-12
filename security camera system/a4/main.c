//
//  main.c
//  CNF0
//
//  Created by chuan1900 on 2015-11-13.
//  Copyright © 2015 chuan1900. All rights reserved.
//

#include "SAT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int fourClause(int n, int e_num, int arrEdge[][2]){
    int p[2], bak;
    int i=0, j=0, k=1, index = 1;
    int nk[n][n], c[2*n];
    //int n=v_num;
    
    while (k <= n){
        
        //int nk[n][n], c[2*n];
        
        pipe(p);
        bak = dup(STDOUT_FILENO);
        dup2(p[1], STDOUT_FILENO);
        
        SAT_Manager mgr = SAT_InitManager();
        
        SAT_SetNumVariables(mgr, n*k); //the meaning of Variables?
        
        
        //to number the nk "matrix"
        index = 1;
        for (i = 0; i < n; i++){
            for (j = 0; j < k; j++){
                nk[i][j] = index;
                index++;
            }
        }
        
        //the first clause
        for (i = 0; i < k; i++){
            for (j = 0; j < n; j++){
                c[j] = (nk[j][i] << 1);
            }
            SAT_AddClause(mgr, c, n);
        }
        
        
        //the second clause
        int t;
        if (k>1){
            for (i = 0; i < n; i++){
                for(j = 0; j < k; j++){
                    for(t = j+1; t < k; t++){
                        c[0] = (nk[i][j] << 1) + 1;
                        c[1] = (nk[i][t] << 1) + 1;
                        SAT_AddClause(mgr, c, 2);
                    }
                }
            }
        }
        
        
        //the third clause
        for (i = 0; i < k; i++){
            for(j = 0; j < n; j++){
                for(t = j+1; t < n; t++){
                    c[0] = (nk[j][i] << 1) + 1;
                    c[1] = (nk[t][i] << 1) + 1;
                    SAT_AddClause(mgr, c, 2);
                }
            }
        }
        
        //the fourth clause
        for (i = 0; i < e_num; i++){
            for(j = 0; j < k; j++){
                c[2*j]=(nk[arrEdge[i][0]][j] << 1);
                c[2*j+1]=(nk[arrEdge[i][1]][j] << 1);
            }
            SAT_AddClause(mgr, c, 2*k);
        }
        
        
        k++;
        int result = SAT_Solve(mgr);
        dup2(bak, STDOUT_FILENO);
        if(result == SATISFIABLE)
        {
            int sa = SAT_NumVariables(mgr);
            for(i = 1; i <= sa; i++)
            {
                int a = SAT_GetVarAsgnment(mgr, i);
                if(a == 1)
                {
                    if (i%(k-1)!=0)
                        printf("%d ", i/(k-1));
                    if (i%(k-1)==0)
                    {
                        printf("%d ", i/(k-1)-1);
                    }
                }
            }
            printf("\n");
            fflush(stdout);
            break;
        }
        else if (result != SATISFIABLE && k==n)
        {
            printf("unsat.\n"); fflush(stdout);
        }
        
    }
    fflush(stdout);
    
    return 0;
}


int main(int argc, const char * argv[]) {
    
    int read;
    size_t len=100;
    char *s=NULL;
    /*if((s=malloc((len+1)))==NULL){
        printf("Can't get memory\n");
        exit(-1);
    }*/
    
    while((read=getline(&s,&len,stdin))!=-1){
        int v_num,e_num;
        int flag=0;
        char *p;
        int x[300];
        int temp1[100][2];
        int count1=0;
        int i=0;
        //to extract numbers from string
        for (p = s; *(p+1) != '\0' ; p++ )
        {
            if ((*p <= '0'|| *p >= '9') && (*(p+1) <='9' && *(p+1) >= '0'))
            {
                sscanf(p+1,"%d",(x+i));
                i++;
            }
        }
        
        
        if(s[0]=='V'){
            
            v_num=x[0];
            //printf("v_num:%d\n",v_num);
            if(v_num==0){
                printf("Error:v number is invalid.\n");
                continue;
            }
        }
        else if(s[0]=='E'){
            flag=1;
            int j=0;
            for (; j < i; j++){
                //printf("%d-%d\n",j,x[j]);
                temp1[count1][0]=x[j];
                temp1[count1][1]=x[j+1];
                
                if(temp1[count1][0]>v_num||temp1[count1][1]>v_num||temp1[count1][0]==temp1[count1][1]){
                    fprintf(stderr,"Error:Input is invalid.\n");
                    //printf("Error: Vextex does not exist.\n");
                    flag=0;
                    break;
                }
                
                count1++;
                j=j+1;
            }
            e_num=j/2;
        }
        
        if (flag ==1){
            if (e_num == 0){
                printf("\n");
                fflush(stdout);
            }
            else{
                fourClause(v_num, e_num, temp1);
            }
        }
                    
    }
    
    
    return 0;
}
