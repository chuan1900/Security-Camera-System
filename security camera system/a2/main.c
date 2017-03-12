

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Queuemember{
    int value;
    struct Queuemember *next;
}*front,*rear,*cur;
void Qcreate();
void Qaddelement(int data);
void Qpopelement();
int  QTopelement();
int  Qemptyornot();


struct Stackmember{
    int value;
    struct Stackmember *next;
}*firstmember,*scur,*member;
void screate();
void saddelement(int value);
void spopelement();
int  stopelement();
int  semptyornot();



// store spots
struct spot{
    int value;
    struct spot* next;
};
//create a spots list
struct Listofedges{
    struct spot* head;
};
//create a map of edges
struct mapofedges{
    // int V;
    struct Listofedges* array;
};





//generate  a  node
struct spot* createNode(int value);
//initial a new graph of Ve vertiexs
struct mapofedges* generateamap(struct mapofedges* map,int Ve);
//generate edges
struct mapofedges* createEdge(struct mapofedges* map, int* side);
//the shortest path of BFS
struct mapofedges* BFS(struct mapofedges* map, int V, int begin, int final);

//the shortest path using BFS
struct mapofedges* BFS(struct mapofedges* map, int V, int begin, int final){
    
    int j;
    int pre[V];
    int serch[V];
    for (j = 0; j<V; j++)
    {
        serch[j] = 0;
        pre[j] = -1;
    }
    
    Qcreate();
    Qaddelement(begin);
    serch[begin] = 1;
    
    while(Qemptyornot())
    {
        int top = QTopelement();
        Qpopelement();
        
        struct spot* nextpoint = map->array[top].head;
        while(nextpoint!=NULL)
        {
            if (serch[nextpoint->value] == 0)
            {
                serch[nextpoint->value] = 1;
                pre[nextpoint->value] = top;
                
                if(nextpoint->value!=final)
                {
                    Qaddelement(nextpoint->value);}
                else
                {
                    
                    break;
                }
            }
            nextpoint = nextpoint->next;
        }
        
    }
    
    int next = final;
    if (pre[next]!=-1)
    {
        screate();
        
        while(next!=begin)
        {
            next = pre[next];
            saddelement(next);
        }
        
        while(semptyornot()==0)
        {
            fprintf(stdout, "%d-",stopelement());
            spopelement();
        }
        fprintf(stdout,"%d\n",final);
    }
    else {
        fprintf(stdout,"Error: there is no path between this two points.\n");
    }
    return map;
}




char** str_split(char* a_str, char* sperate);
int reviewVer(int V,int k);



int main() {
    char input[10000];
    int Ve = 0;
    struct mapofedges* map = NULL;
    int raw = scanf(" %10000[0-9a-zA-Z <>%{%},]c\n",input);
    
    while(raw == 1)
    {
        if(input[0] == 'V')
        {
            char* delete = " ";
            char** final;
            final = str_split(input, delete);
            char* vertice = *(final+1);
            Ve = atoi(vertice);
            map = generateamap(map, Ve);
        }
        else if (input[0] == 'E')
        {
            if (map!=NULL)
            {
                
                //map = generateamap(map, Ve);
                char* delete = " {}<>,";
                char** raw = str_split(input, delete);
                
                int i;
                char* ch;
                int ch1;
                int symbol = 1;
                for(i=1; *(raw+i)!=NULL; i++)
                {
                    ch = *(raw+i);
                    ch1 = atoi(ch);
                    if(reviewVer(Ve,ch1)==0)
                    {
                        symbol =0;
                        break;
                    }
                }
                if (symbol ==1)
                {
                    i=1;
                    
                    while(*(raw+i)!=NULL & *(raw+i+1)!=NULL)
                    {
                        char* a = *(raw+i);
                        char* b = *(raw+i+1);
                        int a1= atoi(a);
                        int b1= atoi(b);
                        
                        
                        int side[] = {a1,b1};
                        createEdge(map,side);
                        i = i+2;
                    }
                    
                    
                }
                else
                    fprintf(stdout,"Error: there is no vertice .\n");
            }
            else
            { fprintf(stdout,"Error: let's start input Vertices.\n");
            }
            
        }
        else if (input[0] == 's')
        {
            char** line;
            char* delete = " ";
            line = str_split(input, delete);
            int i;
            char* num;
            int num1;
            int symbol = 1;
            for(i=1; *(line+i)!=NULL; i++)
            {
                num = *(line+i);
                num1 = atoi(num);
                if(reviewVer(Ve,num1)==0)
                {
                    symbol =0;
                    break;
                }
            }
            if (symbol ==1)
            {
                
                char* start = *(line+1);
                int start1 = atoi(start);
                char* end = *(line+2);
                int end1 = atoi(end);
                BFS(map, Ve, start1, end1);
            }
            else
                fprintf(stdout,"Error: the vertice does not exist.\n");
        }
        
        raw = scanf(" %10000[0-9a-zA-Z <>%{%},]c\n",input);
    }
    
    
    return (EXIT_SUCCESS);
}




