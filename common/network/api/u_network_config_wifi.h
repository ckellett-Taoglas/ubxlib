/*
 * Copyright 2020 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _U_NETWORK_CONFIG_WIFI_H_
#define _U_NETWORK_CONFIG_WIFI_H_

/* Only header files representing a direct and unavoidable
 * dependency between the API of this module and the API
 * of another module should be included here; otherwise
 * please keep #includes to your .c files. */

/* This header file defines the configuration structure for the
 * network API for Wifi.
 */

/* ----------------------------------------------------------------
 * COMPILE-TIME MACROS
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * TYPES
 * -------------------------------------------------------------- */

/* Note: try, wherever possible, to use only basic types in this
 * structure (i.e. int32_t, const char, bool, etc.) since otherwise
 * your Wifi headers will have to be brought in to all files that
 * need this config type, so into all network examples, etc.
 * irrespective of whether Wifi is used there.
 */

/** TODO: WILL BE REMOVED: the device-related stuff is in uDevice
 * and the network-related stuff is in the new network cfg struct.
 * The network configuration for Wifi.  Note that any pin
 * numbers are those of the MCU: if you are using an MCU inside
 * a u-blox module the IO pin numbering for the module is likely
 * different to that from the MCU: check the data sheet for the
 * module to determine the mapping.
 */
typedef struct {
    uNetworkType_t type; /**< All uNetworkConfigurationXxx structures
                              must begin with this for error checking
                              purposes. */
    int32_t module; /**< The module type that is connected,
                         see uShortRangeModuleType_t in u_short_range.h. */
    int32_t uart; /**< The UART HW block to use. */
    int32_t pinTxd; /** The output pin that sends UART data to
                        the cellular module. */
    int32_t pinRxd; /** The input pin that receives UART data from
                        the cellular module. */
    int32_t pinCts; /**< The input pin that the cellular module
                         will use to indicate that data can be sent
                         to it; use -1 if there is no such connection. */
    int32_t pinRts; /**< The output pin output pin that tells the
                         cellular module that it can send more UART
                         data; use -1 if there is no such connection. */
    const char *pSsid; /**< The access point SSID to connect */
    int32_t authentication; /**< The access point authentication mode. Values are:
                                 1: Open (No authentication)
                                 2: WPA/WPA2/WPA3-PSK */
    const char *pPassPhrase; /**< WPA/WPA2/WPA3 passphrase - should be NULL for open */
} uNetworkConfigurationWifi_t;

// TODO will eventually be renamed to uNetworkCfgWifi_t, since
// it is actually nothing to do with the device stuff.
/** The network configuration for Wifi.
 */
typedef struct {
    uNetworkCfgVersion_t version; /**< Version of this network
                                       configuration; allow your
                                       compiler to initialise this
                                       to zero unless otherwise
                                       specified below. */
    uNetworkType_t type;     /**< For error checking purposes. */
    const char *pSsid;       /**< The access point SSID to connect */
    int32_t authentication;  /**< The access point authentication mode. Values are:
                                  1: Open (No authentication)
                                  2: WPA/WPA2/WPA3-PSK */
    const char *pPassPhrase; /**< WPA/WPA2/WPA3 passphrase - should be NULL for open */
    /* This is the end of version 0 of this
       structure: should any fields be added to
       this structure in future they must be
       added AFTER this point and instructions
       must be given against each one as to how
       to set the version field if any of the
       new fields are populated. For example, if
       int32_t magic were added, the comment
       against it might end with the clause "; if this
       field is populated then the version field
       of this structure must be set to 1 or higher". */
} uDeviceNetworkCfgWifi_t;

#endif // _U_NETWORK_CONFIG_WIFI_H_

// End of file
