#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>

int m=6;

typedef struct node
{
    int st_index;
    int x;
    int y;
    struct node* next;
}node;

//queue struct
typedef struct Queue{
    node *front;
    node *rear;
    int size;
}Queue;

//plus
struct sNode{
    int str_id;
    int x,y;
    struct sNode *next;
}Snode;
struct sNode *head=NULL;
struct sNode *curr=NULL;
int lsize=0;

//neighbor list node
struct Nearlistspot{
    int point1x,point1y;
    int point2x,point2y;
    struct Nearlistspot* next;
};


//neighbor list
struct Nearlist{
    struct Nearlistspot* head;
};


struct map{
    int V;
    struct Nearlist* array;
};



struct sNode* creatList(int x,int y,int str_id);
struct sNode* addToList(int x,int y,int str_id);
void printLinkedList();
struct Nearlistspot* newAdjListNode(int x1,int y1, int x2, int y2);
void addEdge(struct map* map, int numberofstreet, int x1,int y1, int x2, int y2);
struct map* createmap(int V);
int math_overlap(int a,int b, int c, int d,int x1,int y1, int x2, int y2);
int random_number(int min_num, int max_num);
struct Queue *CreateList();
int IsEmpty(Queue *pqueue);
void AddList(Queue *pqueue,int st_index,int x,int y);
void ListTraverse(Queue *pqueue);
int min(int x, int y);
int max(int x, int y);
int IsOverlap(int ax,int ay,int bx,int by,int cx,int cy,int dx,int dy,int m);
int CheckOverlap(Queue *pqueue,int frontx,int fronty,int c_ran_x,int c_ran_y,int s_index, int m);
int Tra(Queue *pqueue);
void OutputStreet(int s_index,int n_num, int c_num,Queue *q,int l_num, int m);


//plus
struct sNode* creatList(int x,int y,int str_id){
    struct sNode *ptr = (struct sNode*)malloc(sizeof(struct sNode));
    if(ptr==NULL){
        return NULL;
    }
    ptr->x=x;
    ptr->y=y;
    ptr->str_id=str_id;
    
    ptr->next=NULL;
    
    head=curr=ptr;
    lsize++;
    return ptr;
}
struct sNode* addToList(int x,int y,int str_id){
    if(head==NULL){
        return (creatList(x, y,str_id));
    }
    struct sNode *ptr = (struct sNode*)malloc(sizeof(struct sNode));
    if(ptr==NULL){
        return NULL;
    }
    ptr->x=x;
    ptr->y=y;
    ptr->str_id=str_id;
    ptr->next=NULL;
    
    curr->next=ptr;
    curr=ptr;
    lsize++;
    
    return ptr;
    
}
void printLinkedList(){
    struct sNode* ptr = head;
    printf("LinkedList:");
    while (ptr != NULL){
        printf("[%d,%d,ID:%d]- ", ptr->x, ptr->y, ptr->str_id);
        ptr = ptr->next;
    }
}
//initial a new neighbor list node
struct Nearlistspot* newAdjListNode(int x1,int y1, int x2, int y2){
    struct Nearlistspot* newNode = (struct Nearlistspot*)malloc(sizeof(struct Nearlistspot));
    newNode->point1x = x1;
    newNode->point1y = y1;
    newNode-> point2x = x2;
    newNode->point2y = y2;
    newNode->next = NULL;
    return newNode;
}

//initial a new graph of V vertiexs
struct map* createmap(int V){
    struct map* map = (struct map*)malloc(sizeof(struct map));
    map->V = V;
    map->array = (struct Nearlist*)malloc(V * sizeof(struct Nearlist));
    int j;
    for(j = 0; j < V; ++j){
        map->array[j].head = NULL;
    }
    return map;
}

