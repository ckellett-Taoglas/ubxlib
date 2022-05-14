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

#ifndef _U_NETWORK_H_
#define _U_NETWORK_H_

/* Only header files representing a direct and unavoidable
 * dependency between the API of this module and the API
 * of another module should be included here; otherwise
 * please keep #includes to your .c files. */

#include "u_device.h"

/** @file
 * @brief This header file defines the network API. These functions are
 * thread-safe.  TODO: maybe not!
 *
 * The functions here should be used in conjunction with those in the
 * uDevice API in the following sequence; think of it as "ready, steady,
 * go ... off".
 *
 * uNetworkInit():          call this at start of day in order to make
 *                          this API available: READY.
 * uDeviceOpen():           call this with a pointer to a const structure
 *                          containing the physical configuration for the
 *                          device (module type, physical interface (UART
 *                          etc.), pins used, etc.): when the function
 *                          returns the module is powered-up and ready to
 *                          support a network: STEADY.
 * uNetworkInterfaceUp():   call this with the device handle and a pointer
 *                          to a const structure containing the network
 *                          configuration (e.g. SSID in the case of Wifi,
 *                          APN in the case of cellular, etc.) when you
 *                          would like the network to connect; after this
 *                          is called you can send and receive stuff over
 *                          the network: GO.
 * uNetworkInterfaceDown(): disconnect and shut-down the network; once this
 *                          has returned the module may enter a lower-power
 *                          or powered-off state: you must call
 *                          uNetworkInterfaceUp() to talk with it again: OFF.
 * uDeviceClose():          call this to clear up any resources belonging to
 *                          the network; once this is called uDeviceOpen()
 *                          must be called re-instantiate the device.
 * uNetworkDeinit():        call this at end of day in order to clear up any
 *                          resources owned by this API.
 *                          TODO: state whether this does any form of
 *                          device closing or not [not sure it is able to].
 */

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------------------------------------------------
 * COMPILE-TIME MACROS
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * TYPES
 * -------------------------------------------------------------- */

/** Network types.
 */
//lint -estring(788, uNetworkType_t::U_NETWORK_TYPE_MAX_NUM)
//lint -estring(788, uNetworkType_t::U_NETWORK_TYPE_NONE)
//  Suppress not used within defaulted switch
typedef enum {
    U_NETWORK_TYPE_NONE,
    U_NETWORK_TYPE_BLE,
    U_NETWORK_TYPE_CELL,
    U_NETWORK_TYPE_WIFI,
    U_NETWORK_TYPE_GNSS,
    U_NETWORK_TYPE_MAX_NUM
} uNetworkType_t;

/** A version number for the network configuration structure. In
 * general you should allow the compiler to initialise any variable
 * of this type to zero and ignore it.  It is only set to a value
 * other than zero when variables in a new and extended version of
 * the structure it is a part of are being used, the version number
 * being employed by this code to detect that and, more importantly,
 * to adopt default values for any new elements when the version
 * number is STILL ZERO, maintaining backwards compatibility with
 * existing application code.  The structure this is a part of will
 * include instructions as to when a non-zero version number should
 * be set.
 */
typedef int32_t uNetworkCfgVersion_t;

/* ----------------------------------------------------------------
 * FUNCTIONS
 * -------------------------------------------------------------- */

/** Initialise the network API.  If the network API has already
 * been initialised this function returns success without doing
 * anything.
 *
 * @return  zero on success else negative error code.
 */
int32_t uNetworkInit();

/** Deinitialise the network API.  Any network instances will
 * be removed internally with a call to uNetworkRemove().
 * TODO: QUESTION: this used to remove all the networks, i.e.
 * to do what is now uDeviceClose() also.  It could do that
 * because there was a linked list of what used to be networks
 * and which are now devices/networks.  Since [I think] that
 * list is now gone (because the user carries the context around
 * via the device handle), I don't believe it can do so anymore;
 * this is a behaviour change for the customer that we need to
 * highlight very clearly as they will otherwise have memory leaks;
 * this will have an effect on all of the examples also, since
 * they expect such a clean-up to happen.
 */
void uNetworkDeinit();

