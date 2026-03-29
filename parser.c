#include "parser.h"
#include <stdlib.h>
#include <string.h>

// TODO: Use zero copy-pasting technic
http_headers *parse_request(char *request) {
    char *delim = "\r\n";
    char *rest_request = request;
    char *line = strtok_r(request, delim, &rest_request);
    char *rest_line = line;
    char *method = strtok_r(line, " ", &rest_line);
    char *url = strtok_r(NULL, " ", &rest_line);
    char *version = strtok_r(NULL, " ", &rest_line);

    http_headers *headers = (http_headers *)calloc(1, sizeof(http_headers));

    headers->method = malloc(strlen(method) + 1);
    headers->url = malloc(strlen(url) + 1);
    headers->version = malloc(strlen(version) + 1);

    strncpy(headers->method, method, strlen(method) + 1);
    strncpy(headers->url, url, strlen(url) + 1);
    strncpy(headers->version, version, strlen(version) + 1);

    return headers;
}

void free_headers(http_headers *headers) {
    free(headers->method);
    free(headers->url);
    free(headers->version);
    free(headers);
}
