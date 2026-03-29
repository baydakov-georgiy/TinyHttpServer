#pragma once

typedef struct {
    char *method;
    char *url;
    char *version;
} http_headers;

http_headers *parse_request(char *request);
void free_headers(http_headers* headers);