//add edges
void addEdge(struct map* map, int numberofstreet, int x1,int y1, int x2, int y2){
    
    //printf("I am in addEdge\n");
    struct Nearlistspot* newNode = newAdjListNode(x1, y1, x2, y2);
    newNode->next = map->array[numberofstreet].head;
    map->array[numberofstreet].head = newNode;
}
int math_overlap(int a,int b, int c, int d,int x1,int y1, int x2, int y2){
    
    //printf("I am in math_overlap\n");
    int result = 1;
    double k1;
    double b1;
    double k2;
    double b2;
    if((a == c) || (x1 == x2)){
        if((a == c)&&(x1 == x2)){
            if((((b >= y1)&&(d <= y1))||((b <= y1)&&(d >= y1))) || (((b >= y2)&&(d <= y2))||((b <= y2)&&(d >= y2)))){
                result = 0;
            }
        }
    }else{
        k1 = (b - d)/(a - c);
        b1 = b - k1 * a;
        
        k2 = (y1 - y2)/(x1 - x2);
        b2 = y1 - k2 * x1;
        
        if((k1 == k2) && (b1 == b2)){
            if(((((b >= y1)&&(d <= y1))||((b <= y1)&&(d >= y1))) || (((b >= y2)&&(d <= y2))||((b <= y2)&&(d >= y2))))||((((a >= x1)&&(c <= x1))||((a <= x1)&&(c >= x1))) || (((a >= x2)&&(c <= x2))||((a <= x2)&&(c >= x2))))){
                result = 0;
            }
        }
    }
    
    return result;
}





int random_number(int min,int max)
{
    static int dev_random_fd=-1;
    char* next_random_byte;
    int bytes_to_read;
    unsigned random_value;
    
    assert(max>min);
    
    if (dev_random_fd==-1) {
        dev_random_fd=open("/dev/urandom", O_RDONLY);
        assert(dev_random_fd!=-1);
    }
    
    next_random_byte=(char*)&random_value;
    bytes_to_read=sizeof(random_value);
    do{
        int bytes_read;
        bytes_read=read(dev_random_fd, next_random_byte, bytes_to_read);
        bytes_to_read-=bytes_read;
        next_random_byte+=bytes_read;
    }while (bytes_to_read>0);
    return min+(random_value%(max-min+1));
}



//create a new LinkedList
Queue *CreateList()
{
    Queue *pqueue=(Queue *)malloc(sizeof(Queue));
    pqueue->front=NULL;
    pqueue->rear=NULL;
    pqueue->size=0;
    return pqueue;
}

//queue is empty or not?
int IsEmpty(Queue *pqueue)
{
    if (pqueue->size==0)
        return 1;
    else
        return 0;
}


void AddList(Queue *pqueue,int st_index,int x,int y)
{
    node *P=(node *)malloc(sizeof(node));
    P->st_index=st_index;
    P->x=x;
    P->y=y;
    P->next=NULL;
    if (IsEmpty(pqueue)) {
        pqueue->front=P;
        pqueue->rear=P;
        pqueue->size++;
    }
    else {
        pqueue->rear->next=P;
        pqueue->rear=P;
        pqueue->size++;
    }
}

void ListTraverse(Queue *pqueue)
{

    node *p;
    p=pqueue->front;
    //printf("Elements of the Queue:\n");
    while(p!=NULL){
        p=p->next;
    }
    //printf("\n");
    
}

int min(int x, int y){
    if (x>=y) {
        return y;
    }else return x;
}

int max(int x, int y){
    if (x>=y) {
        return x;
    }else return y;
}


