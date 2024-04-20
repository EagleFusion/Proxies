#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <stdlib.h>

#define TOR_CONFIG_FILE "/etc/tor/torrc"
#define TOR_SOCKS_PORT "9050"

int is_tor_socks_enabled() {
        FILE *file = fopen(TOR_CONFIG_FILE, "r");
        if (file==NULL) {
                fprintf(stderr, "Failed to open Tor configuration\n");
                return 0;
        }
        char line[256];
        int found = 0;
        while(fgets(line, sizeof(line), file)) {
                if(strstr(line, "SocksPort")) {
                        char *port = strtok(line, " ");
                        port = strtok(NULL, " ");

                        if(port != NULL && strcmp(port, TOR_SOCKS_PORT) == 0) {
                                found = 1;
                                break;
                        }
                }
        }
        fclose(file);
        return found;
}

size_t write_callback(char *ptr, size_t size, size_t nmemb, FILE *userdata) {
    return fwrite(ptr, size, nmemb, userdata);
}

void web_scrapper() {
        CURL *curl;
        CURLcode res;
        FILE *output_file;
        curl = curl_easy_init();
        if(curl) {
                curl_easy_setopt(curl, CURLOPT_URL, "https://www.wikipedia.org/");
                curl_easy_setopt(curl, CURLOPT_PROXY, "socks5h://localhost:9050");

                output_file = fopen("output.html", "wb");
                if(!output_file) {
                        fprintf(stderr, "Failed to open output file for writing\n");
                }

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
                fprintf(stderr, "curl_esay_perform() failed: %s\n", curl_easy_strerror(res));
        }

        fclose(output_file);
        curl_easy_cleanup(curl);
}

int main() {
        int status;
        int choice=0;
        printf("1.Check Tor Proxy\n2.Web_Scrapper\n3.Port Scanner\n4.Exit:\t");
        scanf("%d", &choice);
        switch(choice) {
                case 1:
                        if((status = is_tor_socks_enabled()) == 0) {
                                printf("Enable the tor socks before proceeding\n");
                                exit(0);
                        }
                break;

                case 2:
                        web_scrapper();
                break;

                case 3:
                        printf("Working on it\n");
                break;

                case 4:
                        exit(0);
                break;

                default:
                        printf("Option not specified\nExiting........\n");
                        system("clear");
                        exit(0);
                break;
        }
    return 0;
}
