
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

int pid_R = 0;

int main(int argc, char *argv[])
{
    int pid0;
    int pid1;
    int pid2;
    int pid3;
    //rgen-a1
    int rgen[2];
    //a1
    int a1[2];
    //a2
    int a2[2];
    
    pipe(rgen);
    pipe(a1);
    pipe(a2);
    
    char buf[100];
    
    pid0=fork();
    
    if (pid0==0) {
        //执行rgen
        //printf("rgen to do\n");
        pid_R = getpid();
        
        close(rgen[0]);
        dup2(rgen[1],STDOUT_FILENO);
        execv("rgen", argv);
        close(rgen[1]);
    }else{
        pid1=fork();
        if (pid1==0) {
            //执行a1
            //printf("a1 python to do\n");
            close(rgen[1]);
            close(a1[0]);
            dup2(rgen[0],STDIN_FILENO);
            dup2(a1[1],STDOUT_FILENO);
            //dup2(pipe2[1],STDOUT_FILENO);
            execl("/usr/bin/python", "/usr/bin/python", "./a1-ece650.py", (char *)NULL);
            close(rgen[0]);
            close(a1[1]);
            close(a2[1]);
        }else{
            pid2=fork();
            if (pid2==0) {
                //执行a2
                //printf("a2 to do\n");
                close(a1[1]);
                close(a2[1]);
                dup2(a2[0],STDIN_FILENO);
                execlp("./a2-ece650","a2-ece650",NULL);
                close(a1[0]);

            }else{
                pid3=fork();
                if (pid3==0) {
                    //执行python a1输出到屏幕
                    close(a1[1]);
                    close(a2[0]);
                    char* buf = NULL;
                    size_t bytes = 0;
                    FILE *p1 = fdopen(a1[0], "r");
                    FILE *p2 = fdopen(a2[1], "w");
                    while (getline(&buf, &bytes, p1) != EOF)
                    {
                        fprintf(stdout, "%s", buf);
                        fputs(buf, p2);
                        fflush(NULL);
                    }
                    fclose(p1), fclose(p2);
                    
                }else{
                    //从屏幕读输入
                    while(fgets(buf,100,stdin)!=NULL)
                    {
                        write(a2[1],buf,strlen(buf));
                    }
                    close(a1[1]);
                    kill(pid_R,SIGTERM);

                }
            }
        }
    }



    
}