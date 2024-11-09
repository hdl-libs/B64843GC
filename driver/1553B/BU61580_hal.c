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
 * @file BU61580_hal.c
 * @brief
 * @author
 *
 * Revision History :
 * ----------  -------  -----------  ----------------------------
 * Date        Version  Author       Description
 * ----------  -------  -----------  ----------------------------
 * 2024.11.09  1.0.0    johntito
 * ----------  -------  -----------  ----------------------------
**/

#include "BU61580_hal.h"
#include "xil_io.h"
#include <stdlib.h>

/**
 * @name bu61580_write_reg
 * @brief write a word to regster space
 * @param dev the pointer points to the device struct
 * @param offset the offset of address in word
 * @param data the data write to the address
 */
int bu61580_write_reg(BU61580_t *dev, uint32_t offset, uint16_t data)
{
    if (NULL == dev)
    {
        return -1;
    }
    if(offset <= 0x2000)
    	Xil_Out32(dev->ADDR_REG + (offset << 2U), data);
    else
    	return -2;
    return 0;
}

/**
 * @name bu61580_read_reg
 * @brief read a word from regster space
 * @param dev the pointer points to the device struct
 * @param offset the offset of address in word
 * @param data the pointer points to the data
 */
int bu61580_read_reg(BU61580_t *dev, uint32_t offset, uint16_t *data)
{
    if (NULL == dev)
    {
        return -1;
    }
    if(offset <= 0x2000)
    	*data = Xil_In32(dev->ADDR_REG + (offset << 2U));
    else
    	return -2;
    return 0;
}

/**
 * @name bu61580_write_mem
 * @brief write a word to memory space
 * @param dev the pointer points to the device struct
 * @param offset the offset of address in word
 * @param data the data write to the address
 */
int bu61580_write_mem(BU61580_t *dev, uint32_t offset, uint16_t data)
{
    if (NULL == dev)
    {
        return -1;
    }
    if(offset <= 0x2000)
    	Xil_Out32(dev->ADDR_MEM + (offset << 2U), data);
    else
    	return -2;
    return 0;
}

/**
 * @name bu61580_read_mem
 * @brief read a word from memory space
 * @param dev the pointer points to the device struct
 * @param offset the offset of address in word
 * @param data the pointer points to the data
 */
int bu61580_read_mem(BU61580_t *dev, uint32_t offset, uint16_t *data)
{
    if (NULL == dev)
    {
        return -1;
    }
    if(offset <= 0x2000)
    	*data = Xil_In32(dev->ADDR_MEM + (offset << 2U));
    else
    	return -2;
    return 0;
}

// write data to register and readback
int bu61580_write_read_reg(BU61580_t *dev, uint32_t offset, uint16_t *data)
{
    if (NULL == dev)
    {
        return -1;
    }
    bu61580_write_reg(dev, offset, *data);
    bu61580_read_reg(dev, offset, data);
    return 0;
}

// read data from register and logic "or" to input data then write to register
int bu61580_read_write_reg(BU61580_t *dev, uint32_t offset, uint16_t *data)
{
    if (NULL == dev)
    {
        return -1;
    }

    uint16_t tmp;
    bu61580_read_reg(dev, offset, &tmp);
    *data = *data | tmp;
    bu61580_write_reg(dev, offset, *data);
    return 0;
}

/**
 * @name bu61580_init
 * @brief this api will malloc a pointer for user
 * and init the base address
 * @param dev_ptr the pointer points to the device struct pointer
 * @param baseAddress the base address of the periph
 */
int bu61580_init(BU61580_t **dev_ptr, uint32_t baseAddress)
{
    *dev_ptr = (BU61580_t *)calloc(1, sizeof(BU61580_t));

    if (NULL == *dev_ptr)
    {
        return -1;
    }

    /**
     * 对于字节寻址的系统地址需要左移一位，
     * 本设计中将每个字映射到一个 32 位地址上，
     * 因此实际寻址时地址需要左移两位
     */
    (*dev_ptr)->ADDR_REG = baseAddress;                 // 17 WORD REG : 0x0000-0x7FFF
    (*dev_ptr)->ADDR_MEM = baseAddress + (0x2000 << 2); // 8K WORD MEM : 0x8000-0xFFFF
    return 0;
}

/**
 * @name bu61580_deinit
 * @brief this api will free the device pointer
 * @param dev_ptr the pointer points to the device struct pointer
 */
int bu61580_deinit(BU61580_t **dev_ptr)
{
    if (*dev_ptr)
    {
        free(dev_ptr);
        *dev_ptr = NULL;
    }
    return 0;
}
