/*
Name : Bevin kumar.k
Date : 27.03.2025
project: minishell
*/
#include<stdio.h>
#include "main.h"
char *external_commands[200];
int wstatus;
int fd;
int fd1;
Slist *head=NULL;
extern char prompt[10];
extern char input_string[50];
void scan_input(char *prompt, char *input_string)
{
    signal(SIGINT ,handler);
	signal(SIGTSTP,handler);
    extract_external_commands(external_commands);
    

    while(1)
    {
        printf("%s",prompt);
        scanf(" %[^\n]",input_string);
        if(strncmp(input_string,"PS1",3)==0)
        {
        int len=strlen(input_string);
        int index=0;
        int found=0;
            for(int i=0;i<len-1;i++)
            {
                if(strchr(input_string,' '))
                {
                    found=1;
                    break;
                }
                if(input_string[i] == '=')
                {
                    index=i;
                    //printf("%d",index);
                }
            }
            
                if(found)
                {
                    printf("Command not found\n");
                }
                else
                {
                    strcpy(prompt,input_string+index+1);
                }
        }
        char *cmd=get_command(input_string);
        int res=check_command_type(cmd);
       // printf("%d",res);
       if(res==BUILTIN)
       {
        execute_internal_commands(input_string);
       }
       else if(res==EXTERNAL)
       {
        fd=fork();

        if(fd>0)
        {
            
            
           if(waitpid(fd,&wstatus,WUNTRACED)==-1)
           {
            perror("waitdpid failed");
            exit(1);
           }

        }
        else if(fd==0){
        
        signal(SIGINT,SIG_DFL);
		signal(SIGTSTP,SIG_DFL);

        execute_external_commands(input_string);
       }
    }
    else if(res==NO_COMMAND)
    {
        printf("No command found\n");
    }
       

        memset(input_string,0,50);
    }
    
}
char *get_command(char *input_string)
{
    int len=strlen(input_string);
    int index=0;
    static char command[20];

    for(int i=0;i<len;i++)
    {
        if(input_string[i]==' ')
        {
            
            strncpy(command,input_string,i);
            command[i]='\0';

            //printf("%s",command);
            return command;
        }
    }
        
        strcpy(command,input_string);
        //printf("%s",command);
        return command;
}
int check_command_type(char *command)
{
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
        "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
        "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", "jobs", "fg", "bg", NULL};
        
        for(int i=0;builtins[i]!=NULL;i++)
        {
            if(strcmp(builtins[i],command)==0)
            {
                return BUILTIN;
            }

        }
    for(int i=0;external_commands[i] != NULL;i++)
        {
            if(strcmp(external_commands[i],command)==0)
            {
                return EXTERNAL;
            }
        }
        return NO_COMMAND;
}
void extract_external_commands(char **external_commands)
{
    int fd=open("cmd.txt",O_RDONLY);
    if(fd==-1)
    {
        printf("failed to open command.txt\n");
        exit(1);
    }
    char buffer[256];
    int index;
    int bytes_read;
    while((bytes_read=read(fd,buffer,sizeof(buffer)-1))>0)
    {
        buffer[bytes_read]='\0';
        char *token=strtok(buffer, " \n");
        while(token != NULL)
        {
            if(index<200)
            {
                external_commands[index] = (char *)malloc(strlen(token) + 1);
                if (external_commands[index] == NULL)
                 {
                    perror("Failed to allocate memory for command");
                    exit(1);
                 }
                 strcpy(external_commands[index], token); 

                 index++;

            }
            token=strtok(NULL," \n");
        }
    }
    
    close(fd);

}
//function to execute internal commands
void execute_internal_commands(char *input_string)
{
    input_string[strcspn(input_string, "\n")] = '\0';
    //comparing the input string contain "exit"
    if(strcmp(input_string,"exit")==0)
    {
        exit(0);
    }
    //comparing the input string contain "pwd"
    else if(strcmp(input_string,"pwd")==0)
    {
         char buf[50];
         printf("%s",getcwd(buf,50));
    }
    //comparing the input string contain "cd"
    else if(strncmp(input_string,"cd",2)==0)
    {
        char *buf;
        buf=strtok(input_string," ");
        buf=strtok(NULL," ");
        if(buf==NULL)
        {
           printf("The path is missing in the command\n");
        }
        else{
        if(chdir(buf)==-1)
        {
            perror("cd");
        }
        }
    }
//comparing the input string contain "echo $$"
    else if(!strcmp(input_string,"echo $$"))
    {
        printf("%d\n",getpid());
    }
    //comparing the input string contain "echo $?"
     else if(!strcmp(input_string,"echo $?"))
     {
        printf("%d\n",WEXITSTATUS(wstatus));
        
     }
     //comparing the input string contain "ECHO $SHELL"
    else if(!strcmp(input_string,"echo $SHELL"))
    {
        printf("%s",getenv("SHELL"));
    }
    else if(!strcmp(input_string,"jobs"))
    {
      int i = 1;
	if (head == NULL)
	{
    	printf("INFO : List is empty\n");
	}
    else
    {
	    while (head)		
	    {
		    printf("[%d]              ", i++);
            printf("%s     ",head -> status);
            printf("%s     \n",head -> cmd);
		    head = head -> link;
	    }

    }
    }
    else if(!strcmp(input_string,"fg"))
    {
        int pid=last_pid(head);
        if(pid>0)
        {
            kill(pid, SIGCONT);
            delete_last(&head);
            waitpid(pid,&wstatus,WUNTRACED);
        }

    }
    else if(!strcmp(input_string,"bg"))
    {
        fd1=last_pid(head);
        if(fd1>0)
        {
            signal(SIGCHLD,handler);
            kill(fd1,SIGCONT);
            delete_last(&head);    
        }
}
}
//functions to execute external commands
void execute_external_commands(char *input_string)
{
    int found=0;
    int count=0;
    int index=0;
    char *input[20];
    char *token=strtok(input_string," ");
    while(token != NULL)
    {
        input[index]=token;
        index++;
        token=strtok(NULL," ");
    }
    input[index]=NULL;
    
    for(int i=0;input[i] !=NULL;i++)
    {
        if(strchr(input[i],'|'))
        {
            found=1;
            count++;
            
        }
    }
    // for (int i = 0; input[i] != NULL; i++) {
    //     printf("%s\n", input[i]);
    // }
    if(!found)
    execvp(input[0],input);
    
    else if(found)
{
    n_pipe(input,count);
    
}

}
//n-pipe function
void n_pipe(char *input[],int count)
{
    //take a backup to store stdin and stdout
   int backin=dup(0);
   int backout=dup(1);
    char *cmds[count + 1][20];
    int index = 0, arg_index = 0;

    for (int i = 0; input[i] != NULL; i++)
    {
        // check pipe is present or not and counting no of pipes
    if (strcmp(input[i], "|") == 0)
    {
    cmds[index][20] = NULL;
    index++;
    arg_index = 0;
    } else
    {
        //storing in 2D array
    cmds[index][arg_index++] = input[i];
    }
    }
    cmds[index][arg_index] = NULL;
    int prev_fd = 0;
    //run a loop from 0 to pipe count +1
    for (int i = 0; i <= count; i++)
     {
        //creating pipe
        int pipefd[2];
        pipe(pipefd);
        pid_t pid = fork();
//creating a child process
        if (pid == 0) 
        {
            dup2(prev_fd, 0);
            if (i != count)
             {
                dup2(pipefd[1], 1);
            }
            close(pipefd[0]);
            execvp(cmds[i][0], cmds[i]);
            perror("Execution failed");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]);
        prev_fd = pipefd[0];
        wait(NULL);
    }
//placing back the stdin and stdout in correct places
    dup2(backin, 0);
    dup2(backout, 1);
    close(backin);
    close(backout);  
}
void handler(int signum)
{
	
	//change the behavior of signal
	if(signum == SIGINT)
    {
        //if input_string is 0 print prompt
		if(strlen(input_string)==0)
        {
        printf("\n"); 
        printf("%s",prompt); 
        fflush(stdout); 
        }
        
    }
	else if(signum == SIGTSTP)
	{
        if(strlen(input_string)==0)
        {
	    printf("\n%s", prompt);
        fflush(stdout); 
        }
      else
      {
	  //store the datas in single linked list if command is present
      
        printf("\n");
        char com[50];
        strcpy(com,input_string);
        insert_last(&head,com,fd);
        }
        if(signum == SIGCHLD)
		waitpid(fd1, &wstatus, WUNTRACED);
	}
}



