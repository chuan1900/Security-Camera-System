//
//  main.c
//  assignment5_0
//
//  Created by chuan1900 on 2015-11-19.
//  Copyright © 2015 chuan1900. All rights reserved.
//

#include "SAT.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int v_num, e_num;
int arrEdge[500][2];
int arrEdge2[500][2];
int arrEdge3[500][2];
int a_vc1[51], a_vc2[51], sat_output[51];


void *fourClause(void *unused){
    int p[2], bak;
    int i=0, j=0, k=1, index = 1;
    int temp = 0, count_s = 0;
    int nk[v_num][v_num], c[2*v_num];
    //int n=v_num;
    memset(sat_output,-1,sizeof(sat_output));
    
    while (k <= v_num){
        
        //int nk[n][n], c[2*n];
        
        pipe(p);
        bak = dup(STDOUT_FILENO);
        dup2(p[1], STDOUT_FILENO);
        
        SAT_Manager mgr = SAT_InitManager();
        
        SAT_SetNumVariables(mgr, v_num*k); //the meaning of Variables?
        
        
        //to number the nk "matrix"
        index = 1;
        for (i = 0; i < v_num; i++){
            for (j = 0; j < k; j++){
                nk[i][j] = index;
                index++;
            }
        }
        
        //the first clause
        for (i = 0; i < k; i++){
            for (j = 0; j < v_num; j++){
                c[j] = (nk[j][i] << 1);
            }
            SAT_AddClause(mgr, c, v_num);
        }
        
        
        //the second clause
        int t;
        if (k>1){
            for (i = 0; i < v_num; i++){
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
            for(j = 0; j < v_num; j++){
                for(t = j+1; t < v_num; t++){
                    c[0] = (nk[j][i] << 1) + 1;
                    c[1] = (nk[t][i] << 1) + 1;
                    SAT_AddClause(mgr, c, 2);
                }
            }
        }
        
        //the fourth clause
        for (i = 0; i < e_num; i++){
            for(j = 0; j < k; j++){
                c[2*j]=(nk[arrEdge3[i][0]][j] << 1);
                c[2*j+1]=(nk[arrEdge3[i][1]][j] << 1);
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
                    {
                        temp = i/(k-1);
                    }
                    else{
                        temp = i/(k-1)-1;
                    }
                    sat_output[count_s] = temp;
                    count_s++;
                }
            }
            break;
        }
        else if (result != SATISFIABLE && k==v_num)
        {
            printf("unsat.\n"); fflush(stdout);
        }
        
    }
    fflush(stdout);
    
    return NULL;
}


void *approxVC1(void *unused){
    int flag = 0, i, j, count_a1 = 0;
    int v_cal[v_num];
    int v_cal0[v_num];

    memset(v_cal,0,sizeof(v_cal));
    memset(a_vc1,-1,sizeof(a_vc1));
    
    for (i=0; i < v_num; i++){
        v_cal0[i] = i;
    }
    
    while (flag == 0){
        int temp_big = 0, temp_big0 = 0;
        int del_num = 0;
        
        //to calculate degree of each edge
        for (i=0; i < e_num; i++){
            if ( arrEdge[i][0]!= -1 && arrEdge[i][1] != -1){
                v_cal[arrEdge[i][0]]++;
                v_cal[arrEdge[i][1]]++;
            }
        }
        
        //to rank the highest degree edge
        for (i=0; i < v_num; i++){
            for (j = i+1; j < v_num; j++){
                if (v_cal[i] < v_cal[j]){
                    temp_big = v_cal[j];
                    v_cal[j] = v_cal[i];
                    v_cal[i] = temp_big;
                    
                    temp_big0 = v_cal0[j];
                    v_cal0[j] = v_cal0[i];
                    v_cal0[i] = temp_big0;
                    
                }
            }
        }
        
        a_vc1[count_a1] = v_cal0[0];
        count_a1++;
        
        //to delete the related edges of the above vertex
        for (i=0; i < e_num; i++){
            if (arrEdge[i][0] == v_cal0[0] || arrEdge[i][1] == v_cal0[0]){
                arrEdge[i][0] = -1;
                arrEdge[i][1] = -1;
            }
        }
        
        memset(v_cal,0,sizeof(v_cal));
        for (i=0; i < v_num; i++){
            v_cal0[i] = i;
        }
        
        //whether edges all deleted or not, yes->flag = 1, no->flag = 0
        for (i=0; i < e_num; i++){
            if (arrEdge[i][0]== -1 && arrEdge[i][1] == -1){
                del_num++;
            }
        }
        if (del_num == e_num){
            flag = 1;
        }
        
    }
    
    //sort a_vc1[]
    int temps = 0;
    for (i=0; i < v_num; i++){
        for (j = i+1; j < v_num; j++){
            if (a_vc1[j] != -1){
                if (a_vc1[i] > a_vc1[j]){
                    temps = a_vc1[j];
                    a_vc1[j] = a_vc1[i];
                    a_vc1[i] = temps;
                }
            }
        }
    }
    
    
    return NULL;
    
}

