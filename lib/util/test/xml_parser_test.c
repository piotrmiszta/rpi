#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdint.h>
#include <cmocka.h>
#include "xml_parser.h"
#include <utility.h>
#include <stdlib.h>
static int setup(void** state) {
    LoggerParams param = {
        .author = "Piotr Miszta",
        .contact = "miszta.piotr.pm@gmail.com",
        .filename = "log.log",
        .version = 1.00,
    };
    logger_init(&param);
    FILE* file = fopen("/home/Piotr/rpi/files/server/devices/devices.xml", "r");
    if(!file) {
        return -1;
    }
    *state = file;
    return 0;
}

static int teardown(void** state) {
    fclose(*state);
    logger_close();
    return 0;
}
static inline void assertion_child(const char* expected, FILE* stream, XmlParserNodeS* parent) {
    char* result = xml_parser_get_value(stream, parent);
    assert_string_equal(expected, result);
    free(result);
}

static inline void assertion_parent(const char* expected, FILE* stream,XmlParserNodeS* parent,  const char* field) {
    char* result = xml_parser_get_value_parent(stream, parent, field);
    assert_string_equal(expected, result);
    free(result);
}

static void xml_search_test(void ** state) {
    XmlParserNodeS* xml = xml_parser_search_object(*state, "device");
    XmlParserNodeS* xml_name = xml_parser_get_child(*state, xml, "name");

    assertion_child("STM32h7", *state, xml_name);
    assertion_parent("STM32h7", *state, xml, "name");

    assertion_parent("stm32h7.xml", *state, xml, "xml_desc");
    assertion_parent("UART", *state, xml, "protocol");
    assertion_parent("1", *state, xml, "id");
    assertion_parent("No", *state, xml, "required");
    xml_free_node(xml);
    xml_free_node(xml_name);
}

static void xml_failed_search_test(void** state) {
    XmlParserNodeS* xml = xml_parser_search_object(*state, "device");

    assert_ptr_equal(NULL, xml_parser_get_value_parent(*state, xml, "xml_descs"));
    xml_free_node(xml);
}

static void xml_search_multi_test(void ** state) {
    XmlParserNodeS* device_1 = xml_parser_search_object(*state, "device");
    XmlParserNodeS* device_2 = xml_parser_search_next_object(*state, "device", device_1);
    XmlParserNodeS* device_3 = xml_parser_search_next_object(*state, "device", device_2);
    XmlParserNodeS* device_4 = xml_parser_search_next_object(*state, "device", device_3);

    assert_ptr_equal(device_4, NULL);
    assertion_parent("STM32h7",  *state, device_1, "name");
    assertion_parent("stm32h7.xml",  *state, device_1, "xml_desc");
    assertion_parent("UART",  *state, device_1, "protocol");
    assertion_parent("1",  *state, device_1, "id");
    assertion_parent("No",  *state, device_1, "required");


    assertion_parent("STM32f7",  *state, device_2, "name");
    assertion_parent("stm32f7.xml",  *state, device_2, "xml_desc");
    assertion_parent("UART",  *state, device_2, "protocol");
    assertion_parent("2",  *state, device_2, "id");
    assertion_parent("No",  *state, device_2, "required");


    assertion_parent("STM32f4",  *state, device_3, "name");
    assertion_parent("stm32f4.xml",  *state, device_3, "xml_desc");
    assertion_parent("UART",  *state, device_3, "protocol");
    assertion_parent("3",  *state, device_3, "id");
    assertion_parent("No",  *state, device_3, "required");
    xml_free_node(device_1);
    xml_free_node(device_2);
    xml_free_node(device_3);

}


int main(void)
{
    const struct CMUnitTest tests[] = {
            unit_test(xml_search_test),
            unit_test(xml_failed_search_test),
            unit_test(xml_search_multi_test),
    };
    return cmocka_run_group_tests(tests, setup, teardown);
}
