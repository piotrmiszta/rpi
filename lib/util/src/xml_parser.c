#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "error_codes.h"
#include <string.h>
#include <utility.h>

/** TODO:
 * @brief TODO: THIS FILE NEED TO BE OPTIMIZED
 *
*/

#define MAX_LEN_OF_DECL     100

static xml_pos_t search_start_pos(FILE* file, const char* string, xml_pos_t pos);
static xml_pos_t search_end_pos(FILE* file, const char* string, xml_pos_t pos);
static inline XmlParserNodeS* xml_node_alloc(const char* string, xml_pos_t start_pos, xml_pos_t end_pos);

void xml_free_node(XmlParserNodeS* node) {
    if(node) {
        free(node->string);
        node->string = NULL;
    }
    free(node);
}

XmlParserNodeS* xml_parser_search_object(FILE* file, const char* string) {
    if(!fseek(file, 0, SEEK_SET) == 0 ) {
        LOG_ERROR("Can't seek position in xml file error: %d", print_err());
        return NULL;
    }
    xml_pos_t start_pos = search_start_pos(file, string, 0);
    if(start_pos == -1) {
        return NULL;
    }
    xml_pos_t end_pos = search_end_pos(file, string, start_pos);
    if(end_pos == -1) {
        return NULL;
    }
    XmlParserNodeS* node = xml_node_alloc(string, start_pos, end_pos);

    return node;
}

XmlParserNodeS* xml_parser_get_child(FILE* file, const XmlParserNodeS* const parent, const char* string) {
    xml_pos_t start_pos = search_start_pos(file, string, parent->start);
    xml_pos_t end_pos = search_end_pos(file, string, start_pos);

    if(end_pos > parent->end) {
        LOG_FATAL("Error when parsing xml %s is end after parent end - %s", string, parent->string);
        return NULL;
    }
    XmlParserNodeS* node = xml_node_alloc(string, start_pos, end_pos);
    return node;
}

XmlParserNodeS* xml_parser_search_next_object(FILE* file, const char* string, const XmlParserNodeS* const object) {
    xml_pos_t prev_end = 0;
    if(object) {
        prev_end = object->end;
    }
    xml_pos_t start_pos = search_start_pos(file, string, prev_end);
    if(start_pos == -1) {
        return NULL;
    }
    xml_pos_t end_pos = search_end_pos(file, string, start_pos);
        if(end_pos == -1) {
        return NULL;
    }
    XmlParserNodeS* node = xml_node_alloc(string, start_pos, end_pos);
    return node;
}

char* _xml_parser_get_value(FILE* file, xml_pos_t start, xml_pos_t end) {
    fseek(file, start, SEEK_SET);
    char buff[MAX_LEN_OF_DECL] = {0};
    bool fstart = false;
    fread(&buff, sizeof(char), end - start, file);
    for(int i = 0; i < strlen(buff); i++) {
        if(buff[i] == '<') {
            start = true;
        }
        else if (buff[i] == '>' && fstart) {
            LOG_ERROR("Not expected type decl");
            return NULL;
        }
    }
    char* return_value = malloc(sizeof(char) * (strlen(buff) + 1));
    strcpy(return_value, buff);
    return return_value;
}

char* _xml_parser_get_value_parent(FILE* file, xml_pos_t start, xml_pos_t end, const char* string){
    xml_pos_t start_pos = search_start_pos(file, string, start);
    xml_pos_t end_pos = search_end_pos(file, string, start_pos);
    if(start_pos == -1) {
        LOG_ERROR("Can't find %s", string);
        return NULL;
    }
    else if(end_pos == -1) {
        LOG_ERROR("Can't find end of %s", string);
        return NULL;
    }
    else if(end_pos > end) {
        LOG_FATAL("Error when parsing xml %s is end after parent end - %s", string, string);
        return NULL;
    }

    return _xml_parser_get_value(file, start_pos, end_pos);
}

static xml_pos_t search_start_pos(FILE* file, const char* string, xml_pos_t pos) {
    xml_pos_t start_pos = -1;
    fseek(file, pos, SEEK_SET);

    while(!feof(file)) {
            char temp;
            char buff[MAX_LEN_OF_DECL] = {0};
            fread(&temp, sizeof(char), 1, file);
            if(temp == '<') {
            /* searched entity check that entity is equal to string*/
                int i = 0;
                while(i < MAX_LEN_OF_DECL) {
                    fread(&buff[i], sizeof(char), 1, file);
                    if(buff[i] == '>') {
                        buff[i] = '\0';
                        break;
                    }
                    i++;
                }
                if(strcmp(buff, string) == 0) {
                    start_pos = ftell(file);
                    break;
                }
                else {


                }
            }
        }
    return start_pos;
}

static xml_pos_t search_end_pos(FILE* file, const char* string, xml_pos_t pos) {
    xml_pos_t end_pos = -1;
    fseek(file, pos, SEEK_SET);
    while(!feof(file)) {
        char temp;
        char buff[MAX_LEN_OF_DECL];
        fread(&temp, sizeof(char), 1, file);
        if(temp == '<') {
            end_pos = ftell(file) - 1;
            fread(&temp, sizeof(char), 1, file);
            if(temp == '/') {
                int i = 0;
                while(i < MAX_LEN_OF_DECL) {
                    fread(&buff[i], sizeof(char), 1, file);
                    if(buff[i] == '>') {
                        buff[i] = '\0';
                        break;
                    }
                    i++;
                }
                if(strcmp(buff, string) == 0) {
                    break;
                }
                else {
                    end_pos = -1;
                }
            }
        }
    }
    return end_pos;
}

static inline XmlParserNodeS* xml_node_alloc(const char* string, xml_pos_t start_pos, xml_pos_t end_pos) {
    XmlParserNodeS* node = malloc(sizeof(XmlParserNodeS));
    assert_ss(node);
    node->string = malloc(strlen(string) + 1);
    assert_ss(node->string);
    strcpy(node->string, string);
    node->start = start_pos;
    node->end = end_pos;
    return node;
}
