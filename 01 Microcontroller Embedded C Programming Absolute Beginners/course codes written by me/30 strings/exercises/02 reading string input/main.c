#include <stdio.h>

int main() {
    char name[30];
    printf("enter your name!\n");
    //scanf("%s",name);   // will read the string but can't handle white space in between as it will consider a limiting character
    fflush(stdin);
    scanf("%[^\n]s",name); // scan set is used to handle white spaces in between name , so it will read anything until enter is pressed

    printf("Hi %s",name);


    // testing purposes
    /*for (int i = 0; i < 30; ++i) {
        printf("%X\n",name[i]);
    }*/
    return 0;
}
