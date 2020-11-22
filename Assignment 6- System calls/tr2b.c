#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

  if(argc!=3){
    fprintf(stderr, "Error: Incorrect number of arguments");
    exit(1);
  }

  char* from = argv[1];
  char* to = argv[2];

  int fromlen = strlen(from);
  int tolen = strlen(to);
  
  if(fromlen!=tolen){
   fprintf(stderr, "Error: From and To sets must be the same length");
   exit(1); 
  }

  //check for duplicate characters

  for(int i=0;i<fromlen;i++){
    for(int j=0;j<fromlen;j++){
      if(i!=j&&from[i]==from[j]){
	fprintf(stderr, "Error: Duplicate characters are not allowed.");
	exit(1);
      }
    }
  }

  char c;

  while((c = getchar()) != EOF){

    char output=c;

    for(int i=0;i<fromlen;i++){
      if(c==from[i]){
	output=to[i];
      }
    }

    putchar(output);
  }

}
