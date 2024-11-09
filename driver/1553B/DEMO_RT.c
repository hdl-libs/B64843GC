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
 * @file DEMO_RT.c
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

typedef struct RTRecvMsg_t
{
    RTMsgDesc_t rtMessageBlock;
    uint16_t data[32];
} RTRecvMsg_t;

void rt_isr(void *arg)
{
    BU61580_t *dev = (BU61580_t *)arg;

    static uint16_t data_len;
    static uint16_t tx_cnt = 0;
    static bcCtrlWord_rtSubaddrCtrlWord_t subaddr_ctrl_word;
    static intMaskReg_t int_reg = {.all = 0U};
    static cfgReg1_t cfgReg1 = {.all = 0U};
    static RTRecvMsg_t recv_msg;
    static uint16_t rt_cmd_stack_ptr = 0;

    // 读中断状态
    bu61580_read_reg(dev, ADDR_REG_RO_INT_STATUS, &int_reg.all);

    if (int_reg.msg_end) // 判断消息结束
    {

    	// 读“配置寄存器#1”确定当前消息通道是来自 A 或 B
        bu61580_read_reg(dev, ADDR_REG_RO_CMD_STACK_PTR, &rt_cmd_stack_ptr); // 读 RT 命令栈指针寄存器，当前 RT 块状态字
        bu61580_read_reg(dev, ADDR_REG_WR_CONFIG_1, &cfgReg1.all);

        if(cfgReg1.rt0.current_area_b_a)
        	bu61580_read_mem(dev, ADDR_MEM_SACW_SA(1, recv_msg.rtMessageBlock.cmdWord.subAddr), &subaddr_ctrl_word.all);        // 子地址控制字查询表初始化
        else
        	bu61580_read_mem(dev, ADDR_MEM_SACW_SA(0, recv_msg.rtMessageBlock.cmdWord.subAddr), &subaddr_ctrl_word.all);        // 子地址控制字查询表初始化

        bu61580_read_mem(dev, rt_cmd_stack_ptr, &recv_msg.rtMessageBlock.blockStatusWord.all); // 读 RT 命令栈指针寄存器，当前 RT 块状态字
        bu61580_read_mem(dev, rt_cmd_stack_ptr+1, &recv_msg.rtMessageBlock.timeTagWord);              // 时标寄存器
        bu61580_read_mem(dev, rt_cmd_stack_ptr+2, &recv_msg.rtMessageBlock.dataBlockPtr);   // 读RT/MT数据栈地址寄存器,数据块指针
        bu61580_read_mem(dev, rt_cmd_stack_ptr+3, &recv_msg.rtMessageBlock.cmdWord.all);           // RT 上一个命令字，接收的命令字

//        bu61580_read_reg(dev, ADDR_REG_WR_RT_SUBADDR_CTRL_WORD, &subaddr_ctrl_word.all);
//        bu61580_read_reg(dev, ADDR_REG_WR_TIME_TAG, &recv_msg.rtMessageBlock.timeTagWord);              // 时标寄存器
//        bu61580_read_reg(dev, ADDR_REG_WR_RT_DATA_STACK_ADDR, &recv_msg.rtMessageBlock.dataBlockPtr);   // 读RT/MT数据栈地址寄存器,数据块指针
//        bu61580_read_reg(dev, ADDR_REG_WR_RT_LAST_CMD, &recv_msg.rtMessageBlock.cmdWord.all);           // RT 上一个命令字，接收的命令字

        data_len = (recv_msg.rtMessageBlock.cmdWord.len) ? recv_msg.rtMessageBlock.cmdWord.len : 32;

        if (recv_msg.rtMessageBlock.cmdWord.dir)
        {
            // RT->BC
//            for (size_t i = 0; i < data_len; i++)
//                bu61580_write_mem(dev, (recv_msg.rtMessageBlock.dataBlockPtr + i), tx_cnt + i);
            tx_cnt += 1;
        }
        else
        {

            // BC->RT
            memset(recv_msg.data, 0, sizeof(recv_msg.data));
            for (size_t i = 0; i < data_len; i++)
            {
            	bu61580_read_mem(dev, (recv_msg.rtMessageBlock.dataBlockPtr + i), &recv_msg.data[i]);
            }

            if(cfgReg1.rt0.current_area_b_a)
            	bu61580_read_mem(dev, ADDR_MEM_TX_SA(1, recv_msg.rtMessageBlock.cmdWord.subAddr), &recv_msg.rtMessageBlock.dataBlockPtr);
            else
            	bu61580_read_mem(dev, ADDR_MEM_TX_SA(0, recv_msg.rtMessageBlock.cmdWord.subAddr), &recv_msg.rtMessageBlock.dataBlockPtr);

            for (size_t i = 0; i < data_len; i++)
            {
            	bu61580_write_mem(dev, (recv_msg.rtMessageBlock.dataBlockPtr + i), recv_msg.data[i]);
            }

        }
    }
}

