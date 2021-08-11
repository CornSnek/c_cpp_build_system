//This program loads binaries from the other programs (random_hex and hex_lerping) to show background color results from a terminal (with truecolor or not).
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <hex_utilities.h>
#define R_FD(fd_arr) fd_arr[0]
#define W_FD(fd_arr) fd_arr[1]
#define CLOSE_R_FD(fd_arr) close(fd_arr[0])
#define CLOSE_W_FD(fd_arr) close(fd_arr[1])
#define PROGRAM_FAILED 1

void get_random_hex(char str[static 7]);
void change_dir_to_bin(char* bin);
void get_lerp_str(const char hexstr1[static 7],const char hexstr2[static 7],int percentage,char result[static 7]);
void print_background_color(const char fromhexstr[static 7],const char tohexstr[static 7]);
int main(int argc,char** argv){
    change_dir_to_bin(*argv); //Because the execl strings are hardwired to the binary working directory, change directory to the binary's directory.
    char hexstr1[7],hexstr2[7];
    switch(argc){
    case 1:
        get_random_hex(hexstr1);
        get_random_hex(hexstr2);
        break;
    case 3:
        if(parse_valid_hex(*(argv+1))&&parse_valid_hex(*(argv+2))){
            strcpy(hexstr1,*(argv+1));
            strcpy(hexstr2,*(argv+2));
            break;
        }
        fprintf(stderr,"USAGE: 2 arguments requires 2 valid null-terminated hexstrings of 6 characters.\n");
        exit(PROGRAM_FAILED);
    default:
        fprintf(stderr,"USAGE: 0 arguments => Program provides 2 random hexstrings of 6 characters.\n2 arguments => Program uses the 2 hexstrings you provide.\n");
        exit(PROGRAM_FAILED);
    }
    print_background_color(hexstr1,hexstr2);
    return 0;
}
void change_dir_to_bin(char* bin){
    const char* prog_name=strrchr(bin,'/')+1;//+1 to exclude '/' itself.
    if(!prog_name) return; //No '/', then no need to change directory.
    const int ch_bin_len=prog_name-bin;
    char* ch_bin_str=(char*)calloc(ch_bin_len+1,sizeof(char));//+1 to include '\0'
    strncpy(ch_bin_str,bin,ch_bin_len);
    if(chdir(ch_bin_str)!=0){perror("chdir error"); exit(PROGRAM_FAILED);}
    free(ch_bin_str);
}
void get_random_hex(char str[static 7]){//Writes a random 6 hexadecimal string with 7th as '\\0'
    int fd[2];
    if(pipe(fd)){
        perror("pipe error");
        exit(PROGRAM_FAILED);
    }
    pid_t pid=fork();
    switch(pid){
    case -1:
        perror("fork error");
        exit(PROGRAM_FAILED);
    case 0:
        CLOSE_R_FD(fd);
        dup2(W_FD(fd),STDOUT_FILENO);//Redirect to stdout for execl.
        execl("../random_hex/random_hex","./random_hex","6",NULL);//Write hex string result to parent's read fd.
        perror("execl error");
        exit(PROGRAM_FAILED);
    default:
        CLOSE_W_FD(fd);
        int status;
        waitpid(pid,&status,0);
        if(WIFEXITED(status)&&WEXITSTATUS(status)==PROGRAM_FAILED){
            fprintf(stderr,"child exited with error\n");
            exit(PROGRAM_FAILED);
        }
        read(R_FD(fd),str,sizeof(char)*7);
        CLOSE_R_FD(fd);
    }
}
void get_lerp_str(const char hexstr1[static 7],const char hexstr2[static 7],int percentage,char result[static 7]){
    int fd[2];
    char percent_str[4]={0};
    sprintf(percent_str,"%d",percentage);
    if(pipe(fd)){
        perror("pipe error");
        exit(PROGRAM_FAILED);
    }
    pid_t pid=fork();
    switch(pid){
    case -1:
        perror("fork error");
        exit(PROGRAM_FAILED);
    case 0:
        CLOSE_R_FD(fd);
        dup2(W_FD(fd),STDOUT_FILENO);//Redirect to stdout for execl.
        execl("../hex_lerping/hex_lerping","./hex_lerping",hexstr1,hexstr2,percent_str,NULL);//Write hex string result to parent's read fd.
        perror("execl error");
        exit(PROGRAM_FAILED);
    default:
        CLOSE_W_FD(fd);
        int status;
        waitpid(pid,&status,0);
        if(WIFEXITED(status)&&WEXITSTATUS(status)==PROGRAM_FAILED){
            fprintf(stderr,"child exited with error\n");
            exit(PROGRAM_FAILED);
        }
        read(R_FD(fd),result,sizeof(char)*7);
        CLOSE_R_FD(fd);
    }
}
void print_background_color(const char fromhexstr[static 7],const char tohexstr[static 7]){
    printf("Showing background from colors: %s to %s...\n",fromhexstr,tohexstr);
    for(int i=0;i<=100;i++){
        char lerp_str[7]={0};
        get_lerp_str(fromhexstr,tohexstr,i,lerp_str);
        char rgb_str[3][3]={0};//Slicing string into twos. Fixed bug: In shared_src/hex_utilites/hex_utilities.c, padded 6 0s into the hex_lerp function so that this string reads properly.
        strncpy(*rgb_str,lerp_str,2);//r
        strncpy(*(rgb_str+1),(lerp_str+2),2);//g
        strncpy(*(rgb_str+2),(lerp_str+4),2);//b
        const long r=strtol(rgb_str[0],NULL,16),g=strtoul(rgb_str[1],NULL,16),b=strtoul(rgb_str[2],NULL,16);
        //Formula from http://24ways.org/2010/calculating-color-contrast/. Black (30) or white (37).
        int t_color=(((r*299)+(g*587)+(b*114))/1000)>=(0x7f)?30:37;
        printf("\033[48;2;%ld;%ld;%ld;%dm[%ld,%ld,%ld](%s);\033[0m",r,g,b,t_color,r,g,b,lerp_str);
    }
    printf("\n\033[0mFinished showing background colors from %s to %s...\n",fromhexstr,tohexstr);//Reset colors.
    printf("This terminal DOES %ssupport truecolor. Some terminals may show incorrect color hex codes.\n",((getenv("COLORTERM")==NULL)?"NOT ":""));
}
