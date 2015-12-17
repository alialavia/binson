//
// Created by ali on 12/8/15.
// This implementation uses UTF-8 encoding for JSON strings based on RFC3629
//

#include <stdio.h>
#include "jsonreader.h"
#define DEBUG 1
#define LOG if (DEBUG) printf

inline int isDigit(char c)
{
    return (c <= '9' && c >= '0');
}

inline int isWhitespace(char c)
{
    return (c == TAB || c == LF || c == CR || c == CR || c == SP);
}

/* Returns length of the UTF-8 character */
int symbol_len(char c)
{
    if (~c & 0x80)
        return 1;
    if(!((c & 0xc0) ^ 0xc0))
        return 2;
    if(!((c & 0xe0) ^ 0xe0))
        return 3;
    if(!((c & 0xf0) ^ 0xf0))
        return 4;
}

int json_readarray(char **jsonstr)
{
    (*jsonstr)++; // skip '['
    while (**jsonstr && **jsonstr != ']') {
        while (isWhitespace(**jsonstr))
            (*jsonstr)++;
        read_value(jsonstr);

        while (isWhitespace(**jsonstr))
            (*jsonstr)++;

        if (**jsonstr == ',')
            (*jsonstr)++;
        else if (**jsonstr == ']')
            break;
        else
            return JS_BAD_FORMAT_COMMA_EXPECTED | JS_BAD_FORMAT_CLOSING_SQUARE_BRACE_EXPECTED;
    }
    if (**jsonstr != ']')
        return JS_BAD_FORMAT_CLOSING_SQUARE_BRACE_EXPECTED;
    (*jsonstr)++; // skip ']'
    return 0;
}

int json_readobj(char **jsonstr)
{
    (*jsonstr)++; // skip '{'
    while (**jsonstr && **jsonstr != '}') {
        while (isWhitespace(**jsonstr))
            (*jsonstr)++;

        int ret;
        char *name = *jsonstr;
        // read name
        if ((ret = json_readstring(jsonstr)) < 0)
            return ret;

        name++; // skip opening "
        LOG("<name (%d): %.*s>\r\n", ret, ret, name);

        // skip white spaces
        while (isWhitespace(**jsonstr))
            (*jsonstr)++;

        if (**jsonstr != ':')
            return JS_BAD_FORMAT_COLON_EXPECTED;

        (*jsonstr)++; // skip ':'

        LOG("<value>\r\n");
        read_value(jsonstr);  // read value
        while (isWhitespace(**jsonstr))
            (*jsonstr)++;

        if (**jsonstr == ',')
            (*jsonstr)++; // skip ','
        else if (**jsonstr == '}')
            break;
        else
            return JS_BAD_FORMAT_COMMA_EXPECTED | JS_BAD_FORMAT_CLOSING_CURLY_BRACE_EXPECTED;
    }
    if (**jsonstr != '}')
        return JS_BAD_FORMAT_CLOSING_CURLY_BRACE_EXPECTED;
    (*jsonstr)++; // skip '}'
    return 0;
}

// only consider integers for now
int json_readnumber(char **jsonstr, int len)
{
    *jsonstr += len;
    return 0;
}

// called when **jsonstr == '\"'
int json_readstring(char **jsonstr)
{
    char* start = *jsonstr;
    (*jsonstr)++; // skip opening "
    while (**jsonstr && !(**jsonstr=='\"' && *(*jsonstr-1)!='\\'))
        (*jsonstr)++;

    if (**jsonstr != '\"')
        return JS_BAD_FORMAT_CLOSING_QUOTE_EXPECTED;
    (*jsonstr)++; // skip closing "
    return (int) (*jsonstr - start - 2);
}

// match regexp: ^-?(0|[1-9]\d*)(.\d+)?([eE][+-]?\d+)?$
// TEST: Positive: OK, Negative: TODO
int matchJSONNumber (char **jsonstr)
{
    int counter = 0;
    // optional minus sign
    if (**jsonstr == '-')
        counter++;

    // first group, non optional
    if ((*jsonstr)[counter] == '0')
        counter++;
    else if ((*jsonstr)[counter] <= '9' && (*jsonstr)[counter] >= '1')
    {
        counter++;
        while (isDigit((*jsonstr)[counter]))
            counter++;
    }
    else
        return 0;

    // second group, optional
    if ((*jsonstr)[counter] == '.' && isDigit((*jsonstr)[counter+1])) {
        counter += 2;
        while (isDigit((*jsonstr)[counter]))
            counter++;
    }

    // third group, optional
    if ((*jsonstr)[counter] == 'e' || (*jsonstr)[counter] == 'E') {
        counter++;
        if ((*jsonstr)[counter] == '+' || (*jsonstr)[counter] == '-')
            counter++;

        while (isDigit((*jsonstr)[counter]))
            counter++;
    }

    return counter;
}

// if bson == 0 only return the number of required bytes, otherwise set the whole byte
// On error return error code
int read_value(char **jsonstr)
{
    jsontype valtype = 0x0;
    int ret = 0;
    if (!*jsonstr)
        return -1;
    while (**jsonstr)
    {
        while (isWhitespace(**jsonstr))
            (*jsonstr)++;

        if (**jsonstr == '{') {
            valtype = JS_OBJECT;
            if ((ret = json_readobj(jsonstr)) < 0)
                return ret;
            return 0;
        }
        else if (**jsonstr == '[') {
            valtype = JS_ARRAY;
            if ((ret = json_readarray(jsonstr)) < 0)
                return ret;
            return 0;
        }
        else if (**jsonstr == '\"') {
            valtype = JS_STRING;
            if ((ret = json_readstring(jsonstr)) < 0)
                return ret;
            return 0;
        }
        else if (**jsonstr == 'f') {
            valtype = JS_FALSE;
            *jsonstr += 5;
            return 0;
        }
        else if (**jsonstr == 't') {
            valtype = JS_TRUE;
            *jsonstr += 4;
            return 0;
        }
        else if (**jsonstr == 'n') {
            valtype = JS_NULL;
            *jsonstr += 4;
            return 0;
        }
        else if ((ret = matchJSONNumber(jsonstr)) > 0) {
            valtype = JS_NUMBER;
            json_readnumber(jsonstr, ret);
            return 0;
        }
        else
        {
            return JS_BAD_FORMAT_UNEXPECTED_CHAR;
        }
    }
    return 0;
}