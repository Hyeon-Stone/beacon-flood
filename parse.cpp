#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "hdr.h"
#include "parse.h"
#include "etc.h"
#define TAG_OFFSET 2
#define ESSID_MAX_LEN 32
void Parse(char** argv){

    char* dev = argv[1];
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(dev,BUFSIZ,1,1000,errbuf);
    if (handle == NULL) {
       fprintf(stderr, "couldn't open device %s(%s)\n", dev, errbuf);
       exit(-1);
    }
    struct pcap_pkthdr* header;
    const u_char* data;

    int line_cnt = CountLine(argv[2]);
    char** line = (char**)malloc(line_cnt*sizeof(char*));
    for (int i = 0; i<line_cnt; i++)
        line[i] = (char*)calloc(ESSID_MAX_LEN,sizeof(char));
    char* mac = (char*)malloc(sizeof(char)*17);
    int diff_cnt = 0;
    char** mac_lst = (char **)malloc((diff_cnt+1)*sizeof(char*));
    for (int i = 0; i<diff_cnt+1; i++)
        mac_lst[i] = (char*)calloc(17,sizeof(char));

    ReadFile(argv[2],line);
    printf("|======|=======================|================================|\n");
    printf("|  N0  |          MAC          |              ESSID             |\n");
    printf("|======|=======================|================================|\n");

    while(1){
        int res = pcap_next_ex(handle, &header, &data);
        if(res == 0) continue;
        if(res == -1 || res == -2){
            printf("pcap_next_ex return %d(%s)\n",res, pcap_geterr(handle));
        }
        uint16_t RadioTap_Len = Capture_RadioTap(data,RADIOTAP_LEN);
        int bssid = Check_Beacon(data,RadioTap_Len); //CHECK Beacon
        if (bssid == 0)
            continue;
        else{
            mac = Capture_BeaconorData(data,RadioTap_Len,bssid);
            int check = 1;
            int x = 1;
            for(x; x<=diff_cnt; x++){
                if(strncmp(mac_lst[x],mac,17) == 0){
                    check = 0;
                    break;
                }
            }
            if(check == 1){
                diff_cnt++;
                char** tempmac_lst = (char**)realloc(mac_lst,sizeof(char*)*(diff_cnt+1));
                mac_lst = tempmac_lst;
                mac_lst[diff_cnt] = (char*)calloc(17,sizeof(char));
                strncpy(mac_lst[diff_cnt],mac,17);
                printf("|  %2d  |   %3s   |",diff_cnt,mac, EraseEnter(line[(diff_cnt-1)%line_cnt]));
                text_align_center(32,EraseEnter(line[(diff_cnt-1)%line_cnt]));
                printf("|------|-----------------------|--------------------------------|\n");
            }
            Change_Wireless(data,RadioTap_Len,line,x,line_cnt); //ESSID
            int res = pcap_sendpacket(handle, reinterpret_cast<const u_char*>(data),header->caplen);
            if (res != 0) {
                fprintf(stderr, "pcap_sendpacket return %d error=%s\n", res, pcap_geterr(handle));
            }
        }
    }
    free(mac);
    free(handle);
    free(line);
    for(line_cnt; line_cnt >=0; line_cnt--)
        free(line[line_cnt]);

    for(diff_cnt; diff_cnt >=0; diff_cnt--)
        free(mac_lst[diff_cnt]);
    free(*mac_lst);
}

uint16_t Capture_RadioTap(const u_char* data, int option){
    RadioTap* capture = (RadioTap*)data;
    if (option == 0)
        return capture->len;
}

int Check_Beacon(const u_char* data, uint16_t len){
    Beacon* beacon = (Beacon*)(data+len);
    if (beacon->type == 0x80){
        return 1;
    }
    else
        return 0;
}
char* Capture_BeaconorData(const u_char* data, uint16_t len, int bssid){
    char* temp_mac = (char *)malloc(17*sizeof(char));
        Beacon* beacon = (Beacon*)(data+len);
        sprintf(temp_mac,"%02x:%02x:%02x:%02x:%02x:%02x",beacon->BSSID[0], beacon->BSSID[1],beacon->BSSID[2],beacon->BSSID[3],beacon->BSSID[4],beacon->BSSID[5]);
        return temp_mac;

}
void Change_Wireless(const u_char* data, uint16_t len, char** line, int idx, int cnt){
    int Beacon_Len = 24;
    int SSID_Offset = Beacon_Len +12 + TAG_OFFSET;
    Wireless* wireless = (Wireless*)(data+len+Beacon_Len);
    wireless->tag_len = strlen(line[(idx-1)%cnt]);
    char* essid = (char*)(data+len+SSID_Offset);
    memcpy(essid,line[(idx-1)%cnt], strlen(line[(idx-1)%cnt]));
}
