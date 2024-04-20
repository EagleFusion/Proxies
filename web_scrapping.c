#include <stdio.h>
#include <curl/curl.h>

size_t write_callback(char *ptr, size_t size, size_t nmemb, FILE *userdata) {
    return fwrite(ptr, size, nmemb, userdata);
}

int main() {
    CURL *curl;
    CURLcode res;
    FILE *output_file1;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.example.com");
        curl_easy_setopt(curl, CURLOPT_PROXY, "socks5h://localhost:9050");
        // Open file for writing
        output_file1 = fopen("output1.html", "wb");
        if (!output_file1) {
            fprintf(stderr, "Failed to open output file for writing\n");
            return 1;
        }

        // Set the file as the write callback
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, output_file1);

        res = curl_easy_perform(curl);
        if(res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));

        // Close the file
        fclose(output_file1);

        curl_easy_cleanup(curl);
    }
    return 0;
}
