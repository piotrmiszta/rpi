#ifndef XML_PARSER_H_
#define XML_PARSER_H_
#include <stdio.h>
typedef long int xml_pos_t;

typedef struct XmlParserNodeS{
    char* string;
    long int start;
    long int end;
}XmlParserNodeS;

XmlParserNodeS* xml_parser_search_object(FILE* file, const char* string);
XmlParserNodeS* xml_parser_get_child(FILE* file, const XmlParserNodeS* const parent, const char* string);
XmlParserNodeS* xml_parser_search_next_object(FILE* file, const char* string, const XmlParserNodeS* const object);
void xml_free_node(XmlParserNodeS* node);


char* _xml_parser_get_value(FILE* file, xml_pos_t start, xml_pos_t end);
#define xml_parser_get_value(file, node) \
        _xml_parser_get_value(file, node->start, node->end)

char* _xml_parser_get_value_parent(FILE* file, xml_pos_t start, xml_pos_t end, const char* string);
#define xml_parser_get_value_parent(file, node, string) \
        _xml_parser_get_value_parent(file, node->start, node->end, string)
#endif //XML_PARSER_H_
