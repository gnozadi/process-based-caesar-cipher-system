#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//decoder
void decoder(char str[]) {

 int size = strlen(str);
 char* ptr;
 ptr = (char*)malloc(size);
 
 for (int i = 0; i < strlen(str); i++) {
     
     //decoding
    int ascii = (int)str[i];
    
    //for a b c and A B C
    if ((ascii<100 && ascii>91) || (ascii<68 && ascii>64)) ascii+=26;
    ascii -= 3;

    // 7==space // 10==newline
    if((char)ascii!='7' && ascii!=10) ptr[i] = (char)ascii;

   }
 //return
 strcpy(str,ptr);
}


void main() {

    printf("decoder started!\n");
    //FILE* fr;
    FILE* fw;
    char string[10];
    const unsigned MAX_LENGTH = 1000;
    char ds[MAX_LENGTH];
   

    //fr = fopen("decodeInput.txt", "r");
    fw = fopen("decoded.txt", "w");

    char* fifo_pd = "fifo_pd"; //fifo between parent & decoder
    mkfifo(fifo_pd, 0666);
    char* fifo_df = "fifo_df"; //fifo between decoder & finder
    mkfifo(fifo_df, 0666);

    int pd=open(fifo_pd,O_RDONLY);
    read(pd,ds,MAX_LENGTH);
    close(pd);

    // Put decode input in temp array
    //while (fgets(ds, MAX_LENGTH, fr));

    //decode input
    decoder(ds);        
   
    printf("\n\ndecode input is: %s\n\n\n", ds); 
    // Save to a file(decoded.txt)
    fprintf(fw,"%s",ds);


    // Close files
    fclose(fw);
    //fclose(fr);
    
    int df=open(fifo_df,O_WRONLY);
    write(df,ds,strlen(ds)+1);
    close(df);
}
