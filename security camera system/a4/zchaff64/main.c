//  main.c
//  a4-ece650
//
//  Created by 李亚梦 on 15/11/13.
//  Copyright © 2015年 Yameng Li. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "SAT.h"

// struct definition
typedef struct edge{
    int fo;          // former
    int la;          // later
    struct edge * next;
}edge_t, *edge_p;

typedef struct list{
    int edgenum;
    edge_t * head;
}list_t,*list_p;

// Function to create an edge
edge_p createEdge(int x, int y)
{
    edge_p newEdge = (edge_p)malloc(sizeof(edge_t));
    if(!newEdge)
    {
        fprintf(stderr, "Unable to allocate memory for new edge");
    }
    newEdge ->fo = x;
    newEdge ->la = y;
    newEdge ->next = NULL;
    
    return newEdge;
}

// Function to add edge
void addEdge(list_t *list, int x, int y)
{
    edge_p newEdge = createEdge(x, y);
    newEdge ->next = list->head;
    list->head = newEdge;
    list->edgenum++;
}

// destroy the list
void destroyList(list_p list)
{
    if (list)
    {
        edge_p listp = list->head;
        while (listp)
        {
            edge_p temp = listp;
            listp = listp -> next;
            free(temp);
        }
        free(list);
    }
}

// function to creat an empty list
list_p createList()
{
    list_p list = (list_p)malloc(sizeof(list_t));
    if (!list)
    {
        fprintf(stderr, "Unable to allocate memory for list\n");
    }
    list->edgenum = 0;
    list->head = NULL;
    return list;
}

// function to find the satisfication
int findVerCov(list_p list, int k, int ver_num)
// Use the value of the k to find the vertex cover
{
    int i = 0;                      // index of the node
    int j = 1;                      // the colum number
    int next = 0;                   // the next node
    int variable = 0 ;
    int flag = 0;
    
    edge_p pointer = list->head;    // the pointer to the list
    
    variable = k * ver_num;         // The total variables
    
    int p[2], bak;
    pipe(p);
    bak = dup(STDOUT_FILENO);
    dup2(p[1],STDOUT_FILENO);
    
    SAT_Manager mgr = SAT_InitManager();
    
    SAT_SetNumVariables(mgr, variable);
    
    int c[ver_num * 2];            // Clasue array
    
    // Ensure every colum have at least one true
    for (j = 1; j < k+1; j++)                       // begin with the colum
    {
        for (i = 0; i < ver_num; i++)               // each node in the colum
        {
            c[i] = ((i * k + j) << 1);                 // The variable sequence
        }
        SAT_AddClause(mgr, c, ver_num);
    }
    
    // for every line, no node be at two position at the same time
    
    for (i = 0; i < ver_num; i++)                   // for each line
    {
        for (j = 1; j < k; j++)                     // for every node from one the the second last
        {
            for (next = j+1; next < k+1; next++)    // the node behind the former node
            {
                c[0] = ((i * k + j) << 1) + 1;
                c[1] = ((i * k + next) << 1) + 1;
                SAT_AddClause(mgr, c, 2);
            }
        }
    }
    
    // for every colum, no two position is owned by two nodes at the same time
    for (j = 1; j < k + 1; j++)                       // every colum
    {
        for (i = 0; i < ver_num - 1; i++)
        {
            for (next = i+1; next < ver_num; next++)
            {
                c[0] = ((i * k + j) << 1) + 1;
                c[1] = ((next * k + j) << 1) + 1;
                SAT_AddClause(mgr, c, 2);
            }
        }
    }
    
    // check every edge at least have one node in vertex cover
    while (pointer)       // when the list is not Null
    {
        for (j = 1; j < k + 1; j++)           // for the former point
        {
            c[j - 1] = (((pointer->fo) * k + j) << 1);
        }
        for (j = 1; j < k + 1; j++)           // for the later point
        {
            c[j - 1 + k] =(((pointer->la) * k + j) << 1);
        }
        SAT_AddClause(mgr, c, 2*k);
        pointer = pointer->next;
    }
    
    // get the result
    int result = SAT_Solve(mgr);
    dup2(bak, STDOUT_FILENO);
    if (result == SATISFIABLE)
    {
        flag = 1;                               // find the answer signal
        
        int n = SAT_NumVariables(mgr);
        for(i = 1; i <= n; i++)
        {
            int a = SAT_GetVarAsgnment(mgr, i);
            if (a == 1)                            // only output the true one
            {
                printf("%d ",(i - 1)/k );
                fflush(stdout);
            }
        } // find all the true
        printf("\n");
        fflush(stdout);
    } // if result satisfiable
    else
    {
        flag = 0;                                // not find tha answer
    }
    return flag;
}


int main()
{
    int command;
    int vertex_num = 0;
    int former = 0;     // the edge former node index
    int later = 0;      // the edge later node index
    int k = 0;          // the number of nodes in the vertex cover
    int stop = 0;       // if stop = 1, find the answer ,quit finding
    
    // initiate the list
    list_p list = createList();
    
    // Read the V and E
    while((command = getchar()) != EOF)
    {
        switch (command)
        {
            case 'V':
            case 'v':
                destroyList(list);
                list = createList();
                scanf(" %d", &vertex_num);
                //fprintf(stdout, "The V : %d\n", vertex_num);
                while ((command = getchar()) == '\n')
                {
                    break;
                }
                break;
                
            case 'E':
            case 'e':
                //fprintf(stdout, "E ");
                while ((command = getchar()) != '\n')
                {
                    if (command == '<')
                    {
                        scanf("%d,%d>", &former, &later);
                        if (former >= vertex_num || later >= vertex_num)
                        {
                            fprintf(stderr, "The index of number is out of bound\n");
                            return 0;
                        }
                        else
                        {
                            addEdge(list, former, later);
                        }
                    }  // if
                }   // while
                for (k = 1; k <= vertex_num; k++)          // The number of the vertex cover
                {
                    stop = findVerCov(list, k, vertex_num);
                    if (stop == 1)
                    {
                        break;
                    }
                }
                break;
                
            default:
                fprintf(stderr, "No such command\n");
                break;
        }
    }
    return 0;
}