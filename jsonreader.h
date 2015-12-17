//
// Created by ali on 12/8/15.

//

#ifndef BINSON_JSONREADER_H
#define BINSON_JSONREADER_H
#define TAB 0x09
#define LF 0x0A
#define CR 0x0D
#define SP 0x20

typedef enum jsontype {
    JS_ARRAY,
    JS_OBJECT,
    JS_NUMBER,
    JS_STRING,
    JS_TRUE,
    JS_FALSE,
    JS_NULL
} jsontype;

#define JS_BAD_FORMAT_COMMA_EXPECTED -1
#define JS_BAD_FORMAT_CLOSING_CURLY_BRACE_EXPECTED -2
#define JS_BAD_FORMAT_CLOSING_SQUARE_BRACE_EXPECTED -3
#define JS_BAD_FORMAT_COLON_EXPECTED -4
#define JS_BAD_FORMAT_CLOSING_QUOTE_EXPECTED -5
#define JS_BAD_FORMAT_UNEXPECTED_CHAR -6

int matchJSONNumber (char**);
int json_readstring(char **jsonstr);
int json_readnumber(char **jsonstr, int len);
int json_readobj(char **jsonstr);
int json_readarray(char **jsonstr);
int read_value(char **jsonbuffer);

#endif //BINSON_JSONREADER_H