void rt_reg_init(BU61580_t *dev)
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

    // 配置寄存器 #3,D15-增强模式使能
    cfgReg3.enhanced_mode_enable = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_3, &cfgReg3.all);

    // 配置寄存器 #3
    cfgReg3.enhanced_mode_enable = 1U;
    cfgReg3.illegalization_disabled = 1U;
    cfgReg3.enhanced_mode_code_handling = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_3, &cfgReg3.all);

    // 配置寄存器 #6
    cfgReg6.rt_addr_source = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_6, &cfgReg6.all);

    // 配置寄存器 #1，开启 RT
    cfgReg1.rt0.mode = MODE_RT;
    cfgReg1.rt0.dynamic_bus_control_acceptance = 1U;
    cfgReg1.rt0.busy = 1U;
    cfgReg1.rt0.service_request = 1U;
    cfgReg1.rt0.subsystem_flag = 1U;
    cfgReg1.rt0.rtflag = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_1, &cfgReg1.all);

    // 配置寄存器 #2
    cfgReg2.enhanced_int = 1U;
    cfgReg2.rx_sa_double_buffer_enable = 1U;
    cfgReg2.int_sts_auto_clear = 1U;
    cfgReg2.overwrite_invalid_data = 1U;
    cfgReg2.enhanced_rt_memory_management = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_2, &cfgReg2.all);

    // 配置寄存器 #4
    cfgReg4.mode_cmd_override_busy = 1U;
    cfgReg4.latch_rt_address_with_config5 = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_4, &cfgReg4.all);

    // 配置寄存器 #5
    cfgReg5._12mhz_clock_select = 1U;
    cfgReg5.expanded_crossing_enabled = 1U;
    cfgReg5.gap_check_enabled = 1U;
    cfgReg5.rt_address_latch_transparent = 1U;
    cfgReg5.rt_address = 1U;
    cfgReg5.rt_address_parity = 0U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_CONFIG_5, &cfgReg5.all);

    // 中断屏蔽寄存器
    intMaskReg.msg_end = 1U;
    bu61580_write_read_reg(dev, ADDR_REG_WR_INT_MASK, &intMaskReg.all);
}

void rt_mem_init(BU61580_t *dev)
{

    // 初始化存储器
    for (size_t i = 0; i < 8192; i++)
    {
        bu61580_write_mem(dev, i, 0x0000);
    }

    bu61580_write_mem(dev, ADDR_MEM_REGION_A_STACK_PTR, 0x0000);      // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_A_MSG_CNT, 0xFFFF);        // set messages in a frame
    bu61580_write_mem(dev, ADDR_MEM_REGION_A_INIT_STACK_PTR, 0x0000); // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_A_INIT_MSG_CNT, 0xFFFF);   // set messages in a frame

    bu61580_write_mem(dev, ADDR_MEM_REGION_B_STACK_PTR, 0x0000);      // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_B_MSG_CNT, 0xFFFF);        // set messages in a frame
    bu61580_write_mem(dev, ADDR_MEM_REGION_B_INIT_STACK_PTR, 0x0000); // set send stack pointer
    bu61580_write_mem(dev, ADDR_MEM_REGION_B_INIT_MSG_CNT, 0xFFFF);   // set messages in a frame

    // 子地址控制字初始化，开启发送和接收 EOM 中断，启动双重缓冲
    bcCtrlWord_rtSubaddrCtrlWord_t rtSubaddrCtrlWord = {.all = 0U};
    rtSubaddrCtrlWord.rtSubaddrCtrlWord.tx_eom_int = 1U;
    rtSubaddrCtrlWord.rtSubaddrCtrlWord.rx_eom_int = 1U;
    rtSubaddrCtrlWord.rtSubaddrCtrlWord.rx_double_buffer_enable = 1U;
    rtSubaddrCtrlWord.rtSubaddrCtrlWord.rx_mm = RT_MM_DOUBLE_BUFFER;

    // 配置存储器区域，子地址 0 不在此配置
    for (size_t i = 1; i < 32; i++)
    {
        bu61580_write_mem(dev, ADDR_MEM_SACW_SA(0, i), rtSubaddrCtrlWord.all);        // 子地址控制字查询表初始化
        bu61580_write_mem(dev, ADDR_MEM_SACW_SA(1, i), rtSubaddrCtrlWord.all);        // 子地址控制字查询表初始化
        bu61580_write_mem(dev, ADDR_MEM_RX_BCST_SA(0, i), ADDR_RT_DATA_BLOCK(i * 2)); // 接收 (广播) 查询表初始化
        bu61580_write_mem(dev, ADDR_MEM_RX_BCST_SA(1, i), ADDR_RT_DATA_BLOCK(i * 2)); // 接收 (广播) 查询表初始化
        bu61580_write_mem(dev, ADDR_MEM_TX_SA(0, i), ADDR_RT_DATA_BLOCK(i + 64));     // 发送查询表初始化
        bu61580_write_mem(dev, ADDR_MEM_TX_SA(1, i), ADDR_RT_DATA_BLOCK(i + 64));     // 发送查询表初始化
        for (size_t j = 0; j < 32; j++)
        {
            bu61580_write_mem(dev, ADDR_RT_DATA_BLOCK(i + 64) + j, j);
        }
    }

    // 方式码初始化

    // 忙位查询表地址初始化
    for (size_t i = 0; i < 8; i++)
    {
        bu61580_write_mem(dev, ADDR_MEM_BUSY_BIT_QUERY_TABLE(i), 0x0000);
    }
}

// RT 测试
void rt_test(BU61580_t *dev)
{

    rt_reg_init(dev);

    rt_mem_init(dev);

    while (1)
    {
        rt_isr(dev);
    }
}
