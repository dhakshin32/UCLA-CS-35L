#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, const char* argv[]){

  
  if(argc!=3){
    fprintf(stderr, "Error: Incorrect number of arguments");
    exit(1);
  }

  const char* from = argv[1];
  const char* to = argv[2];

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

 while((read(0,&c,1)) > 0){

    char output=c;

    for(int i=0;i<fromlen;i++){
      if(c==from[i]){
        output=to[i];
      }
    }
    write(1, &output, 1);
  
  }


  
}
