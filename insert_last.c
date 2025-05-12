/*
Name : Bevin kumar .k
date : 04.04.2025
project : minishell
*/

#include "main.h"
extern char input_string[50];
extern int fd;
int insert_last(Slist **head,char *buf,int pid)
{
    Slist *new=malloc(sizeof(Slist));
    if(new==NULL)
    {
        return 0;
    }
    strcpy(new->cmd,buf);
    new->link=NULL;
    new->pid=pid;
    strcpy(new->status,"Stopped");
    if(*head==NULL)
    {
        *head=new;
        return 1;
    }
    Slist *temp=*head;
    while(temp->link !=NULL)
    {
    temp=temp->link;
    }
    temp->link=new;
    return 1;  
}
//delete the command from linked list
int delete_last(Slist **head)
{
    if(*head==NULL)
    {
        return 0;
    }
    if((*head)->link == NULL)
    {
        free(*head);
        *head= NULL;
        return 1;
    }
    Slist *temp=*head,*prev;
    while(temp->link != NULL)
    {
        prev=temp;
        temp=temp->link;
    }
    prev->link=NULL;
    free(temp);
    
    return 1;

}
int last_pid(Slist *head)
{
    if(head == NULL)
	{
		return 0;
	}
	Slist *temp = head;
	while(temp-> link != NULL)
	{
		temp = temp ->link;
	}
	return temp -> pid;
}