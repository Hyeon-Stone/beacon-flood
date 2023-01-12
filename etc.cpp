#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void PrintMAC(char* msg, uint8_t *mac){
    printf("| %s | %02x:%02x:%02x:%02x:%02x:%02x |\n", msg, mac[0], mac[1],mac[2],mac[3],mac[4],mac[5]);
}
int CountLine(char* file_name){
    int line_cnt = 0;
    char temp;
    FILE* file;
    file = fopen(file_name, "r");

    while(fscanf(file,"%c",&temp) != EOF){
        if(temp == '\n')
            line_cnt++;
    }
    fclose(file);
    return line_cnt;
}
void ReadFile(char* file_name, char** line_set){
    int idx = 0;
    FILE *in = fopen(file_name, "r");
    while (!feof(in)) {
        fgets(line_set[idx++], 32, in);
    }
    fclose(in);
}
char* EraseEnter(char* str){
    for (int i = 0; str[i] != 0; i++) {
        if (str[i] == '\n') {
            str[i] = 0;
            break;
        }
    }
    return str;
}
int GetStringLen(char* str){
    int cnt = 0;
    while(*str){
        if(*str >> 7)
            str++;
        str++;
        cnt++;
    }
    return cnt;
}

void text_align_center(int columns, char* text){
    int indent;
    int indent2;
    if(((columns - GetStringLen(text)) % 2) == 0 )
        indent2 = indent = ((columns - GetStringLen(text)) / 2);
    else{
            indent = (columns - GetStringLen(text)-1) / 2;
            indent2 = indent+1;
    }
    for (int i = 0; i < indent-1; i++)
    {
        putchar(' '); // 왼쪽 여백 채우기
    }
    printf("%s", text);

    for (int i = 0; i <= indent2; i++)
    {
        putchar(' '); // 왼쪽 여백 채우기
    }
    printf("|\n");
}