int IsOverlap(int ax,int ay,int bx,int by,int cx,int cy,int dx,int dy, int m){
    int ret=0;
    if (ay==by&&cy==dy&&ay==cy) {
        if ((cx>min(ax,bx)&&cx<max(ax,bx))||
            (dx>min(ax,bx)&&dx<max(ax,bx))||
            (ax>min(cx,dx)&&ax<max(cx,dx))||
            (bx>min(cx,dx)&&bx<max(cx,dx))||
            (ax==cx&&bx==dx)||
            (ax==dx&&bx==cx)) {
            //printf("both are level,overlap\n");
            ret=1;//1 means overlap
        }
    }
    else if (ax==bx&&cx==dx&&ax==cx){
        if ((cy>min(ay,by)&&cy<max(ay,by))||
            (dy>min(ay,by)&&dy<max(ay,by))||
            (ay>min(cy,dy)&&ay<max(cy,dy))||
            (by>min(cy,dy)&&by<max(cy,dy))||
            (ay==cy&&by==dy)||
            (ay==dy&&by==cy)) {
            //printf("both are vertical,overlap\n");
            ret=1;//1 means overlap
        }
    }else if ((ax!=bx)&&(cx!=dx)&&(ay!=by)&&(cy!=dy)){
        if ((ay-by)/(ax-bx)-(cy-dy)/(cx-dx)==0){
            if (((ay-by)*(cx-bx)/(ax-bx)+by==cy&&cy>min(ay,by)&&cy<max(ay,by))||
                ((ay-by)*(dx-bx)/(ax-bx)+by==dy&&dy>min(ay,by)&&dy<max(ay,by))||
                ((cy-dy)*(ax-dx)/(cx-dx)+dy==ay&&ay>min(cy,dy)&&ay<max(cy,dy))||
                ((cy-dy)*(bx-dx)/(cx-dx)+dy==by&&by>min(cy,dy)&&by<max(cy,dy))||
                (ax==cx&&ay==cy&&bx==dx&&by==dy)||
                (ax==dx&&ay==dy&&bx==cx&&by==cy)) {
                //printf("both are oblique,overlap\n");
                ret=1;//1 means overlap
            }
        }
    }
    
    return ret;
}




int CheckOverlap(Queue *pqueue,int frontx,int fronty,int c_ran_x,int c_ran_y,int s_index,int m)
{
    //c点=(previousx,previousy) d点=(c_ran_x,c_ran_y)
    //a点=(ax,ay) b点=(bx,by)
    int cx=frontx;
    int cy=fronty;
    int dx=c_ran_x;
    int dy=c_ran_y;
    
    int ret=0;//0表示没有overlap
    int ax,ay,bx,by;
    
    node *p;
    p=pqueue->front;
    
    
    
    ListTraverse(pqueue);
    
    while(p->next!=NULL)
    {//遍历
        
        if (p->st_index==p->next->st_index) {
            
            ax=p->x;
            ay=p->y;
            bx=p->next->x;
            by=p->next->y;
            ret=IsOverlap(ax,ay,bx,by,cx,cy,dx,dy,m);
            if (ret==1) {
                break;
            }
        }
        //printf("\n");
        p=p->next;
    }
    
    
    return ret;
}

int Tra(Queue *pqueue)
{
    //c点=(previousx,previousy) d点=(c_ran_x,c_ran_y)
    //a点=(ax,ay) b点=(bx,by)
    
    int ret=0;//0表示没有overlap
    int ax,ay,bx,by;
    
    node *p;
    p=pqueue->front;
    
    
    
    while(p->next!=NULL)
    {//遍历
        
        if (p->st_index==p->next->st_index) {
            
            
            //printf("p->st_index=%d p->next->st_index=%d",p->st_index,p->next->st_index);
            ax=p->x;
            ay=p->y;
            bx=p->next->x;
            by=p->next->y;
            //printf("a点=(%d,%d),b点=(%d,%d)\n",ax,ay,bx,by);
            
            
        }
        //printf("\n");
        p=p->next;
    }
    
    
    return ret;
}


