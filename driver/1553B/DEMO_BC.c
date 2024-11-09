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
 * @file DEMO_BC.c
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

#include "BU61580.h"
#include "BU61580_hal.h"
#include "stdlib.h"
#include "string.h"

extern void usleep(unsigned long useconds);

static bool msg_err;
static bool msg_end;
static uint16_t R1553B_DATA[32];

int bc_isr(void *arg)
{
    BU61580_t *dev = (BU61580_t *)arg;

    static normalBCMsgDesc_t normal_BC_msg_desc;
    static bcCtrlWord_rtSubaddrCtrlWord_t ctrlWord;
    static cmdWord_t cmdWord;
    static stsWord_t stsWord;
    static uint16_t nowPos, initCnt, initPos, nowCnt;
    static uint16_t cmdStackPtr;
    static intMaskReg_t intMaskReg = {.all = 0U};
    static cfgReg1_t cfgReg1 = {.all = 0U};

    cmdWord.all = 0U;
    intMaskReg.all = 0U;
    cfgReg1.all = 0U;

    // 读中断状态
    bu61580_read_reg(dev, ADDR_REG_RO_INT_STATUS, &intMaskReg.all);

    if (intMaskReg.msg_end == 1U) // 判断消息结束
    {
        bu61580_read_reg(dev, ADDR_REG_WR_CONFIG_1, &cfgReg1.all);
        bu61580_read_reg(dev, ADDR_REG_RO_CMD_STACK_PTR, &cmdStackPtr);

        bu61580_read_mem(dev, ADDR_MEM_REGION_A_STACK_PTR, &nowPos);       // 获取当前栈指针
        bu61580_read_mem(dev, ADDR_MEM_REGION_A_MSG_CNT, &nowCnt);         // 获取当前栈指针
        bu61580_read_mem(dev, ADDR_MEM_REGION_A_INIT_STACK_PTR, &initPos); // 获取当前栈指针
        bu61580_read_mem(dev, ADDR_MEM_REGION_A_INIT_MSG_CNT, &initCnt);   // 获取当前栈指针

        bu61580_read_mem(dev, cmdStackPtr, &normal_BC_msg_desc.blockStatusWord.all);
        bu61580_read_mem(dev, cmdStackPtr + 1, &normal_BC_msg_desc.timeTagWord);
        bu61580_read_mem(dev, cmdStackPtr + 2, &normal_BC_msg_desc.msgGapTimeWord);
        bu61580_read_mem(dev, cmdStackPtr + 3, &normal_BC_msg_desc.msgBlockAddr);

        if (normal_BC_msg_desc.blockStatusWord.error_flag)
        {
            msg_err = true;
            return 0;
        }

        if (normal_BC_msg_desc.blockStatusWord.eom)
        {

            bu61580_read_mem(dev, normal_BC_msg_desc.msgBlockAddr, &ctrlWord.all);
            bu61580_read_mem(dev, normal_BC_msg_desc.msgBlockAddr + 1, &cmdWord.all);
            bu61580_read_mem(dev, normal_BC_msg_desc.msgBlockAddr + 3, &stsWord.all);

            if (cmdWord.dir)
            {
                for (size_t i = 0; i < (cmdWord.len ? cmdWord.len : 32); i++)
                    bu61580_read_mem(dev, normal_BC_msg_desc.msgBlockAddr + 4 + i, &R1553B_DATA[i]);
            }

            msg_end = true;
            return 1;
        }
    }
    return 0;
}

