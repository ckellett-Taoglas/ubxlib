/*
 * Copyright 2020 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* Only #includes of u_* and the C standard library are allowed here,
 * no platform stuff and no OS stuff.  Anything required from
 * the platform/OS must be brought in through u_port* to maintain
 * portability.
 */

/** @file
 * @brief Stub of the ble portion of the network API.
 * Include this if the ble network is not used in the application.
 */

#include "stddef.h"    // NULL, size_t etc.
#include "stdint.h"    // int32_t etc.
#include "stdbool.h"

#include "u_error_common.h"

#include "u_network.h"
#include "u_network_config_ble.h"
#include "u_network_private_ble.h"

/* ----------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------- */

// TODO since we're changing things, rename this to
// uNetworkPrivateInitBle() for consistency?
int32_t uNetworkInitBle(void)
{
    return (int32_t) U_ERROR_COMMON_NOT_IMPLEMENTED;
}

// TODO since we're changing things, rename this to
// uNetworkPrivateDeinitBle() for consistency?
void uNetworkDeinitBle(void)
{
}

// TODO: WILL BE REMOVED.
int32_t uNetworkAddBle(const uNetworkConfigurationBle_t *pConfiguration,
                       uDeviceHandle_t *pDevHandle)
{
    (void) pConfiguration;
    (void) pDevHandle;
    return (int32_t) U_ERROR_COMMON_NOT_IMPLEMENTED;
}

// TODO: WILL BE REMOVED.
int32_t uNetworkRemoveBle(uDeviceHandle_t devHandle)
{
    (void) devHandle;
    return (int32_t) U_ERROR_COMMON_NOT_IMPLEMENTED;
}

// TODO: WILL BE REMOVED.
int32_t uNetworkUpBle(uDeviceHandle_t devHandle,
                      const uNetworkConfigurationBle_t *pConfiguration)
{
    (void) devHandle;
    (void) pConfiguration;
    return (int32_t) U_ERROR_COMMON_NOT_IMPLEMENTED;
}

// TODO: WILL BE REMOVED.
int32_t uNetworkDownBle(uDeviceHandle_t devHandle,
                        const uNetworkConfigurationBle_t *pConfiguration)
{
    (void) devHandle;
    (void) pConfiguration;
    return (int32_t) U_ERROR_COMMON_NOT_IMPLEMENTED;
}

// TODO rename this to uNetworkPrivateChangeStateBle() for consistency?
int32_t uNetworkChangeStateBle(uDeviceHandle_t devHandle,
                               uDeviceNetworkCfgBle_t *pCfg, bool up)
{
    (void) devHandle;
    (void) pCfg;
    (void) up;
    return (int32_t) U_ERROR_COMMON_NOT_IMPLEMENTED;
}

// End of file
