/*******************************
 * wyshell.c
 * Krishna Sai Chemudupati
 * November 16, 2018
 * COSC 3750, Homework 8
 *
 * Modified upon the simple.c given by Dr.Buckner.
 * C source code for wyshell.c
**********************************/

#include<stdio.h>
#include"wyscanner.h"

typedef struct node Node;
typedef struct word Word;

struct node{
   Node * next;
   Node * prev;
   char * command;
   Word * arg_list;
   int input; 
   int output;
   int error;
   char* in_file;
   char * out_file;
   char * err_file;
};

struct word{
   Word *next;
   Word *prev;
   char *string;

};


int main()
{
  char *tokens[]={ "QUOTE_ERROR", "ERROR_CHAR", "SYSTEM_ERROR",
                "EOL", "REDIR_OUT", "REDIR_IN", "APPEND_OUT",
                "REDIR_ERR", "APPEND_ERR", "REDIR_ERR_OUT", "SEMICOLON",
                "PIPE", "AMP", "WORD" };
  int rtn;
  char *rpt;
  char buf[1024];

  while(1) {
    printf("%s","$> ");   
    rpt=fgets(buf,256,stdin);
    if(rpt == NULL) {
      if(feof(stdin)) {
	return 0;
      }
      else {
	perror("fgets from stdin");
	return 1;
      }
    }
    rtn=parse_line(buf);
    int track = 0;
    int errorFound = -1;
    int outRedirFound = -1;
    int wordOutRedir = -1;
    int inRedirFound = -1;
    int wordInRedir = -1;
    int foundPipe =-1;
    int foundPipeWord = -1;
    int foundAmp = -1;
    while(rtn !=  EOL && errorFound<0){ 
      // that makes it break out when an error or EOL is seen
      switch(rtn) {
        case WORD:
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(track == 0){
            printf(":--: %s\n",lexeme);
            track = 1;
          }else {
            printf(" --: %s\n", lexeme); 
          }
          if(outRedirFound > 0){
            wordOutRedir = 1;
          }
          if(inRedirFound > 0){
            wordInRedir = 1;
          }
          if(foundPipe > 0){
            foundPipeWord = 1;
          }
          break;
        case ERROR_CHAR:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound  = 1;
            break;
          }
          printf("error character: %d\n",error_char);
          errorFound = 1;
          break;
        case SYSTEM_ERROR:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          perror("system error");
          return 1;
          break;
        case QUOTE_ERROR:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          errorFound = 1;
          printf("quote error\n");
          break;
        case REDIR_OUT:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(inRedirFound > 0 && wordInRedir < 0){
            errorFound = 1;
            printf("Ambiguous input redirection\n");
            break;
          }          
          if(outRedirFound > 0){
            errorFound = 1;
            printf("Ambiguous output redirection\n");
          }else{
            outRedirFound = 1;
            printf(" %s\n", ">"); 
          }
          break;
        case APPEND_OUT:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(inRedirFound > 0 && wordInRedir < 0){
            errorFound = 1;
            printf("Ambiguous input redirection\n");
            break;
          }          
          if(outRedirFound > 0){
            errorFound = 1;
            printf("Ambiguous output redirection\n");
          }else{
            outRedirFound = 1;
            printf(" %s\n", ">>"); 
          }
          break;
        case REDIR_ERR:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(inRedirFound > 0 && wordInRedir < 0){
            errorFound = 1;
            printf("Ambiguous input redirection\n");
            break;
          }          
          if(outRedirFound > 0){
            errorFound = 1;
            printf("Ambiguous output redirection\n");
          }else{
            outRedirFound = 1;
            printf(" %s\n", "2>"); 
          }
          break;
        case APPEND_ERR:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(inRedirFound > 0 && wordInRedir < 0){
            errorFound = 1;
            printf("Ambiguous input redirection\n");
            break;
          }          
          if(outRedirFound > 0){
            errorFound = 1;
            printf("Ambiguous output redirection\n");
          }else{
            outRedirFound = 1;
            printf(" %s\n", "2>>"); 
          }
          break;
        case REDIR_ERR_OUT:
           if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(inRedirFound > 0 && wordInRedir < 0){
            errorFound = 1;
            printf("Ambiguous input redirection\n");
            break;
          }
          if(outRedirFound > 0){
            errorFound = 1;
            printf("Ambiguous output redirection\n");
          }else{
            outRedirFound = 1;
            printf(" %s\n", "2>1");
          }
          break; 
        case REDIR_IN:
        //case APPEND_IN:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(outRedirFound > 0){
            // && wordOutRedir < 0
            errorFound = 1;
            printf("Ambiguous output redirection\n");
            break;
          } 
          if(inRedirFound > 0 || foundPipe > 0){
            errorFound = 1;
            printf("Ambiguous input redirection\n");
          }else{
            inRedirFound = 1;
            printf(" %s\n", "<"); 
          }
          break;
        case PIPE:
          if(track == 0){
            printf("Pipe, Semicolon or Redirection before first\n");
            errorFound = 1;
            break;
          }
          if(foundAmp > 0){
            printf("Amp not at the end.\n");
            errorFound = 1;
            break;
          }
          if(outRedirFound > 0 || (inRedirFound > 0 && wordInRedir < 0)){
            // || inRedirFound > 0
            errorFound = 1;
            printf("Ambiguous output redirection\n");
            break;
          } 
          track = 0;
          foundPipe = 1;
          foundPipeWord = -1;
          printf(" %s\n", "|"); 
          break;
        case SEMICOLON:
          if(track == 0){
            errorFound = 1;
            printf("Semicolon before first.\n");
            break;
          }
          track = 0;
          outRedirFound = -1;
          wordOutRedir = -1;
          inRedirFound = -1;
          wordInRedir = -1;
          foundPipe = -1;
          foundAmp = -1;
          printf(" %s\n", ";");
          break; 
        case AMP: 
          if(foundAmp > 0){
            printf("Found more than one Ampersand tokens in one command.\n");
            break; 
          }
          foundAmp = 1;
          printf(" &\n");
          break;
        default:          
          printf("%d: %s\n",rtn,tokens[rtn%96]);
      }
      rtn=parse_line(NULL);
    }
    if(((inRedirFound > 0 && wordInRedir < 0) || (outRedirFound > 0 && wordOutRedir < 0) || (foundPipe > 0 && foundPipeWord < 0)) && errorFound < 0){
      printf("Ambiguous output/input redirection\n");
      errorFound = 1;
    }
    if(errorFound<0){
      printf(" --: EOL\n");
    }
    printf("\n");
  }
}