/** TODO: WILL BE REMOVED: functionality will be in uDeviceOpen().
 * Add a network instance. When this returns successfully
 * the module is powered up and available for configuration but
 * is not yet connected to anything.
 *
 * @param type             the type of network to create,
 *                         BLE, Wifi, cellular, etc.
 * @param pConfiguration   a pointer to the configuration
 *                         information for the given network
 *                         type.  This must be stored
 *                         statically, a true constant: the
 *                         contents are not copied by this
 *                         function. The configuration
 *                         structures are defined by this
 *                         API in the u_network_xxx.h header
 *                         files and have the name
 *                         uNetworkConfigurationXxx_t, where
 *                         xxx is replaced by one of Cell,
 *                         Ble or Wifi.  The configuration
 *                         is passed transparently through to
 *                         the given API, hence the use of
 *                         void * here. The first entry in
 *                         all of these structures is of type
 *                         uNetworkType_t to indicate the
 *                         type and allow cross-checking.
 * @param[out] pDevHandle  a pointer to the output handle.
 *                         Will only be set on success.
 *                         This handle may also
 *                         be used with the underlying sho/cell
 *                         API to perform operations that
 *                         cannot be carried out through
 *                         this network API.
 * @return                 zero on success or negative error
 *                         code on failure.
 */
int32_t uNetworkAdd(uNetworkType_t type,
                    const void *pConfiguration,
                    uDeviceHandle_t *pDevHandle);

/** TODO: WILL BE REMOVED: functionality will be in uDeviceClose().
 * Remove a network instance.  It is up to the caller to ensure
 * that the network in question is disconnected and/or powered
 * down etc.; all this function does is remove the logical
 * instance, clearing up resources.
 *
 * @param devHandle  the handle of the device to remove.
 * @return           zero on success else negative error code.
 */
int32_t uNetworkRemove(uDeviceHandle_t devHandle);

/** TODO: WILL BE REMOVED: functionality will be in uNetworkInterfaceUp().
 * Bring up the given network instance, connecting it as defined
 * in the configuration passed to uNetworkAdd().  If the network
 * is already up the implementation should return success without
 * doing anything.
 *
 * @param devHandle the handle of the device to bring up.
 * @return          zero on success else negative error code.
 */
int32_t uNetworkUp(uDeviceHandle_t devHandle);

/** TODO: WILL BE REMOVED: functionality will be in uNetworkInterfaceDown().
 * Take down the given network instance, disconnecting
 * it from any peer entity.  After this function returns
 * uNetworkUp() must be called once more to ensure that the
 * module is brought back to a responsive state.
 *
 * @param devHandle the handle of the device to take down.
 * @return          zero on success else negative error code.
 */
int32_t uNetworkDown(uDeviceHandle_t devHandle);

/** Bring up the given network interface on a device, connecting it as defined
 * in the configuration passed to uNetworkConfigure(). If the network
 * is already up the implementation should return success without
 * doing anything.
 *
 * @param devHandle        the handle of the device to bring up.
 * @param netType          which of the module interfaces.
 * @param pConfiguration   a pointer to the configuration
 *                         information for the given network
 *                         type.  This must be stored
 *                         statically, a true constant: the
 *                         contents are not copied by this
 *                         function. The configuration
 *                         structures are defined by this
 *                         API in the u_network_xxx.h header
 *                         files and have the name
 *                         uNetworkConfigurationXxx_t, where
 *                         xxx is replaced by one of Cell,
 *                         Ble or Wifi.  The configuration
 *                         is passed transparently through to
 *                         the given API, hence the use of
 *                         void * here. The second entry in
 *                         all of these structures is of type
 *                         uNetworkType_t to indicate the
 *                         type and allow cross-checking.
 *                         Can be set to NULL on subsequent calls
 *                         if the configuration is unchanged.
 * @return                 zero on success else negative error code.
 */
int32_t uNetworkInterfaceUp(uDeviceHandle_t devHandle, uNetworkType_t netType,
                            const void *pConfiguration);

/** Take down the given network interface on a device, disconnecting
 * it from any peer entity.  After this function returns
 * uNetworkInterfaceUp() must be called once more to ensure that the
 * module is brought back to a responsive state.
 *
 * @param devHandle the handle of the device to take down.
 * @param netType   which of the module interfaces.
 * @return          zero on success else negative error code.
 */
int32_t uNetworkInterfaceDown(uDeviceHandle_t devHandle, uNetworkType_t netType);

#ifdef __cplusplus
}
#endif

#endif // _U_NETWORK_H_

// End of file
