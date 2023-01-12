#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include "hdr.h"
#include "parse.h"
void usage() {
    printf("syntax: beacon-flood <interface> <ssid-list-file>\n");
    printf("sample: beacon-flood mon0 ssid-list.txt\n");
}

int main(int argc, char* argv[]){

    if (argc < 3) {
            usage();
            return -1;
    }
    Parse(argv);
}
