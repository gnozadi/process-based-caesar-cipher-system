#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


int main(){
    printf("finder started!\n");
    int start,length,pos;
    const unsigned MAX_LENGTH = 1000;
    char fs[MAX_LENGTH];
    char words[MAX_LENGTH];
    char final[MAX_LENGTH];

   // char* ptr;
    char d[]="$";
    char* pos_ptr;
   

    FILE* fww;

    // Initialize files
    //f=fopen("finderInput.txt","r");//fifo_pf
    //fw=fopen("decoded.txt","r"); //fifo_df
    fww=fopen("words.txt","w"); //fifo_fp



    char* fifo_pf = "fifo_pf"; //fifo between parent & finder
    mkfifo(fifo_pf, 0666);

    char* fifo_df = "fifo_df"; //fifo between decoder & finder
    mkfifo(fifo_df, 0666);

    char* fifo_fp = "fifo_fp"; //fifo between finder & placer
    mkfifo(fifo_fp, 0666);

    int pf=open(fifo_pf,O_RDONLY);
    read(pf,fs,240);
    close(pf);
  
    int df=open(fifo_df,O_RDONLY);
    read(df,words,240);
    close(df);

 
    // Put finder input in fs    
    //while (fgets(fs, MAX_LENGTH, f));

    // Put decoded string in words 
    //while (fscanf(fw,"%s",words)>=0);
    
 int fp = open(fifo_fp, O_WRONLY);
 int counter=0;
    do{
       pos_ptr=strstr(fs,d);
       if(pos_ptr==NULL){
          break;
       }
       else pos= pos_ptr-fs;

    
       // Find statr and length of important words
       printf("pos: %d\n",pos);
       start=(fs[pos-3]-48);
       if(fs[pos-4]>='0' && fs[pos-4]<='9') start+=(fs[pos-4]-48)*10;

       length=(fs[pos-1]-48);
       if(fs[pos-2]>='0' && fs[pos-2]<='9') length+=(fs[pos-2]-48)*10;
    
       fs[pos]='*';
       int j=0;
       char ptr[length];
   
       printf(" start: %d, length: %d\n",start,length);

       char* new_ptr=(char*)calloc(length,sizeof(char));
       for (int i=start;pos_ptr!=NULL && j<length;i++){
	   fprintf(fww,"%c",words[i]);
      new_ptr[j]=words[i];
	   j++;
       }
       fprintf(fww,"%c",'*');
       new_ptr[j]='*';
       if(counter==0)strcpy(final,new_ptr);
       else strcat(final,new_ptr);
       counter++;
       //write(fp, , strlen(new_ptr)+1);
       free(new_ptr);
    }while(pos_ptr!=NULL);
    
   write(fp, final, strlen(final)+1);
   // int fp = open(fifo_fp, O_WRONLY);
  /*   write(fp, words, strlen(words)+1);
    close(fp);*/
    char text2[MAX_LENGTH];
    FILE* fff=fopen("words.txt","r");
    while(fgets(text2,MAX_LENGTH,fff));
    printf("\n text2== %s\n",text2);
    write(fp,text2,strlen(text2)+1);
    close(fp);

    //fclose(f);
    //fclose(fw);
    fclose(fww);
     
return 0;
}
