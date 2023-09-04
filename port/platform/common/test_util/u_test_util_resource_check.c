/*
 * Copyright 2019-2023 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file
 * @brief  Functions to check for leakage of heap, OS resources (tasks etc.)
 * and transports (UARTs etc.).
 */

#ifdef U_CFG_OVERRIDE
# include "u_cfg_override.h" // For a customer's configuration override
#endif

#include "stddef.h"    // NULL, size_t etc.
#include "stdint.h"    // int32_t etc.
#include "stdbool.h"

#include "u_cfg_sw.h"
#include "u_cfg_os_platform_specific.h"  // For #define U_CFG_OS_CLIB_LEAKS
#include "u_cfg_app_platform_specific.h"
#include "u_cfg_test_platform_specific.h"

#include "u_error_common.h"

#include "u_port.h"
#include "u_port_os.h"
#include "u_port_heap.h"
#include "u_port_debug.h"
#include "u_port_uart.h"
#include "u_port_i2c.h"
#include "u_port_spi.h"

#ifdef U_CFG_TEST_ENABLE_INACTIVITY_DETECTOR
#include "u_debug_utils.h"
#endif
#ifdef U_CFG_MUTEX_DEBUG
#include "u_mutex_debug.h"
#endif

#include "u_test_util_resource_check.h"

/* ----------------------------------------------------------------
 * COMPILE-TIME MACROS
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * TYPES
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * VARIABLES
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * STATIC FUNCTIONS
 * -------------------------------------------------------------- */

/* ----------------------------------------------------------------
 * PUBLIC FUNCTIONS
 * -------------------------------------------------------------- */

// Check that resources are within limits and have been cleaned up.
bool uTestUtilResourceCheck(const char *pPrefix,
                            const char *pErrorMarker,
                            bool printIt)
{
    bool resourcesClean = true;
    int32_t x;
    int32_t osShouldBeOutstanding = 0;

    if (pPrefix == NULL) {
        pPrefix = "";
    }

    if (pErrorMarker == NULL) {
        pErrorMarker = "";
    }

#if defined(U_CFG_TEST_ENABLE_INACTIVITY_DETECTOR) && !defined(ARDUINO)
    osShouldBeOutstanding += U_DEBUG_UTILS_OS_RESOURCES;
#endif
#ifdef U_CFG_MUTEX_DEBUG
    osShouldBeOutstanding += U_MUTEX_DEBUG_OS_RESOURCES;
#endif
    osShouldBeOutstanding += U_PORT_HEAP_MONITOR_OS_RESOURCES;

    // Check main task stack against our limit
    x = uPortTaskStackMinFree(NULL);
    if (x != (int32_t) U_ERROR_COMMON_NOT_SUPPORTED) {
        if (x < U_CFG_TEST_OS_MAIN_TASK_MIN_FREE_STACK_BYTES) {
            resourcesClean = false;
        }
        if (printIt) {
            uPortLog("%s%smain task stack had a minimum of %d byte(s) free"
                     " (minimum is %d).\n", pPrefix,
                     resourcesClean ? "" : pErrorMarker,
                     x, U_CFG_TEST_OS_MAIN_TASK_MIN_FREE_STACK_BYTES);
        }
    }

    // Check all-time heap usage against our limit
    x = uPortGetHeapMinFree();
    if (x >= 0) {
        if (x < U_CFG_TEST_HEAP_MIN_FREE_BYTES) {
            resourcesClean = false;
        }
        if (printIt) {
            uPortLog("%s%sheap had a minimum of %d byte(s) free"
                     " (minimum is %d).\n", pPrefix,
                     resourcesClean ? "" : pErrorMarker,
                     x, U_CFG_TEST_HEAP_MIN_FREE_BYTES);
        }
    }

    // Check that all heap pUPortMalloc()s have uPortFree()s
    x = uPortHeapAllocCount();
    if (x > 0) {
        if (printIt) {
            uPortLog("%s%s%d outstanding call(s) to pUPortMalloc().\n",
                     pPrefix, pErrorMarker, x);
            uPortHeapDump(pPrefix);
        }
        resourcesClean = false;
    }

    // Check that all OS resources have been free'd
    x = uPortOsResourceAllocCount();
    if (x != osShouldBeOutstanding) {
        if (printIt) {
            uPortLog("%s%sexpected %d outstanding OS resource(s) (tasks etc.)"
                     " but got %d; they might yet be cleaned up.\n",
                     pPrefix, pErrorMarker, osShouldBeOutstanding, x);
        }
        resourcesClean = false;
    }
    x = uPortUartResourceAllocCount();
    if (x > 0) {
        if (printIt) {
            uPortLog("%s%s%d UART resource(s) outstanding.\n", pPrefix, pErrorMarker, x);
        }
        resourcesClean = false;
    }
    x = uPortI2cResourceAllocCount();
    if (x > 0) {
        if (printIt) {
            uPortLog("%s%s%d I2C resource(s) outstanding.\n", pPrefix, pErrorMarker, x);
        }
        resourcesClean = false;
    }
    x = uPortSpiResourceAllocCount();
    if (x > 0) {
        if (printIt) {
            uPortLog("%s%s%d SPI resource(s) outstanding.\n", pPrefix, pErrorMarker, x);
        }
        resourcesClean = false;
    }
    if (resourcesClean && printIt) {
        uPortLog("%sresources are good (%d outstanding OS resource(s), as expected).\n",
                 pPrefix, osShouldBeOutstanding);
    }

    return resourcesClean;
}

// End of file