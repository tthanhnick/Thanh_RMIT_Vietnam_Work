/*
  RMIT University Vietnam
  Course: EEET2601 Engineering Computing 1
  Semester: 2020B
  Assessment: Assessment 3
  Author: Nguyen Tien Thanh
  ID: s3818111
  Created  date: 19/09/2020
  Acknowledgement: Clearly acknowledge the resources that you use to complete this assessment
*/

//Declare header
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include<ctype.h>


int main(void) {

    //open FILE
    FILE *fp=fopen("data.txt","r");
    FILE *fp1=fopen("result.txt","w");

    //declare variable
    int i;
    char line[1024];
    float a[9999];
    float average=0.0;
    int count=1;

    //while function to get string
  while (fgets(line, 1024, fp)!=NULL)
    {
        
        //declare variable
        char word[100];
        float word_len=0.0;
        
        //calculate average
        for(int i=0;i<strlen(line);i++){
           if(isspace(line[i])==0){
                word_len++;
                count++;
           }
                if(isspace(line[i])!=0){ 
                    average=(average+word_len/2);
                    a[i]=word_len;
                    word_len=0;
                }
          
           }
       } 
       

    //print to another file
    fprintf(fp1,"%.2f",average);
    fclose(fp);
    fclose(fp1);   
    return 0;
}
