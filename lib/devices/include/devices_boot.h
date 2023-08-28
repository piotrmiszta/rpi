#ifndef DEVICES_BOOT_H_
#define DEVICES_BOOT_H_
#include "error_codes.h"
#include "types.h"
#include "xml_parser.h"
/**
 * @brief searching devices specified in xml file
 *          files/server/devices/devices.xml
*/
error_t devices_boot(const char* filename);

#endif // DEVICES_BOOT_H_
