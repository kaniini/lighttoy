/*
 * Copyright (c) 2015 Tony Luo
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
 * copies of the Software, and to permit persons to whom the Software is furnished
 * to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LIBBULB__PROTOCOL_H
#define LIBBULB__PROTOCOL_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <unistd.h>

/*

Level 0 - Frame.

+------+------------+--------+----+---------+
| size | descriptor | source | id | payload |
+------+------------+--------+----+---------+

uint16_t size:
  Size of frame in bytes little endian. 65,535 bytes max.

uint16_t:
   0..11 - protocol: 12 bit unsigned integer. 4095 max.

      12 - addressable: Frame includes address.
       0 - Is lx_frame_t
       1 - Is lx_frame_address_t

      13 - tagged: Frame targets tag or device in lx_frame_address_t.target.
       0 - Targets a device.
       1 - Targets tags id.

  14..15 - origin: 2 bit reserved message origin indicator. For internal lifx use. Should be 0.

uint32_t source:
  Message source identifier from NAT table (Internal LIFX use)

*/
#pragma pack(push, 1)
typedef struct lx_frame_t {
  #define LX_FRAME_FIELDS   \
    uint16_t size;          \
    uint16_t protocol:12;   \
    uint8_t  addressable:1; \
    uint8_t  tagged:1;      \
    uint8_t  origin:2;      \
    uint32_t source;

  LX_FRAME_FIELDS
} lx_frame_t;
#pragma pack(pop)

/*

Level 1 - Frame with address.

+------+------------+--------+----+--------+----------+-----------+---------+
| size | descriptor | source | id | target | reserved | reserved2 | payload |
+------+------------+--------+----+--------+----------+-----------+---------+

Inherited from frame_t.

uint16_t size:
  As above.

uint16_t protocol/addressable/tagged:
  As above.

uint32_t source:
  As above.

uint8_t target[8]:
  Device or tag target within site.
  device: Target is a 6 byte value based on mac address of light [bytes 0-5].
  tag:    uint64_t bitmask of tag slots targetted. 0 matches all devices.

uint8_t reserved[6]:
    Reserved

uint8_t:
  0       - Response required
  1       - Acknowledgement required
  2..7    - Reserved for future use.

uint8_t sequence:
  Wrap around sequence number (Reserved for LIFX use)

*/
#pragma pack(push, 1)
typedef struct lx_frame_address_t {
  #define LX_FRAME_ADDRESS_FIELDS \
    LX_FRAME_FIELDS           \
    uint8_t  target[8];       \
    uint8_t  reserved[6];         \
    uint8_t  res_required:1;  \
    uint8_t  ack_required:1;  \
    uint8_t  :6;              \
    uint8_t  sequence;

  LX_FRAME_ADDRESS_FIELDS
} lx_frame_address_t;
#pragma pack(pop)


/* INTERNAL LIFX USE */
#define LX_FRAME_ORIGIN_LAN  (0x00)
#define LX_FRAME_ORIGIN_PAN  (0x01)
#define LX_FRAME_ORIGIN_WAN  (0x02)

// Size of a field inside a raw level 0 frame
#define lx_protocol_frame_field_sizeof(m) (sizeof(((lx_frame_address_t *)NULL)->m))

// Size of a complete message inside a level 1 protocol frame
#define lx_protocol_sizeof(s)             (offsetof(lx_protocol_t, payload) + sizeof(s))

// Size of a field inside a level 1 protocol message
#define lx_protocol_field_sizeof(s, m)    (sizeof(((lx_protocol_t *)NULL)->payload.s.m))

typedef enum lx_protocol_device_service_t {
    LX_PROTOCOL_DEVICE_SERVICE_UDP = 1
} lx_protocol_device_service_t;