void *approxVC2(void *unused){
    
    int i, j, count_vc2 = 0;
    memset(a_vc2,-1,sizeof(a_vc2));
    
    for (i=0; i < e_num; i++){
        if (arrEdge2[i][0] != -1 && arrEdge2[i][1] != -1){
            for (j=i+1; j < e_num; j++){
                if (arrEdge2[j][0] == arrEdge2[i][0] || arrEdge2[j][0] == arrEdge2[i][1] || arrEdge2[j][1] == arrEdge2[i][0] || arrEdge2[j][1] == arrEdge2[i][1]){
                    arrEdge2[j][0] = -1;
                    arrEdge2[j][1] = -1;
                }
            }
            a_vc2[count_vc2] = arrEdge2[i][0];
            a_vc2[count_vc2+1] = arrEdge2[i][1];
            count_vc2 = count_vc2 + 2;
            arrEdge2[i][0] = -1;
            arrEdge2[i][1] = -1;
        }
    }
    
    //sort a_vc2[]
    int temps = 0;
    for (i=0; i < v_num; i++){
        for (j = i+1; j < v_num; j++){
            if (a_vc2[j] != -1){
                if (a_vc2[i] > a_vc2[j]){
                    temps = a_vc2[j];
                    a_vc2[j] = a_vc2[i];
                    a_vc2[i] = temps;
                }
            }
        }
    }
    
    
    return NULL;
}

/*
 * IO function, print three output
 *
 */

void *ioFunc(void *unused){
    
    int i;
    printf("CNF-SAT-VC: ");
    for (i=0; i < v_num; i++){
        if (sat_output[i] != -1 && sat_output[i+1] != -1){
            printf("%d,",sat_output[i]);
        }
        else if (sat_output[i] != -1 && sat_output[i+1] == -1){
            printf("%d",sat_output[i]);
            break;
        }
    }
    printf("\n");
    fflush(stdout);
    
    
    printf("APPROX-VC-1: ");
    for (i=0; i < v_num; i++){
        if (a_vc1[i] != -1 && a_vc1[i+1] != -1){
            printf("%d,",a_vc1[i]);
        }
        else if (a_vc1[i] != -1 && a_vc1[i+1] == -1){
            printf("%d",a_vc1[i]);
            break;
        }
    }
    printf("\n");
    fflush(stdout);
    
    
    printf("APPROX-VC-2: ");
    for (i=0; i < v_num; i++){
        if (a_vc2[i] != -1 && a_vc2[i+1] != -1){
            printf("%d,",a_vc2[i]);
            fflush(stdout);
        }
        else if (a_vc2[i] != -1 && a_vc2[i+1] == -1){
            printf("%d\n",a_vc2[i]);
            fflush(stdout);
            break;
        }
    }
    fflush(stdout);
    
    return NULL;
}


int main(int argc, const char * argv[]) {
    
    int read;
    pthread_t thread_SAT, thread_APP1, thread_APP2,thread_out;
    size_t len=100;
    char *s=NULL;
    /*if((s=malloc((len+1)))==NULL){
     printf("Can't get memory\n");
     exit(-1);
     }*/
    
    while((read=getline(&s,&len,stdin))!=-1){
        int flag=0;
        char *p;
        int x[300];
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
                arrEdge[count1][0]=x[j];
                arrEdge[count1][1]=x[j+1];
                arrEdge2[count1][0]=x[j];
                arrEdge2[count1][1]=x[j+1];
                arrEdge3[count1][0]=x[j];
                arrEdge3[count1][1]=x[j+1];
                
                if(arrEdge[count1][0]>v_num||arrEdge[count1][1]>v_num||arrEdge[count1][0]==arrEdge[count1][1]){
                    fprintf(stderr,"Error:Input is invalid.\n");
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
                pthread_create(&thread_SAT,NULL,&fourClause,NULL);
                pthread_create(&thread_APP1,NULL,&approxVC1,NULL);
                pthread_create(&thread_APP2,NULL,&approxVC2,NULL);
                
                pthread_join(thread_SAT,NULL);
                pthread_join(thread_APP2,NULL);
                pthread_join(thread_APP1,NULL);
                
                pthread_create(&thread_out,NULL,&ioFunc,NULL);
                pthread_join(thread_out,NULL);
            }
        }
        
    }
    
    return 0;
}
