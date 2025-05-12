/*
Name : Bevin kumar.k
Date : 26.03.2025
project: minishell
*/

#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "main.h"
char input_string[50];
char prompt[10]="minishell$";
int main()
{
    system("clear");
    
    
    scan_input(prompt,input_string);
}