#define LX_PROTOCOL_DEVICE_TYPES \
  X(LX_PROTOCOL_DEVICE_STATE_SERVICE, lx_protocol_device_state_service) \
  X(LX_PROTOCOL_DEVICE_STATE_HOST_INFO, lx_protocol_device_state_host_info) \
  X(LX_PROTOCOL_DEVICE_STATE_HOST_FIRMWARE, lx_protocol_device_state_host_firmware) \
  X(LX_PROTOCOL_DEVICE_STATE_WIFI_INFO, lx_protocol_device_state_wifi_info) \
  X(LX_PROTOCOL_DEVICE_STATE_WIFI_FIRMWARE, lx_protocol_device_state_wifi_firmware) \
  X(LX_PROTOCOL_DEVICE_SET_POWER, lx_protocol_device_set_power) \
  X(LX_PROTOCOL_DEVICE_STATE_POWER, lx_protocol_device_state_power) \
  X(LX_PROTOCOL_DEVICE_SET_LABEL, lx_protocol_device_set_label) \
  X(LX_PROTOCOL_DEVICE_STATE_LABEL, lx_protocol_device_state_label) \
  X(LX_PROTOCOL_DEVICE_STATE_VERSION, lx_protocol_device_state_version) \
  X(LX_PROTOCOL_DEVICE_STATE_INFO, lx_protocol_device_state_info) \
  X(LX_PROTOCOL_DEVICE_ECHO_REQUEST, lx_protocol_device_echo_request) \
  X(LX_PROTOCOL_DEVICE_ECHO_RESPONSE, lx_protocol_device_echo_response)

#pragma pack(push, 1)
typedef struct lx_protocol_device_state_service_t {
  uint8_t  service;
  uint32_t port;
} lx_protocol_device_state_service_t;
      
typedef struct lx_protocol_device_state_host_info_t {
  float    signal;
  uint32_t tx;
  uint32_t rx;
  int16_t  mcu_temperature;
} lx_protocol_device_state_host_info_t;

typedef struct lx_protocol_device_state_host_firmware_t {
  uint64_t build;
  uint64_t install;
  uint32_t version;
} lx_protocol_device_state_host_firmware_t;

typedef struct lx_protocol_device_state_wifi_info_t {
  float    signal;
  uint32_t tx;
  uint32_t rx;
  int16_t  mcu_temperature;
} lx_protocol_device_state_wifi_info_t;

typedef struct lx_protocol_device_state_wifi_firmware_t {
  uint64_t build;
  uint64_t install;
  uint32_t version;
} lx_protocol_device_state_wifi_firmware_t;

typedef struct lx_protocol_device_set_power_t {
  uint16_t level;
} lx_protocol_device_set_power_t;

typedef struct lx_protocol_device_state_power_t {
  uint16_t level;
} lx_protocol_device_state_power_t;

typedef struct lx_protocol_device_set_label_t {
  uint8_t  label[32];
} lx_protocol_device_set_label_t;

typedef struct lx_protocol_device_state_label_t {
  uint8_t  label[32];
} lx_protocol_device_state_label_t;

typedef struct lx_protocol_device_state_version_t {
  uint32_t vendor;
  uint32_t product;
  uint32_t version;
} lx_protocol_device_state_version_t;

typedef struct lx_protocol_device_state_info_t {
  uint64_t time;
  uint64_t uptime;
  uint64_t downtime;
} lx_protocol_device_state_info_t;

typedef struct lx_protocol_device_echo_request_t {
  uint8_t  payload[64];
} lx_protocol_device_echo_request_t;

typedef struct lx_protocol_device_echo_response_t {
  uint8_t  payload[64];
} lx_protocol_device_echo_response_t;
#pragma pack(pop)

/*
  lx_protocol_light
*/

#define LX_PROTOCOL_LIGHT_TYPES \
  X(LX_PROTOCOL_LIGHT_HSBK, lx_protocol_light_hsbk) \
  X(LX_PROTOCOL_LIGHT_SET_COLOR, lx_protocol_light_set_color) \
  X(LX_PROTOCOL_LIGHT_SET_POWER, lx_protocol_light_set_power) \
  X(LX_PROTOCOL_LIGHT_STATE_POWER, lx_protocol_light_state_power) \
  X(LX_PROTOCOL_LIGHT_STATE, lx_protocol_light_state) \

