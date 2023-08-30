/*
  RMIT University Vietnam
  Course: EEET2601 Engineering Computing 1
  Semester: 2020B
  Assessment: Assessment 2
  Author: Nguyen Tien Thanh
  ID: Your student id s3818111
  Created  date: 22/08/2020
  Acknowledgement: Clearly acknowledge the resources that you use to complete this assessment
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// WRITE YOUR FUNCTION HERE
void strdate(char *src, char *dest) {
    int i,j;//name of variable
    //take the number out of punctuation and substitute to *dest
    for(int i=0;i<strlen(src);i++){
    
    
          if( isdigit(src[i]) ) 
          dest[j++]=src[i];
    }
     }

// PLEASE DON'T CHANGE ANYTHING FROM THIS LINE
int main() {
    // Get date in dd/mm/yyyy format
    char date1[11];
    printf("Enter date in dd/mm/yyyy format: ");
    scanf("%s", date1);

    // Test the function
    char date2[20];  // 20 characters is enough for date2
    strdate(date1, date2);
    printf("%s\n", date2);

    return 0;
}