void bc_reg_init(BU61580_t *dev)
{
    buildInTestResReg_t buildInTestResReg = {.all = 0U};
    startResetReg_t startResetReg = {.all = 0U};
    intMaskReg_t intMaskReg = {.all = 0U};
    cfgReg1_t cfgReg1 = {.all = 0U};
    cfgReg2_t cfgReg2 = {.all = 0U};
    cfgReg3_t cfgReg3 = {.all = 0U};
    cfgReg4_t cfgReg4 = {.all = 0U};
    cfgReg5_t cfgReg5 = {.all = 0U};
    cfgReg6_t cfgReg6 = {.all = 0U};

    // 检查自动测试状态
    bu61580_read_reg(dev, ADDR_REG_RO_BUILT_IN_SELF_TEST_STAT, &buildInTestResReg.all);
    if (0xA800U != buildInTestResReg.all)
        return;

    // 复位
    startResetReg.reset = 1U;
    bu61580_write_reg(dev, ADDR_REG_WO_START_RESET, startResetReg.all);

    usleep(10000);

    // 配置寄存器 #3,D15-增强模式使能
    cfgReg3.enhanced_mode_enable = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_3, &cfgReg3.all);

    // 配置寄存器 #3
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_3, &cfgReg3.all);

    // 配置寄存器 #6
    cfgReg6.rt_addr_source = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_6, &cfgReg6.all);

    // 配置寄存器 #1
    cfgReg1.bc.mode = MODE_BC;
    //	cfgReg1.bc.frame_auto_repeat = 1U;
    cfgReg1.bc.internal_trigger_enabled = 1U;
    cfgReg1.bc.inter_msg_gap_timer_enabled = 1U;
    cfgReg1.bc.retry_enabled = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_1, &cfgReg1.all);

    // 配置寄存器 #2
    cfgReg2.enhanced_int = 1U;
    cfgReg2.rx_sa_double_buffer_enable = 1U;
    cfgReg2.int_sts_auto_clear = 1U;
    cfgReg2._256_word_boundary_disable = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_2, &cfgReg2.all);

    // 配置寄存器 #4
    cfgReg4.expanded_bc_control_word_enable = 1;
    cfgReg4._1st_retry_alt_same_bus = 1U;
    cfgReg4._2nd_retry_alt_same_bus = 1U;
    cfgReg4.valid_busy_no_data = 1U;
    cfgReg4.valid_me_no_data = 1U;
    cfgReg4.latch_rt_address_with_config5 = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_4, &cfgReg4.all);

    // 配置寄存器#5 D11-扩展过零点检测
    cfgReg5._12mhz_clock_select = 1U;
    cfgReg5.expanded_crossing_enabled = 1U;
    cfgReg5.rt_address_latch_transparent = 1U;
    cfgReg5.rt_address = 1U;
    cfgReg5.rt_address_parity = 0U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_5, &cfgReg5.all);

    // 中断屏蔽寄存器
    bu61580_write_read_reg(dev, ADDR_REG_WR_INT_MASK, &intMaskReg.all);

    // BC 帧时间 1s, 分辨率 10MHz -> 10us,40MHz -> 2.5us
    bu61580_write_reg(dev, ADDR_REG_WR_BC_FRAME_TIME, (uint16_t)16000U);

    // 打开 BC 消息帧结束中断
    intMaskReg.all = 0U;
    intMaskReg.info_frm_end = 1U;
    bu61580_write_reg(dev, ADDR_REG_WR_INT_MASK, intMaskReg.all);
}

void bc_msg_block_push(BU61580_t *dev, bcCtrlWord_rtSubaddrCtrlWord_t ctrlWord, cmdWord_t cmdWord, uint16_t *data, uint16_t *stack_ptr)
{
    static uint16_t s_blk_index = 0;
    static uint16_t s_stack_ptr = ADDR_MEM_STACK_A(0);
    static uint16_t s_data_ptr = ADDR_BC_DATA_BLOCK(0);

    {
        s_stack_ptr = ADDR_MEM_STACK_A(s_blk_index);
        s_data_ptr = ADDR_BC_DATA_BLOCK(s_blk_index);

        if ((stack_ptr != NULL))
            *stack_ptr = s_stack_ptr;

        bu61580_write_mem(dev, s_stack_ptr++, 0U);
        bu61580_write_mem(dev, s_stack_ptr++, 0U);
        bu61580_write_mem(dev, s_stack_ptr++, 0U);
        bu61580_write_mem(dev, s_stack_ptr++, s_data_ptr);

        for (size_t i = 0; i < SIZE_BC_DATA_BLOCK; i++)
        {
            bu61580_write_mem(dev, s_data_ptr + i, 0);
        }

        bu61580_write_mem(dev, s_data_ptr++, ctrlWord.all);
        bu61580_write_mem(dev, s_data_ptr++, cmdWord.all);

        // 数据
        if (!cmdWord.dir)
            for (size_t i = 0; i < (cmdWord.len ? cmdWord.len : 32); i++)
            {
                bu61580_write_mem(dev, s_data_ptr++, data[i]);
            }

        s_blk_index = (s_blk_index >= 63) ? 0 : (s_blk_index + 1);
    }
}

