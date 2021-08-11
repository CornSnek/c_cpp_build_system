#include <stdio.h>
#include <sodium.h>
const char hex_chars[16]={//Just lowercase.
    '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
};
int main(int argc,char** argv){
    if(sodium_init()==-1){
        fprintf(stderr,"Unable to initialize sodium.\n");
        printf("000000"); //Default 000000 on error.
        return 1;
    }
    if(argc==2){
        int length=atoi(*(argv+1));
        for(char i=0;i<length;i++){
            printf("%c",hex_chars[randombytes_random()%16]);//Random every time the program is run.
        }
        printf("%c",'\0'); //Last character should be null terminator.
        return 0;
    }
    printf("000000");
    fprintf(stderr,"Usage: Number to return random hexadecimal characters (lowercase).\n");
    return 1;
}
