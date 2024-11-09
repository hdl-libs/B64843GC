// ---------------------------------------------------------------------------------------
// Copyright (c) 2024 john_tito All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// ---------------------------------------------------------------------------------------

/**
 * @file BU61580_hal.h
 * @brief
 * @author
 */

/******************************************************************************/
/************************ Copyright *******************************************/
/******************************************************************************/

#ifndef _BU61580_HAL_H_
#define _BU61580_HAL_H_

/******************************************************************************/
/************************ Include Files ***************************************/
/******************************************************************************/

#include <stdbool.h>
#include <stdint.h>
#include "BU61580.h"
/******************************************************************************/
/************************ Marco Definitions ***********************************/
/******************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************/
    /************************ Types Definitions ***********************************/
    /******************************************************************************/

    typedef struct BU61580_t
    {
        uint32_t ADDR_REG;
        uint32_t ADDR_MEM;
    } BU61580_t;

    /******************************************************************************/
    /************************ Functions Declarations ******************************/
    /******************************************************************************/
    extern int bu61580_init(BU61580_t **dev_ptr, uint32_t baseAddress);
    extern int bu61580_deinit(BU61580_t **dev_ptr);

    extern int bu61580_read_mem(BU61580_t *dev, uint32_t offset, uint16_t *data);
    extern int bu61580_write_mem(BU61580_t *dev, uint32_t offset, uint16_t data);
    extern int bu61580_read_reg(BU61580_t *dev, uint32_t offset, uint16_t *data);
    extern int bu61580_write_reg(BU61580_t *dev, uint32_t offset, uint16_t data);

    extern int bu61580_read_write_reg(BU61580_t *dev, uint32_t offset, uint16_t *data);
    extern int bu61580_write_read_reg(BU61580_t *dev, uint32_t offset, uint16_t *data);

    /******************************************************************************/
    /************************ Variable Declarations *******************************/
    /******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif // _BU61580_HAL_H_
