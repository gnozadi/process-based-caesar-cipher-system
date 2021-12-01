#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
  int pid1,pid2;
  FILE* frr;
  const unsigned MAX_LENGTH = 10000;
  char temp[MAX_LENGTH];
  char text[MAX_LENGTH];
  char decoder[MAX_LENGTH];
  char finder[MAX_LENGTH];
  char placer[MAX_LENGTH];

  //Creating named pipes
  char* fifo_pd = "fifo_pd"; //fifo between parent & decoder
  mkfifo(fifo_pd, 0666);
  char* fifo_pf = "fifo_pf"; //fifo between parent & finder
  mkfifo(fifo_pf, 0666);
  char* fifo_pp = "fifo_pp"; //fifo between parent & placer


  // Open input file
  int i=0,sw=0;
  frr = fopen("text.txt", "r");
  
  // Put input file in temp array
  while(fgets(temp,MAX_LENGTH,frr)){
    strcat(text,temp);
  }

  // Marking the end of file with a '!'
  strcat(text,"!");


   ///**************************************///
  ///*Getting the text for child processes*///
 ///**************************************///

  ///Getting the text for decoder
  for(int j=0;text[i]!='#';j++){    
    decoder[j]=text[i];
    i++;
  }
  //Pass 'x###'
  i+=4;
  
  //Getting the text for finder
  for(int j=0;text[i]!='#';j++){
    finder[j]=text[i];
    i++;
    }
  i+=4;

  //Getting the text for placer
  for(int j=0;text[i]!='!';j++){     
    placer[j]=text[i];
    i++;
  }

   ///**************************************///
  ///*         Save Parts in Files        *///
 ///**************************************///

  //printf("decoder: %s\n",decoder);
  FILE* fwd=fopen("decodeInput.txt","w");
  fputs(decoder,fwd);
  fclose(fwd);

  //printf("finder: %s\n",finder);
  FILE* fwf=fopen("finderInput.txt","w");
  fputs(finder,fwf);
  fclose(fwf);

  //printf("placer: %s\n",placer);
  FILE* fwp=fopen("placerInput.txt","w");
  fputs(placer,fwp);
  fclose(fwp);

  sleep(1);



   ///**************************************///
  ///*         Start processes            *///
 ///**************************************///

  pid1=fork();
  pid2=fork();
  

   ///************************///
  ///*Handle child processes*///
 ///************************///


  // Still In parent process and child process didn't start
  if (pid1<0 && pid2<0) { 
    printf("FAIL");
    return -1;
  }

  // Child 1 - decoder
  else if (pid1==0 && pid2>0){ 
     sleep(2);
     printf("First child(decoder) is starting...\n");
     char *args[]={"./decoder",NULL};
     execvp(args[0],args);

     //int fd=open("fifo_pd",O_RDONLY);

  }
  // Child 2 - finder
  else if (pid2==0 && pid1>0){ 
     
     sleep(3);
     printf("Second child(finder) is starting...\n");
     char *args[]={"./finder",NULL};
     execvp(args[0],args);

  } 
  // Child 3 - placer
  if (pid2==0 && pid1==0){ //child 3 - placer
    
    sleep(10);
    printf("Third child(placer) is starting...\n");
    char *args[]={"./placer",NULL};
    execvp(args[0],args);

  // In parent process
  }else{ 
    sleep(5);
  }
  sleep(10);
  printf("Opening the pipes to write...\n");
  // writing the texts on pipe
  int pd=open(fifo_pd,O_WRONLY);
  write(pd,decoder,strlen(decoder));
  close(pd);

  int pf=open(fifo_pf,O_WRONLY);
  write(pf,finder,strlen(finder)+1);
  close(pf);

  int pp=open(fifo_pp,O_WRONLY);
  write(pp,placer,strlen(placer)+1);
  close(pp);

  sleep(5);
  return 0;
}