void OutputStreet(int s_index,int n_num, int c_num,Queue *q,int l_num,int m)
{
    
    //printf("在走生成街道的函数\n");
    int n_ran;
    int n_index=0;
    char a[100]="a";
    char yinhao[2]={'"'};
    char stname[8];
    
    int previousx=c_num+1;
    int previousy=c_num+1;
    
    //printf("n_ran=%d\n",n_ran);
    if (n_num==1) {
     n_ran=1;
     }else{
     n_ran=random_number(0,n_num-1);
     if (n_ran>=0) {
     n_ran=n_ran+1;
     }
     if (n_ran<0) {
     n_ran=-n_ran+1;
     }
     }
    
    
    
    sprintf(stname, "%d", s_index);
    strcat(a,yinhao);//a=a"
    strcat(a,stname);//a=a"index
    strcat(a,yinhao);//a=a"index"
    
    
    
    while (n_index<=n_ran) {
        
        //生成一个点不能超过二十五次
        int attempt=25;
        
        while (1) {
            

            fflush(stdout);
            

            if(attempt <= 0){
                fprintf(stderr,"%s\n","Error: failed to generate valid input for 25 simultaneous attempts");
                exit(0);
            }
            
            char left[10]="(";
            

            char right[2]=")";
            char douhao[2]=",";
            char x_zuobiao[4];
            char y_zuobiao[4];
            
            int c_ran_x=random_number(-c_num,c_num);
            int c_ran_y=random_number(-c_num,c_num);
            
            int overflag=0;
            if (q->size>=2 && n_index>=1) {
                overflag=CheckOverlap(q,previousx,previousy,c_ran_x,c_ran_y,s_index,m);
            }
        
            if ((previousx!=c_ran_x||previousy!=c_ran_y)&&overflag==0 ) {
                
                AddList(q,s_index,c_ran_x,c_ran_y);
                
                sprintf(x_zuobiao, "%d", c_ran_x);
                sprintf(y_zuobiao, "%d", c_ran_y);
                strcat(left,x_zuobiao);//(x
                strcat(left,douhao);//(x,
                strcat(left,y_zuobiao);//(x,y
                strcat(left,right);//(x,y)
                strcat(a,left);
                
                previousx=c_ran_x;
                previousy=c_ran_y;
                n_index++;
                break;
            }
            
            attempt--;
        }
        
        //sleep(l_num);
    }
    
    //printf("%s\n",a);
    fprintf(stdout,"%s\n", a);
    
}

void processHandle(){
    exit(0);
}


int main(int argc, const char * argv[]) {
    
    signal(SIGTERM, processHandle);
    
    int index;

    const char *s="-s";
    const char *n="-n";
    const char *l="-l";
    const char *c="-c";
    
    //default k
    int s_num=10;
    int n_num=5;
    int l_num=5;
    int c_num=20;
    
    Queue *q;
    int s_index;
    
    //s>=2 n>=1 l>=5 c>=1
    
    if (argc>1) {
        for (index=1; index<argc; index=index+2) {
            //printf("目前在比较的argv[index]=%s\n",argv[index]);
            if (strcmp(argv[index],s)==0) {
                s_num = atoi(argv[index+1]);
                if (s_num<2) {
                    fprintf(stderr,"%s\n","Error: your should input 's' number larger than 2.");
                    exit(0);
                }
            }
            else if (strcmp(argv[index],n)==0){
                n_num=atoi(argv[index+1]);
                if (n_num<1) {
                    fprintf(stderr,"%s\n","Error: your should input 'n' number larger than 1.");
                    exit(0);
                }
            }
            else if (strcmp(argv[index],l)==0){
                l_num=atoi(argv[index+1]);
                if (l_num<5) {
                    fprintf(stderr,"%s\n","Error: your should input 'l' number larger than 5.");
                    exit(0);
                }
            }
            else if (strcmp(argv[index],c)==0){
                c_num=atoi(argv[index+1]);
                if (c_num<1) {
                    fprintf(stderr,"%s\n","Error: your should input 'c' number larger than 20.");
                    exit(0);
                }
            }
        }
    }
    
    
    while (1) {
        int s_ran;
        int l_ran;
        
        if (s_num==2) {
            s_ran=2;
        }else{
            s_ran=random_number(0,s_num-2);
            if(s_ran>=0){
                s_ran=s_ran+2;
            }
            if (s_ran<0) {
                s_ran=-s_ran+2;
            }
        }
        
        if (l_num==5) {
            l_ran=5;
        }else{
            l_ran=random_number(0,l_num-5);
            if (l_ran>=0) {
                l_ran=l_ran+5;
            }
            if (l_ran<0) {
                l_ran=-l_ran+5;
            }
        }
        
        
        printf("r\n");
        
        
        q=CreateList();
        
        
        for (s_index=0; s_index<s_ran; s_index++) {
            
            OutputStreet(s_index,n_num,c_num,q,l_num,m);
            
        }
        
        
        printf("g\n");
        fflush(stdout);
        sleep(l_num);
        
    }
    
    return 0;
}

