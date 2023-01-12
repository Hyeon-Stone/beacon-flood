#include <stdio.h>
#include <stdint.h>
#include <pcap.h>
#include <arpa/inet.h>

void Parse(char** argv);
int Check_Beacon(const u_char* data, uint16_t len);
char* Capture_BeaconorData(const u_char* data, uint16_t len, int bssid);
void Change_Wireless(const u_char* data, uint16_t len, char** line, int idx, int diff_cnt);
uint16_t Capture_RadioTap(const u_char* data, int option);
int getbit(uint32_t x, int n);

enum options{
    RADIOTAP_LEN = 0,
    RADIOTAP_FIND_PRESENT_FLAG = 1,
    ENC = 4
};