#pragma pack(push, 1)
typedef struct lx_protocol_light_hsbk_t {
  uint16_t hue;
  uint16_t saturation;
  uint16_t brightness;
  uint16_t kelvin;
} lx_protocol_light_hsbk_t;

typedef struct lx_protocol_light_set_color_t {
  uint8_t  stream;
  lx_protocol_light_hsbk_t color;
  uint32_t duration;
} lx_protocol_light_set_color_t;

typedef struct lx_protocol_light_set_power_t {
  uint16_t level;
  uint32_t duration;
} lx_protocol_light_set_power_t;

typedef struct lx_protocol_light_state_power_t {
  uint16_t level;
} lx_protocol_light_state_power_t;

typedef struct lx_protocol_light_state_t {
  lx_protocol_light_hsbk_t color;
  int16_t  dim;
  uint16_t power;
  uint8_t  label[32];
  uint64_t tags;
} lx_protocol_light_state_t;
#pragma pack(pop)

typedef enum lx_protocol_type_t {
  LX_PROTOCOL_DEVICE_GET_SERVICE = 2,
  LX_PROTOCOL_DEVICE_STATE_SERVICE = 3,
  LX_PROTOCOL_DEVICE_GET_HOST_INFO = 12,
  LX_PROTOCOL_DEVICE_STATE_HOST_INFO = 13,
  LX_PROTOCOL_DEVICE_GET_HOST_FIRMWARE = 14,
  LX_PROTOCOL_DEVICE_STATE_HOST_FIRMWARE = 15,
  LX_PROTOCOL_DEVICE_GET_WIFI_INFO = 16,
  LX_PROTOCOL_DEVICE_STATE_WIFI_INFO = 17,
  LX_PROTOCOL_DEVICE_GET_WIFI_FIRMWARE = 18,
  LX_PROTOCOL_DEVICE_STATE_WIFI_FIRMWARE = 19,
  LX_PROTOCOL_DEVICE_GET_POWER = 20,
  LX_PROTOCOL_DEVICE_SET_POWER = 21,
  LX_PROTOCOL_DEVICE_STATE_POWER = 22,
  LX_PROTOCOL_DEVICE_GET_LABEL = 23,
  LX_PROTOCOL_DEVICE_SET_LABEL = 24,
  LX_PROTOCOL_DEVICE_STATE_LABEL = 25,
  LX_PROTOCOL_DEVICE_GET_VERSION = 32,
  LX_PROTOCOL_DEVICE_STATE_VERSION = 33,
  LX_PROTOCOL_DEVICE_GET_INFO = 34,
  LX_PROTOCOL_DEVICE_STATE_INFO = 35,
  LX_PROTOCOL_DEVICE_ACKNOWLEDGEMENT = 45,
  LX_PROTOCOL_DEVICE_ECHO_REQUEST = 58,
  LX_PROTOCOL_DEVICE_ECHO_RESPONSE = 59,
  LX_PROTOCOL_LIGHT_GET = 101,
  LX_PROTOCOL_LIGHT_SET_COLOR = 102,
  LX_PROTOCOL_LIGHT_STATE = 107,
  LX_PROTOCOL_LIGHT_GET_POWER = 116,
  LX_PROTOCOL_LIGHT_SET_POWER = 117,
  LX_PROTOCOL_LIGHT_STATE_POWER = 118,
} lx_protocol_type_t;

#pragma pack(push, 1)
typedef struct lx_protocol_t {
  LX_FRAME_ADDRESS_FIELDS
  uint64_t reserved64;
  uint16_t type;
  uint16_t reserved16;
  union {
    #define X(constant, name) name ## _t name;
    LX_PROTOCOL_DEVICE_TYPES \
    LX_PROTOCOL_LIGHT_TYPES
    #undef X
  } payload;
} lx_protocol_t;
#pragma pack(pop)

#endif
