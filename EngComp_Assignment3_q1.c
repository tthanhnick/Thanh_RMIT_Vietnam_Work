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

//declare header
#include <stdio.h>
#include <string.h>

// Declare the function
char *mystrcat(char *dest, char *src);

// Do not change main()
int main() {
    // Test mystrcat()
    char s1[20] = "Hello, "; // 20 is enough to append s2 into s1
    char s2[] = "World!";
    printf("%s\n", mystrcat(s1, s2));
    return 0;
}

// Implement the function from here
char *mystrcat(char *dest, char *src) {

    //declare variable temp
    char *temp;

    //use for loop to substitute the value
    for(int i=0;i<strlen(src);i++){
        temp=&src[i];
        for(int j=0;j<strlen(dest);j++){
            temp=&dest[j];
        }
    }

    //return the variable
    return temp;
}
