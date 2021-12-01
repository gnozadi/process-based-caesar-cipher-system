#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

// Add a str to another in place k
void strInsert(char str1[], const char str2[], int k) {
   int i, length ;
   length = strlen(str2);
   for (i=strlen(str1); i >= k ; i --) 
        str1[i+length] = str1[i] ;
   
   for (i=0 ;i<length; i++)
       str1[k + i] = str2[i] ;
}

// Remove str2 from str1
int strDelete(char str1[], const char str2[]) {
    int i, position, length;
    char* pos_ptr;
    pos_ptr = strstr(str1, str2);
    position=pos_ptr-str1;
    if (position == -1) return(0);
    
    length = strlen(str2) ;
    for (i= position+length ; str1[i]; i++){
          str1[i - length] = str1[i] ;
    }
    str1[i-length] = '\0' ;
    return(1);
}

// Return first index of matching
int find(char str1[], const char str2[]) {
    char* pos_ptr=strstr(str1,str2);
    if(pos_ptr==NULL){
     return -1;
     }
    else return pos_ptr-str1;
}

char* replaceWord(char s[], char oldW[], char newW[]){
   int pos;
   char* pos_ptr;


   pos=find(s,oldW);
   strDelete(s,oldW);

   strInsert(s,newW,pos);

   return s;
}

int main(){
   printf("placer started!\n");
   const unsigned MAX_LENGTH = 256;
   char ps[MAX_LENGTH]; // placer input 
   char ps_temp[MAX_LENGTH];
   char important_words[MAX_LENGTH];
   char words[MAX_LENGTH];
   char* result;
   int i=0,j=0,sw=0;


   //FILE* fp=fopen("placerInput.txt","r+");
   //FILE* fr=fopen("words.txt","r");
   FILE* fw=fopen("finalPlacer.txt","w");

   int count=0;

   //while(fgets(ps_temp,MAX_LENGTH,fp)){
   //   if (count==0) strcpy(ps,ps_temp);
   //   else strcat(ps,ps_temp);
  //    count++;
  // }


   //while (fgets(important_words, MAX_LENGTH,fr));


   char* fifo_pp = "fifo_pp"; //fifo between parent & placer
   mkfifo(fifo_pp, 0666);
   char* fifo_fp = "fifo_fp"; //fifo between finder & plcer
   mkfifo(fifo_fp, 0666);

   int pp=open(fifo_pp,O_RDONLY);
   read(pp,ps,240);
   close(pp);

   printf("buffer = %s\n",ps);

   int fp=open(fifo_fp,O_RDONLY);
   read(fp,important_words,240);
   close(fp);

    printf("\n\n\n%s\n\n\n", ps);
    printf("i words \n\n\n%s\n\n\n", important_words);
   printf("placer = %s \n words %s \n",ps,important_words);
   do{
       printf("hello\n");
       sw=0,j=0;
       //geting the important words
   for (;sw==0; i++){
       // printf("i= %c hello\n",important_words[i]);
       if(important_words[i]=='*'){
           important_words[i]='@';
           sw++;
       }
       else{
           words[j]=important_words[i];
           j++;
       }
   }
   
    //putting the important words into their place
   char* new_ptr;
   new_ptr=(char*)calloc(j,sizeof(char));
   for(int x=0;x<j;x++){
        new_ptr[x]=words[x];
    }
    result=replaceWord(ps,"$",new_ptr);
    printf("\nresult= \n%s", result);
     printf("hello\n");
    free(new_ptr);
   }while(find(important_words,"*")!=-1);
    
    //saving the final text
    printf("\nresult= \n%s", result);
    fprintf(fw,"\n\n*************88%s",result);
    //fclose(fp);
    //fclose(fr);
    fclose(fw);
    return 0;
}
