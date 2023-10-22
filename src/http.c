#include <stdio.h>
#include <curl/curl.h>

#include "http.h"
#include "value.h"
#include "vm.h"

typedef struct CURLResponse {
    char* headers;
    char* content;
    size_t hSize;
    size_t cSize;
} CURLResponse;

typedef enum HTTPMethod {
    HTTP_HEAD, 
    HTTP_GET, 
    HTTP_POST, 
    HTTP_PUT, 
    HTTP_DELETE, 
    HTTP_PATCH, 
    HTTP_OPTIONS, 
    HTTP_TRACE,
    HTTP_CONNECT,
    HTTP_QUERY
} HTTPMethod;

// static size_t httpCURLHeaders(void* headers, size_t size, size_t nitems, void* userdata) {
//     size_t realsize = size * nitems;
//     if (nitems != 2) {
//         CURLResponse* curlResponse = (CURLResponse*)userdata;
//         char* ptr = realloc(curlResponse->headers, curlResponse->hSize + realsize + 1);
//         if (!ptr) return 0;

//         curlResponse->headers = ptr;
//         memcpy(&(curlResponse->headers[curlResponse->hSize]), headers, realsize);
//         curlResponse->hSize += realsize;
//         curlResponse->headers[curlResponse->hSize] = 0;
//     }
//     return realsize;
// }

// static size_t httpCURLResponse(void* contents, size_t size, size_t nmemb, void* userdata) {
//     size_t realsize = size * nmemb;
//     CURLResponse* curlResponse = (CURLResponse*)userdata;
//     char* ptr = realloc(curlResponse->content, curlResponse->cSize + realsize + 1);
//     if (!ptr) return 0;

//     curlResponse->content = ptr;
//     memcpy(&(curlResponse->content[curlResponse->cSize]), contents, realsize);
//     curlResponse->cSize += realsize;
//     curlResponse->content[curlResponse->cSize] = 0;
//     return realsize;
// }

// static ObjString* httpParsePostData(VM* vm, ObjDictionary* postData) {
//     if (postData->count == 0) return emptyString(vm);
//     else {
//         char string[UINT8_MAX] = "";
//         size_t offset = 0;
//         int startIndex = 0;

//         for (int i = 0; i < postData->capacity; i++) {
//             ObjEntry* entry = &postData->entries[i];
//             if (IS_UNDEFINED(entry->key)) continue;
//             Value key = entry->key;
//             char* keyChars = valueToString(vm, key);
//             size_t keyLength = strlen(keyChars);
//             Value value = entry->value;
//             char* valueChars = valueToString(vm, value);
//             size_t valueLength = strlen(valueChars);

//             memcpy(string + offset, keyChars, keyLength);
//             offset += keyLength;
//             memcpy(string + offset, "=", 1);
//             offset += 1;
//             memcpy(string + offset, valueChars, valueLength);
//             offset += valueLength;
//             startIndex = i + 1;
//             break;
//         }

//         for (int i = startIndex; i < postData->capacity; i++) {
//             ObjEntry* entry = &postData->entries[i];
//             if (IS_UNDEFINED(entry->key)) continue;
//             Value key = entry->key;
//             char* keyChars = valueToString(vm, key);
//             size_t keyLength = strlen(keyChars);
//             Value value = entry->value;
//             char* valueChars = valueToString(vm, value);
//             size_t valueLength = strlen(valueChars);

//             memcpy(string + offset, "&", 1);
//             offset += 1;
//             memcpy(string + offset, keyChars, keyLength);
//             offset += keyLength;
//             memcpy(string + offset, "=", 1);
//             offset += 1;
//             memcpy(string + offset, valueChars, valueLength);
//             offset += valueLength;
//         }

//         string[offset] = '\0';
//         return copyString(vm, string, (int)offset + 1);
//     }
// }

// static CURLcode httpSendRequest(VM* vm, ObjString* url, HTTPMethod method, ObjDictionary* data, CURL* curl, CURLResponse* curlResponse) {
//     curlResponse->headers = malloc(0);
//     curlResponse->content = malloc(0);
//     curlResponse->hSize = 0;
//     curlResponse->cSize = 0;

//     char* urlChars = url->chars;
//     curl_easy_setopt(curl, CURLOPT_URL, urlChars);
//     if (method > HTTP_POST) {
//         curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, httpMapMethod(method));
//     }
 
//     if (method == HTTP_HEAD) {
//         curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
//     }
//     else if (method == HTTP_POST || method == HTTP_PUT || method == HTTP_PATCH) {
//         char* dataChars = httpParsePostData(vm, data)->chars;
//         curl_easy_setopt(curl, CURLOPT_POSTFIELDS, dataChars);
//     }
//     else if (method == HTTP_OPTIONS) {
//         curl_easy_setopt(curl, CURLOPT_REQUEST_TARGET, "*");
//     }

//     curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpCURLResponse);
//     curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)curlResponse);
//     curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, httpCURLHeaders);
//     curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)curlResponse);
//     curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
//     return curl_easy_perform(curl);
// }
// char* request(const char* url, const char* method, const char* body)
// {
//     CURL *curl;
//     CURLcode res;

//     curl_global_init(CURL_GLOBAL_DEFAULT);
//     curl = curl_easy_init();

//     if(curl)
//     {
//         curl_easy_setopt(curl, CURLOPT_URL, url);
//         res = curl_easy_perform(curl);
//         if (res != CURLE_OK)
//         {
//             fprintf(stderr, "curl_easy_perform() failed: '%s'\n", curl_easy_strerror(res));
//         }
//         curl_easy_cleanup(curl);
//         printf("\n");
//         curl_global_cleanup();
//     }
// }