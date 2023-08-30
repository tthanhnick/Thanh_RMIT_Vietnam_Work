/*
  RMIT University Vietnam
  Course: EEET2601 Engineering Computing 1
  Semester: 2020B
  Assessment: Assessment 1
  Author: Nguyen Tien Thanh
  ID: S3818111
  Created 25/07/2020
  Acknowledgement: Clearly acknowledge the resources that you use to complete this assessment
*/
#include <stdio.h>
#include <string.h>


int main(){
    int count=0;
    int counts=0;
    int dem=0;
    int dems=0;
    int cong=0;
    int other=0;
    char dg[]="digit";
    char dgs[]="digits";
    char vw[]="vowels";
    char vws[]="vowels";
    char cn[]="consonant";
    char cns[]="consonants";
    char ch[]="character";
    char chs[]="characters";
    char string[100];
    char vowels []="ueoai";
    char vowel[]="UEOAI";
    char digit[]="0123456789";
    char consonants[]="qwrtypsdfghjklzxcvbnm";
    char consonant[]="QWRTYPSDFGHJKLZXCVBNM";

//input the string
printf("Enter a string: ");
scanf("%[^\n]",string);

//check for digits consonants vowels and other characters
for(int i=0;i<strlen(vowels);i++){
    for(int j=0;j<strlen(string);j++){
    if(string[j]==vowels[i]){
        count++;
    }          
}
}
for(int i=0;i<strlen(vowel);i++){
    for(int j=0;j<strlen(string);j++){
    if(string[j]==vowel[i]){
        counts++;
    }          
}
}
for(int i=0;i<strlen(consonants);i++){
    for(int j=0;j<strlen(string);j++){
    if(string[j]==consonants[i]){
        dem++;
    }          
}
}
for(int i=0;i<strlen(consonant);i++){
    for(int j=0;j<strlen(string);j++){
    if(string[j]==consonant[i]){
        dems++;
    }          
}
}
for(int i=0;i<strlen(digit);i++){
    for(int j=0;j<strlen(string);j++){
    if(string[j]==digit[i]){
        cong++;
    }          
}
}
other=strlen(string)-(count+counts)-(dem+dems)-cong;

//check wheher print out either singular or plural noun
if(cong>1){
    strcpy(dg, dgs);
}else{
     strcpy(dgs, dg);
}
if(count+counts>1){
    strcpy(vw, vws);
}else{
     strcpy(vws, vw);
}
if(dem+dems>1){
    strcpy(cn, cns);
}else{
     strcpy(cns, cn);
}
if(other>1){
    strcpy(ch, chs);
}else{
     strcpy(chs, ch);
}

//print the result
printf("The string has %d %s, %d %s, %d %s, and %d other %s.",cong,dg,count+counts,vw,dem+dems,cn,other,ch);
return 0;
}
