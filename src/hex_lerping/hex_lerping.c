#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <hex_utilities.h>

int main(int argc,char** argv){
    if(argc!=4){
        fprintf(stderr,"Usage: 2 hexadecimal strings of the form rrggbb, and a number from 1 to 100.\n");
        printf("000000");//Print black on error.
        exit(1);
    }
    const char* s1num=*(argv+1);
    if(strlen(s1num)!=6){
        fprintf(stderr,"Error! Hexadecimal strings of length 6 is required for first/second arguments.\n");
        printf("000000");
        exit(1);
    }
    const char* s2num=*(argv+2);
    if(strlen(s2num)!=6){
        fprintf(stderr,"Error! Hexadecimal strings of length 6 is required for first/second arguments.\n");
        printf("000000");
        exit(1);
    }
    const int percentage=strtol(*(argv+3),NULL,10);
    if(percentage<0||percentage>100){
        fprintf(stderr,"Error! 3rd argument, %s, is not a valid number between 0 and 100.\n",*(argv+3));
        printf("000000");
        exit(1);
    }
    int c1num, c2num;
    if(!parse_valid_hex(s1num)){
        fprintf(stderr,"Error! 1st argument, %s, is not a valid hexadecimal string. Should be between 000000 and ffffff only.\n",s1num);
        printf("000000");
        exit(1);
    }
    c1num=strtol(s1num,NULL,16);//Convert to integer (hex string to decimal) for hex_lerp
    if(!parse_valid_hex(s2num)){
        fprintf(stderr,"Error! 1st argument, %s, is not a valid hexadecimal string. Should be between 000000 and ffffff only.\n",s2num);
        printf("000000");
        exit(1);
    }
    c2num=strtol(s2num,NULL,16);
    hex_lerp(c1num,c2num,percentage);
    return 0;
}
