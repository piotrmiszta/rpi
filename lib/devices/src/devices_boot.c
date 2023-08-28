#include "devices.h"
#include "error_codes.h"
#include <utility.h>
#include "devices_boot.h"
#include "xml_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include "devices_thread.h"
static error_t devices_parse_xml(FILE* file);
static inline protocol_t devices_get_protocol_from_string(const char* string);

error_t devices_boot(const char* filename) {
    devices_create_list();
    FILE* file = fopen(filename, "r");
    if(!file) {
        LOG_ERROR("Can't open file %s, error: %d", filename, print_err());
        return EDXML;
    }
    LOG_INFO("Device boot procedure start!!!");
    devices_parse_xml(file);
    LOG_INFO("End of device boot");
    fclose(file);
    return SUCCESS;
}

static error_t devices_parse_xml(FILE* file) {
    XmlParserNodeS* device = NULL;
    while(1) {
        XmlParserNodeS* device_prev = device;
        device = xml_parser_search_next_object(file, "device", device_prev);
        xml_free_node(device_prev);
        if (!device) {
            /*break*/
            break;
        }
        else {
            char* name = xml_parser_get_value_parent(file, device, "name");
            char* protocol_s = xml_parser_get_value_parent(file, device, "protocol");
            protocol_t protocol = devices_get_protocol_from_string(protocol_s);
            if(protocol == PROTOCOL_UNKNOWN) {
                LOG_ERROR("Protocol is unknown string was: %s, skip device", protocol_s);
                free(name);
                free(protocol_s);
                continue;
            }
            DeviceS* new_device = devices_create(name, protocol);
            if(!new_device) {
                LOG_ERROR("Can't create new device!!");
                free(name);
                free(protocol_s);
                free(device);
                return EDBOOT;
            }
            error_t err = devices_add(new_device);
            if(err) {
                LOG_ERROR("Can't push new device to list %d was returned", err);
            }
            else {
                LOG_INFO("New device was added to device list: name = %s, protocol = %s", name, protocol_s);
            }
            free(name);
            free(protocol_s);
            if(thrd_create(&new_device->thread, devices_thread_run, new_device) != thrd_success) {
                LOG_ERROR("Can't create new thread for %s device", new_device->name);
                assert_ss("TODO: not implemented yet alternativi" == 0);
            }
            LOG_INFO("Thread created!");
        }
    }
    xml_free_node(device);
    return SUCCESS;
}

static inline protocol_t devices_get_protocol_from_string(const char* string) {
    if(!strcmp(string, "UART")) {
        return PROTOCOL_UART;
    }
    else if(!strcmp(string, "USB")) {
        return PROTOCOL_USB;
    }
    else if(!strcmp(string, "USART")) {
        return PROTOCOL_USART;
    }
    else if(!strcmp (string, "ETH")) {
        return PROTOCOL_ETH;
    }
    else if(!strcmp(string, "CUSTOM")) {
        return PROTOCOL_CUSTOM;
    }
    else {
        return PROTOCOL_UNKNOWN;
    }
}