//create a  queue
void Qcreate(){
    front = rear = NULL;
}

//add element to queue
void Qaddelement(int value){
    if(front != NULL){
        cur = (struct Queuemember *)malloc(1*sizeof(struct Queuemember));
        rear->next = cur;
        cur->value = value;
        cur->next = NULL;
        rear = cur;
    }else{
        rear = (struct Queuemember *)malloc(1*sizeof(struct Queuemember));
        rear->next = NULL;
        rear->value = value;
        front = rear;
    }
}

// pop element in queue
void Qpopelement(){
    if(front == NULL){
        return;
    }else{
        if(front->next != NULL){
            cur = front;
            front = front->next;
            free(cur);
        }else{
            free(front);
            front = NULL;
            rear = NULL;
        }
    }
}

//get the first element
int QTopelement(){
    if((front != NULL)&&(rear != NULL)){
        return (front->value);
    }else{
        return 0;
    }
}

//wether the queue is empty or not
int Qemptyornot(){
    if ((front == NULL) && (rear == NULL))
        return 0;
    else
        return 1;
}



//create a stack
void screate(){
    firstmember = NULL;
}

//add  element to the stack
void saddelement(int data){
    if(firstmember != NULL){
        // make the current element as the top element.
        member = (struct Stackmember *)malloc(1*sizeof(struct Stackmember));
        member->value = data;
        member->next = firstmember;
        firstmember = member;
    }else{
        firstmember = (struct Stackmember *)malloc(1*sizeof(struct Stackmember));
        firstmember->value = data;
        firstmember->next = NULL;
    }
}


//pop  element from the stack
void spopelement(){
    if(firstmember == NULL){
        return;
    }else{
        scur = firstmember;
        firstmember = firstmember->next;
    }
    free(scur);
}

//get the top element
int stopelement(){
    return(firstmember->value);
}

//whether the stack is empty or not
int semptyornot(){
    if(firstmember == NULL){
        return 1;
    }else{
        return 0;
    }
}


//generate  a  node
struct spot* createNode(int value){
    struct spot* newspot = (struct spot*)malloc(sizeof(struct spot));
    newspot->value = value;
    newspot->next = NULL;
    return newspot;
}

//initial a new graph of V vertiexs
struct mapofedges* generateamap(struct mapofedges* map,int Ve){
    map = (struct mapofedges*)malloc(sizeof(struct mapofedges*));
    //map->V = Ve;
    map->array = (struct  Listofedges*)malloc((Ve) * sizeof(struct  Listofedges));
    int j;
    for(j = 0; j < Ve; ++j){
        map->array[j].head = NULL;
    }
    return map;
}


//generate edges

struct mapofedges* createEdge(struct mapofedges* map, int* side){
    int review=0;
    int aim = side[1];
    int start = side[0];
    
    struct spot* scur=map->array[start].head;
    while (scur)
    {
        if (scur->value==aim)
        { review=1;
            break;
        }else
        {
            scur=scur->next;
        }
    }
    if (review==0){
        struct spot* newspot = createNode(aim);
        newspot->next = map->array[start].head;
        map->array[start].head = newspot;
        newspot =  createNode(start);
        newspot->next = map->array[aim].head;
        map->array[aim].head = newspot;
    }
    return map;
}





//seperate numbers from symbol
char** str_split(char* a_str, char* sperate)
{
    char** final = 0;
    char** current;
    char* delete;
    int cal = 0;
    delete= strtok(a_str, sperate);
    while(delete != NULL)
    {
        
        current = realloc(final,sizeof(char*)*(cal+1));
        if ( current != NULL ) {
            final=current;
        } else {
            free(final);
        }
        *(final+cal) = strdup(delete);
        cal++;
        delete = strtok(0,sperate);
    }
    
    *(final+cal) = 0;
    
    return final;
}




//check within the range
int reviewVer(int V,int k)
{
    if (k>=V | k<0)
        return 0;
    else
        return 1;
    
}
