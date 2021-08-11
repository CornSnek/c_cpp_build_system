#include <hex_utilities.h>
bool parse_valid_hex(const char col_str[static 7]){//7 to account for the '\\0' to terminate the hex string.
    for(int i=0;i<6;i++){
        if(!isxdigit(col_str[i])){//Is from 0-9,a-z,or A-Z
            return false;
        }
    }
    return col_str[6]=='\0';
}
void hex_lerp(int c1num, int c2num, int percentage){
    const int r_mask=0xff0000,g_mask=0x00ff00,b_mask=0x0000ff;
    const int rednum=(int)( ((c1num&r_mask)>>16) + (int)( ( ((c2num&r_mask)>>16) - ((c1num&r_mask)>>16) ) * (((float)percentage)/100) ) )<<16; //Bitshifting/bitmasking to lerp the 5th and 6th bytes.
    const int greennum=(int)( ((c1num&g_mask)>>8) + (int)( ( ((c2num&g_mask)>>8) - ((c1num&g_mask)>>8) ) * (((float)percentage)/100) ) )<<8; //... 3rd and 4th bytes.
    const int bluenum=(c1num&b_mask) + (int)( ( (c2num&b_mask) - (c1num&b_mask) ) * (((float)percentage)/100)) ;
    printf("%06x",rednum+greennum+bluenum); //Output lerp to stdout. Pad 6 0s so that the colors will read as 00 red, 00 green, and/or 00 blue.
}