void bc_msg_start(BU61580_t *dev, uint16_t head_ptr, uint16_t cnt)
{
    static startResetReg_t startResetReg = {.all = 0U};

    bu61580_write_mem(dev, ADDR_MEM_REGION_A_STACK_PTR, head_ptr);        // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_A_MSG_CNT, 0xFFFF - cnt);      // set messages in a frame (0xffff - value) 这里是 1 个消息块，设置 0XFFFE
    bu61580_write_mem(dev, ADDR_MEM_REGION_A_INIT_STACK_PTR, head_ptr);   // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_A_INIT_MSG_CNT, 0xFFFF - cnt); // set messages in a frame (0xffff - value) 这里是 1 个消息块，设置 0XFFFE

    bu61580_write_mem(dev, ADDR_MEM_REGION_B_STACK_PTR, head_ptr);        // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_B_MSG_CNT, 0xFFFF - cnt);      // set messages in a frame (0xffff - value) 这里是 1 个消息块，设置 0XFFFE
    bu61580_write_mem(dev, ADDR_MEM_REGION_B_INIT_STACK_PTR, head_ptr);   // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_B_INIT_MSG_CNT, 0xFFFF - cnt); // set messages in a frame (0xffff - value) 这里是 1 个消息块，设置 0XFFFE

    startResetReg.all = 0U;
    startResetReg.bc_mt_start = 1U;
    bu61580_write_reg(dev, ADDR_REG_WO_START_RESET, startResetReg.all);
}

// BC 测试
void bc_test(BU61580_t *dev)
{

    // 寄存器初始化
    bc_reg_init(dev);

    uint16_t data[32];
    uint16_t stack_ptr;
    cmdWord_t cmdWord = {.all = 0U};
    bcCtrlWord_rtSubaddrCtrlWord_t ctrlWord = {.all = 0U};

    while (1)
    {
        for (size_t i = 1; i < 31; i++)
        {
            ctrlWord.bcCtrlWord.bus_channel_ab = 1U;
            ctrlWord.bcCtrlWord.retry_enabled = 0U;

            cmdWord.len = 0U;
            cmdWord.subAddr = i;
            cmdWord.rtAddr = 1U;
            cmdWord.dir = 0U;

            for (size_t j = 0; j < 32; j++)
                data[j] = i + j;

            bc_msg_block_push(dev, ctrlWord, cmdWord, data, &stack_ptr);

            msg_err = false;
            msg_end = false;
            bc_msg_start(dev, stack_ptr, 1);

            while (!bc_isr(dev))
            {
                if (msg_err || msg_end)
                    break;
            }

            if (msg_err)
            {
                // !TODO: do something here
                continue;
            }

            ctrlWord.bcCtrlWord.bus_channel_ab = 1U;
            ctrlWord.bcCtrlWord.retry_enabled = 0U;

            cmdWord.len = 0U;
            cmdWord.subAddr = i;
            cmdWord.rtAddr = 1U;
            cmdWord.dir = 1U;

            bc_msg_block_push(dev, ctrlWord, cmdWord, data, &stack_ptr);

            msg_err = false;
            msg_end = false;
            bc_msg_start(dev, stack_ptr, 1);

            while (!bc_isr(dev))
            {
                if (msg_err || msg_end)
                    break;
            }

            if (msg_err)
            {
                // !TODO: do something here
                continue;
            }

            if (msg_end)
            {
                for (uint16_t i = 1; i < 32; i++)
					if (R1553B_DATA[i] != data[i])
					{
						break;
					}
                // !TODO: do something here
                continue;
            }
        }
    }
}
