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
 * @file BU61580.h
 * @brief
 * @author
 */

/******************************************************************************/
/************************ Copyright *******************************************/
/******************************************************************************/

#ifndef _BU61580_H_
#define _BU61580_H_

/******************************************************************************/
/************************ Include Files ***************************************/
/******************************************************************************/

#include <stdbool.h>
#include <stdint.h>

/******************************************************************************/
/************************ Marco Definitions ***********************************/
/******************************************************************************/

/**
 * 寄存器地址定义
 */
#define ADDR_REG_WR_INT_MASK 0x00             // 0x00，RW，中断屏蔽寄存器
#define ADDR_REG_WR_CONFIG_1 0x01             // 0x01，RW，配置寄存器#1
#define ADDR_REG_WR_CONFIG_2 0x02             // 0x02，RW，配置寄存器#2
#define ADDR_REG_WO_START_RESET 0x03          // 0x03，WO，启动/复位寄存器
#define ADDR_REG_RO_CMD_STACK_PTR 0x03        // 0x03，RO，非增强 BC/RT 命令栈指针寄存器/增强 BC 指令表指针寄存器
#define ADDR_REG_WR_BC_CTRL_WORD 0x04         // 0x04，RW，BC 控制字
#define ADDR_REG_WR_RT_SUBADDR_CTRL_WORD 0x04 // 0x04，RW，RT 子地址控制字寄存器
#define ADDR_REG_WR_TIME_TAG 0x05             // 0x05，RW，时标寄存器
#define ADDR_REG_RO_INT_STATUS 0x06           // 0x06，RO，中断状态寄存器#1
#define ADDR_REG_WR_CONFIG_3 0x07             // 0x07，RW，配置寄存器#3
#define ADDR_REG_WR_CONFIG_4 0x08             // 0x08，RW，配置寄存器#4
#define ADDR_REG_WR_CONFIG_5 0x09             // 0x09，RW，配置寄存器#5
#define ADDR_REG_WR_RT_DATA_STACK_ADDR 0X0A   // 0x0A，RO，RT 数据栈地址寄存器
#define ADDR_REG_WR_MT_DATA_STACK_ADDR 0X0A   // 0x0A，RO，MT 数据栈地址寄存器
#define ADDR_REG_RO_BC_FRAME_TIME_REMAIN 0x0B // 0x0B，RO，BC 帧间剩余时间寄存器
#define ADDR_REG_RO_BC_MSG_TIME_REMAIN 0x0C   // 0x0C，RO，BC 消息间剩余时间寄存器
#define ADDR_REG_WR_BC_INIT_PTR 0x0D          // 0x0D，RW，增强 BC 初始化指令指针
#define ADDR_REG_WR_BC_FRAME_TIME 0x0D        // 0x0D，RW，非增强 BC 帧时间
#define ADDR_REG_WR_RT_LAST_CMD 0x0D          // 0x0D，RW，RT 上一个命令字
#define ADDR_REG_WR_MT_TRIGGER_WORD 0x0D      // 0x0D，RW，MT 触发字寄存器
#define ADDR_REG_RO_RT_STATUS 0x0E            // 0x0E，RO，RT 状态字寄存器
#define ADDR_REG_RO_RT_BIT_WORD 0x0F          // 0x0F，RO，RT 比特字寄存器

#define ADDR_REG_WR_TEST_0 0x10                  // 0x10，RW，测试寄存器#0
#define ADDR_REG_WR_TEST_7 0x17                  // 0x17，RW，测试寄存器#7
#define ADDR_REG_WR_CONFIG_6 0x18                // 0x18，RW，配置寄存器#6
#define ADDR_REG_WR_CONFIG_7 0x19                // 0x19，RW，配置寄存器#7
#define ADDR_REG_RESERVED 0x1A                   // 0x1A，保留
#define ADDR_REG_RO_BC_COND_CODE 0x1B            // 0x1B，RO，BC 条件码寄存器
#define ADDR_REG_WO_BC_GENERAL_TAG 0x1B          // 0x1B，WO，BC 通用标记寄存器
#define ADDR_REG_RO_BUILT_IN_SELF_TEST_STAT 0x1C // 0x1C，RO，内建自测试状态寄存器
#define ADDR_REG_WR_INT_MASK_2 0x1D              // 0x1D，RW，中断屏蔽寄存器#2
#define ADDR_REG_RO_INT_STATUS_2 0x1E            // 0x1E，RO，中断状态寄存器#2
#define ADDR_REG_WR_BC_SEQ_PTR 0x1F              // 0x1F，RW，BC 通用序列指针寄存器
#define ADDR_REG_WR_RT_INTRPT_SEQ_PTR 0x1F       // 0x1F，RW，RT 中断状态序列指针寄存器
#define ADDR_REG_WR_MT_INTRPT_SEQ_PTR 0x1F       // 0x1F，RW，MT 中断状态序列指针寄存器

#define BURX 0U
#define BUTX 1U
#define BUMSGDATANUM 32U

/******************************************************************************/
/****************************** 存储器映射 **********************************/
/******************************************************************************/

#define ADDR_MEM_REGION_A_STACK_PTR 0x0100      // 堆栈指针 A
#define ADDR_MEM_REGION_A_MSG_CNT 0x0101        // 消息计数 A
#define ADDR_MEM_REGION_A_INIT_STACK_PTR 0x0102 // 初始堆栈指针 A
#define ADDR_MEM_REGION_A_INIT_MSG_CNT 0x0103   // 初始消息计数 A

#define ADDR_MEM_REGION_B_STACK_PTR 0x0104      // 堆栈指针 B
#define ADDR_MEM_REGION_B_MSG_CNT 0x0105        // 消息计数 B
#define ADDR_MEM_REGION_B_INIT_STACK_PTR 0x0106 // 初始堆栈指针 B
#define ADDR_MEM_REGION_B_INIT_MSG_CNT 0x0107   // 初始消息计数 B

/******************************************************************************/
/****************************** BC 存储器映射 **********************************/
/******************************************************************************/

#define SIZE_MEM_STACK_A 0x0004                               // 栈区 A 大小
#define ADDR_MEM_STACK_A(n) (0x0000 + SIZE_MEM_STACK_A * (n)) // 栈区 A 起始地址

#define SIZE_MEM_STACK_B 0x0004                               // 栈区 B 大小
#define ADDR_MEM_STACK_B(n) (0x0F00 + SIZE_MEM_STACK_B * (n)) // 栈区 B 起始地址

#define SIZE_BC_DATA_BLOCK 0x0026                                // 数据块大小
#define ADDR_BC_DATA_BLOCK(n) (0x108 + SIZE_BC_DATA_BLOCK * (n)) // 数据块地址

/******************************************************************************/
/****************************** RT 存储器映射 **********************************/
/******************************************************************************/

#define ADDR_MEM_FUNC_CODE_INTR_TBL 0x0108U // 方式码选择性中断表地址
#define SIZE_MEM_FUNC_CODE_INTR_TBL 0x8U    // 方式码选择性中断表大小

#define ADDR_MEM_FUNC_CODE_DATA 0x0110U // 方式码数据地址
#define SIZE_MEM_FUNC_CODE_DATA 0x30U   // 方式码数据地址大小

#define ADDR_MEM_QUERY_TABLE_A 0x0140U // 查询表 A 地址
#define ADDR_MEM_QUERY_TABLE_B 0x01C0U // 查询表 B 地址
#define SIZE_MEM_QUERY_TABLE 0x80U     // 查询表大小

#define ADDR_MEM_RX_BCST_SA(ch, n) (0x0140U + SIZE_MEM_QUERY_TABLE * (ch) + (n)) // 接收 (广播) 查询表:0140-015F
#define ADDR_MEM_TX_SA(ch, n) (0x0160U + SIZE_MEM_QUERY_TABLE * (ch) + (n))      // 发送查询表 : 0160-017F
#define ADDR_MEM_BCST_SA(ch, n) (0x0180U + SIZE_MEM_QUERY_TABLE * (uint16_t)(ch) + (uint16_t)(n))    // 广播查询表 (可选择):0180-019F
#define ADDR_MEM_SACW_SA(ch, n) (0x01A0U + SIZE_MEM_QUERY_TABLE * (uint16_t)(ch) + (uint16_t)(n))    // 子地址控制字查询表 (可选择):01A0-01BF

#define ADDR_MEM_BUSY_BIT_QUERY_TABLE(n) (0x0240 + (n)) // 忙位查询表地址
#define SIZE_MEM_BUSY_BIT_QUERY_TABLE 0x8               // 忙位查询表大小

#define ADDR_MEM_ILLEGAL_CMD_TBL 0x0300 // 命令非法化表地址
#define SIZE_MEM_ILLEGAL_CMD_TBL 0x20  // 命令非法化表大小

#define SIZE_RT_DATA_BLOCK 0x20                                                          // 数据块大小
#define ADDR_RT_DATA_BLOCK(n) ((((n) < 5) ? 0x0260 : 0x0400) + SIZE_RT_DATA_BLOCK * (n)) // 数据块地址

/******************************************************************************/
/****************************** 可选择消息 MT 存储器映射 **********************************/
/******************************************************************************/

#define ADDR_MEM_CMD_STATK_PTR_A 0x0102  // 监控命令堆栈指针 A（固定位置）
#define ADDR_MEM_DATA_STATK_PTR_A 0x0103 // 监控数据堆栈指针 A（固定位置）
#define ADDR_MEM_CMD_STATK_PTR_B 0x0106  // 监控命令堆栈指针 B（固定位置）
#define ADDR_MEM_DATA_STATK_PTR_B 0x0107 // 监控数据堆栈指针 B（固定位置）

#define SIZE_SEL_MT_LOOKUP_TABLE 0x80
#define ADDR_MEM_SEL_MT_LOOKUP_TABLE 0x00280 // 可选择监控查询表（固定区域）

#define SIZE_CMD_STATK 0x0400
#define ADDR_MEM_CMD_STATK_A 0x0400 // 监控命令堆栈 A

#define SIZE_DATA_STATK 0x0400
#define ADDR_MEM_DATA_STATK_A 0x0800 // 监控数据堆栈 A

#ifdef __cplusplus
extern "C"
{
#endif

    /******************************************************************************/
    /************************ Register Types Definitions **************************/
    /******************************************************************************/

    // 0x00，RW，中断屏蔽寄存器
    // 0x06，RO，中断状态寄存器
    typedef union intMaskReg_t
    {
        struct
        {
            uint16_t msg_end : 1;                       // bit0，消息结束
            uint16_t bc_sts_set_rt_mode_trigger_mt : 1; // bit1，BC 状态设置/RT 方式码/MT 模式触发
            uint16_t fmt_err : 1;                       // bit2，格式出错
            uint16_t info_frm_end : 1;                  // bit3，BC 信息帧结束
            uint16_t msg_ctrl_word_end : 1;             // bit4，BC 控制字/RT 子地址控制字消息结束
            uint16_t rt_circ_buffer_flip : 1;           // bit5，RT 循环缓冲翻转
            uint16_t ttag_flip : 1;                     // bit6，时标翻转
            uint16_t rt_addr_odd_parity_err : 1;        // bit7，RT 地址奇校验出错
            uint16_t bc_retries : 1;                    // bit8，BC 重试
            uint16_t handshake_fail : 1;                // bit9，握手失败 (与外部 RAM 或主机)
            uint16_t mt_data_stack_flip : 1;            // bit10，MT 数据栈翻转
            uint16_t mt_cmd_stack_flip : 1;             // bit11，MT 命令栈翻转
            uint16_t bc_rt_stack_flip : 1;              // bit12，BC/RT 命令栈翻转
            uint16_t trs_timeout : 1;                   // bit13，BC/RT 传输超时
            uint16_t mem_parity_err : 1;                // bit14，存储器奇校验出错
            uint16_t : 1;                               // bit15，保留
        };
        uint16_t all;
    } intMaskReg_t;

    // 0x01，RW，配置寄存器#1
    typedef union cfgReg1_t
    {
        struct cfgReg1_bc_t
        {
            uint16_t bc_msg_in_progress : 1;          // bit0，BC 消息执行中 (只读)
            uint16_t bc_frm_in_progress : 1;          // bit1，BC 帧执行中 (只读)
            uint16_t bc_enabled : 1;                  // bit2，BC 使能 (只读)
            uint16_t doubled_single_retry : 1;        // bit3，双/单消息重试
            uint16_t retry_enabled : 1;               // bit4，消息重试使能
            uint16_t inter_msg_gap_timer_enabled : 1; // bit5，消息间记时使能
            uint16_t internal_trigger_enabled : 1;    // bit6，内部触发使能
            uint16_t external_trigger_enabled : 1;    // bit7，外部触发使能
            uint16_t frm_auto_repeat : 1;             // bit8，帧自动重发
            uint16_t sts_set_stop_on_frm : 1;         // bit9，状态设置帧停止
            uint16_t sts_set_stop_on_msg : 1;         // bit10，状态设置消息停止
            uint16_t frm_stop_on_error : 1;           // bit11，帧出错停止
            uint16_t msg_stop_on_error : 1;           // bit12，消息出错停止
            uint16_t current_area_b_a : 1;            // bit13，当前区域 B/A_n
            uint16_t mode : 2;                        // bit14-15，00:BC，10:RT，01:MT see BU_MODE
        } bc;

        struct cfgReg1_rt0_t
        {
            uint16_t rt_msg_in_progress : 1;             // bit0，RT 消息执行中 (增强模式，只读)
            uint16_t : 1;                                // bit1，保留
            uint16_t : 1;                                // bit2，保留
            uint16_t : 1;                                // bit3，保留
            uint16_t : 1;                                // bit4，保留
            uint16_t : 1;                                // bit5，保留
            uint16_t : 1;                                // bit6，保留
            uint16_t rtflag : 1;                         // bit7，RT 标记 (增强模式)(低有效)
            uint16_t subsystem_flag : 1;                 // bit8，子系统标记 (低有效)
            uint16_t service_request : 1;                // bit9，服务请求 (低有效)
            uint16_t busy : 1;                           // bit10，忙位 (低有效)
            uint16_t dynamic_bus_control_acceptance : 1; // bit11，动态总线控制受理 (低有效)
            uint16_t msg_monitor_enabled : 1;            // bit12，消息监测使能
            uint16_t current_area_b_a : 1;               // bit13，当前区域 B/A_n
            uint16_t mode : 2;                           // bit14-15，00:BC，10:RT，01:MT see BU_MODE
        } rt0;

        struct cfgReg1_rt1_t
        {
            uint16_t rt_msg_in_progress : 1;  // bit0，RT 消息执行中 (只读)
            uint16_t s00 : 1;                     // bit1，S00
            uint16_t s01 : 1;                     // bit2，S01
            uint16_t s02 : 1;                     // bit3，S02
            uint16_t s03 : 1;                     // bit4，S03
            uint16_t s04 : 1;                     // bit5，S04
            uint16_t s05 : 1;                     // bit6，S05
            uint16_t s06 : 1;                     // bit7，S06
            uint16_t s07 : 1;                     // bit8，S07
            uint16_t s08 : 1;                     // bit9，S08
            uint16_t s09 : 1;                     // bit10，S09
            uint16_t s10 : 1;                     // bit11，S10
            uint16_t msg_monitor_enabled : 1; // bit12，消息监测使能
            uint16_t current_area_a_b : 1;        // bit13，当前区域 B/A_
            uint16_t mt_bcrt : 1;                 // bit14，逻辑 0
            uint16_t rt_bcmt : 1;                 // bit15，逻辑 1
        } rt1;

        struct cfgReg1_mt_t
        {
            uint16_t monitor_active : 1;           // bit0，监测激活 (只读)
            uint16_t monitor_triggered : 1;        // bit1，监测触发 (只读)
            uint16_t monitor_enabled : 1;          // bit2，监测使能 (只读)
            uint16_t : 1;                          // bit3，保留
            uint16_t : 1;                          // bit4，保留
            uint16_t : 1;                          // bit5，保留
            uint16_t : 1;                          // bit6，保留
            uint16_t external_trigger_enabled : 1; // bit7，外部触发使能
            uint16_t : 1;                          // bit8，保留
            uint16_t stop_on_trigger : 1;          // bit9，停止触发
            uint16_t start_on_trigger : 1;         // bit10，触发启动
            uint16_t trigger_enabled_word : 1;     // bit11，触发使能字
            uint16_t msg_monitor_enabled : 1;      // bit12，消息监测使能
            uint16_t current_area_b_a : 1;         // bit13，当前区域 B/A_
            uint16_t mode : 2;                     // bit14-15，00:BC，10:RT，01:MT see BU_MODE
        } mt;
        uint16_t all;
    } cfgReg1_t;

    // 0x02，RW，配置寄存器#2
    typedef union cfgReg2_t
    {
        struct
        {
            uint16_t separate_bcst_data : 1;            // bit0，隔离广播数据
            uint16_t enhanced_rt_memory_management : 1; // bit1，增强 RT 存储器管理
            uint16_t clear_service_request : 1;         // bit2，清除服务请求
            uint16_t level_pulse_int_request : 1;       // bit3，电平/脉冲中断请求
            uint16_t int_sts_auto_clear : 1;            // bit4，中断状态自动清除
            uint16_t load_time_tag_on_synchronize : 1;  // bit5，同步时装载时标
            uint16_t clear_time_tag_on_synchronize : 1; // bit6，同步时清除时标
            uint16_t ttr0 : 1;                          // bit7，时标分辨率 0
            uint16_t ttr1 : 1;                          // bit8，时标分辨率 1
            uint16_t ttr2 : 1;                          // bit9，时标分辨率 2
            uint16_t _256_word_boundary_disable : 1;    // bit10，256 字的边界无效
            uint16_t overwrite_invalid_data : 1;        // bit11，覆盖非法数据
            uint16_t rx_sa_double_buffer_enable : 1;    // bit12，RT 双重缓冲使能
            uint16_t busy_lookup_table_enable : 1;      // bit13，存储器回写表忙位使能
            uint16_t : 1;                               // bit14，保留
            uint16_t enhanced_int : 1;                  // bit15，增强模式中断
        };
        uint16_t all;
    } cfgReg2_t;

    // 0x03，WO，启动/复位寄存器
    // 0x03，RO，命令堆栈指针寄存器
    typedef union startResetReg_t
    {
        struct
        {
            uint16_t reset : 1;               // bit0，复位
            uint16_t bc_mt_start : 1;         // bit1，BC/MT 启动
            uint16_t int_reset : 1;           // bit2，中断复位
            uint16_t time_tag_reset : 1;      // bit3，时标复位
            uint16_t time_tag_test_clock : 1; // bit4，时标测试时钟
            uint16_t bc_stop_on_frm : 1;      // bit5，BC 帧停止
            uint16_t bc_mt_stop_on_msg : 1;   // bit6，BC/MT 消息停止
            uint16_t : 9;                     // bit7-15，保留
        };
        uint16_t cmdStackPtr;
        uint16_t all;
    } startResetReg_t;

    // 0x04，RW，BC 控制字
    // 0x04，RW，RT 子地址控制字
    typedef union bcCtrlWord_rtSubaddrCtrlWord_t
    {
        struct bcCtrlWord_t
        {
            uint16_t rt2rt_fmt : 1;               // bit0，RT-RT 格式
            uint16_t bcst_fmt : 1;                // bit1，广播模式格式
            uint16_t mode_code_fmt : 1;           // bit2，模式码格式
            uint16_t ab_select : 1;               // bit3，1553A/B 选择
            uint16_t eom_intenable : 1;           // bit4，消息发完中断使能
            uint16_t mask_bcst_bit : 1;           // bit5，屏蔽广播模式位
            uint16_t offline_self_test : 1;       // bit6，掉线自测试
            uint16_t bus_channel_ab : 1;          // bit7，总线通道 1:A0:B
            uint16_t retry_enabled : 1;           // bit8，消息重试使能
            uint16_t reserved_bits_mask : 1;      // bit9，保留位屏蔽
            uint16_t terminal_flag_bit_mask : 1;  // bit10，终端标记位屏蔽
            uint16_t subsys_flag_bit_mask : 1;    // bit11，子地址标记位屏蔽
            uint16_t subsys_Busy_Bit_Mask : 1;    // bit12，子地址忙位屏蔽
            uint16_t service_request_bitmask : 1; // bit13，服务请求位屏蔽
            uint16_t msg_error_bit_mask : 1;      // bit14，消息出错位屏蔽
            uint16_t : 1;                         // bit15，保留
        } bcCtrlWord;

        struct rtSubaddrCtrlWord_t
        {
            uint16_t bcst_mm : 3;                 // bit0-2，广播模式时：存储器管理
            uint16_t bcst_circ_buf_int : 1;       // bit3，广播模式时：循环缓冲中断
            uint16_t bcst_eom_int : 1;            // bit4，广播模式时：消息收完中断
            uint16_t rx_mm : 3;                   // bit5-7，接收时：存储器管理 see RT_MM_MODE
            uint16_t rx_circ_buf_int : 1;         // bit8，接收时：循环缓冲中断
            uint16_t rx_eom_int : 1;              // bit9，接收时：消息收完中断
            uint16_t tx_mm : 3;                   // bit10-12，发送时：存储器管理
            uint16_t tx_circ_buf_int : 1;         // bit13，发送时：循环缓冲中断
            uint16_t tx_eom_int : 1;              // bit14，发送时：消息发完中断
            uint16_t rx_double_buffer_enable : 1; // bit15，接收时：双重缓冲模式使能
        } rtSubaddrCtrlWord;

        uint16_t all;
    } bcCtrlWord_rtSubaddrCtrlWord_t;

    // 0x07，RW，配置寄存器#3
    typedef union cfgReg3_t
    {
        struct
        {
            uint16_t enhanced_mode_code_handling : 1; // bit0，增强方式码处理
            uint16_t _1553a_mode_codes_enable : 1;    // bit1，1553A 方式码使能
            uint16_t rtfail_flag_wrap_enable : 1;     // bit2，RT 反馈测试失败或终端标志使能
            uint16_t busy_rx_transfer_disable : 1;    // bit3，接收器忙无效
            uint16_t illegal_rx_transfer_disable : 1; // bit4，接收器无效非法化
            uint16_t alternate_sts_word_enable : 1;   // bit5，可选择状态字使能
            uint16_t override_mode_tr_error : 1;      // bit6，传送/接收无效使能
            uint16_t illegalization_disabled : 1;     // bit7，非法化无效
            uint16_t mt_data_stack_size : 3;          // bit8-10，MT 数据栈空间 see MT_DATA_STACK_1K
            uint16_t mt_cmd_stack_size : 2;           // bit11-12，MT 命令栈空间 see MT_CMD_STACK_1K
            uint16_t bc_rt_cmd_stack_size : 2;        // bit13-14，BC/RT 命令栈空间
            uint16_t enhanced_mode_enable : 1;        // bit15，增强模式使能
        };
        uint16_t all;
    } cfgReg3_t;

    // 0x08，RW，配置寄存器#4
    typedef union cfgReg4_t
    {
        struct
        {
            uint16_t test_mode : 3;                       // bit0-2，测试模式
            uint16_t latch_rt_address_with_config5 : 1;   // bit3，用配置寄存器#5 锁存 RT 地址
            uint16_t mt_tag_gap_option : 1;               // bit4，MT 时间间隔选项
            uint16_t valid_busy_no_data : 1;              // bit5，忙位/没有数据时认为正确
            uint16_t valid_me_no_data : 1;                // bit6，消息错误/没有数据时认为正确
            uint16_t _2nd_retry_alt_same_bus : 1;         // bit7，第二次重发在另一条总线还是相同的总线
            uint16_t _1st_retry_alt_same_bus : 1;         // bit8，第一次重发在另一条总线还是相同的总线
            uint16_t retry_if_sts_set : 1;                // bit9，状态置位时重发
            uint16_t retry_if_a_and_me : 1;               // bit10，如果-A 或消息错误时消息重发
            uint16_t bcst_mask_enable_xor : 1;            // bit11，广播屏蔽 ENA/XOR*
            uint16_t expanded_bc_control_word_enable : 1; // bit12，扩展 BC 控制字使能
            uint16_t mode_cmd_override_busy : 1;          // bit13，方式命令不考虑忙位
            uint16_t inhibit_bit_word_if_busy : 1;        // bit14，忙时字禁止 BIT 字
            uint16_t external_bit_word_enable : 1;        // bit15，外部 BIT 字使能
        };
        uint16_t all;
    } cfgReg4_t;

    // 0x09，RW，配置寄存器#5
    typedef union cfgReg5_t
    {
        struct
        {
            uint16_t rt_address_parity : 1;            // bit0，RT 地址奇校验
            uint16_t rt_address : 5;                   // bit1-5，RT 地址
            uint16_t rt_address_latch_transparent : 1; // bit6，RT 地址锁存/透明
            uint16_t bcst_disabled : 1;                // bit7，广播使能
            uint16_t gap_check_enabled : 1;            // bit8，消息间隔检查使能
            uint16_t response_timeout_select_0 : 1;    // bit9，响应超时选择 0
            uint16_t response_timeout_select_1 : 1;    // bit10，响应超时选择 1
            uint16_t expanded_crossing_enabled : 1;    // bit11，外部过零使能
            uint16_t external_tx_inhibit_b : 1;        // bit12，外部 B 通道禁止
            uint16_t external_tx_inhibit_a : 1;        // bit13，外部 A 通道禁止
            uint16_t logic_0 : 1;                      // bit14，单沿采样选择
            uint16_t _12mhz_clock_select : 1;          // bit15，12/16MHz 时钟选择
        };
        uint16_t all;
    } cfgReg5_t;

    // 0x18，RW，配置寄存器#6
    typedef union cfgReg6_t
    {
        struct
        {
            uint16_t clk_sel : 2;                                   // bit0-1，时钟选择
            uint16_t _64Word_Reg_Space : 1;                         // bit2，64 字寄存器空间
            uint16_t : 1;                                           // bit3，保留
            uint16_t enhanced_MT : 1;                               // bit4，增强消息监控
            uint16_t rt_addr_source : 1;                            // bit5，RT 地址源
            uint16_t int_status_seq_en : 1;                         // bit6，中断状态序列使能
            uint16_t disable_legalization_msg_int_status_seq : 1;   // bit7，禁止合法化消息中断状态序列
            uint16_t disable_illegalization_msg_int_status_seq : 1; // bit8，禁止非法化消息中断状态序列
            uint16_t global_circ_buffer_size : 3;                   // bit9-11，全局循环缓冲大小
            uint16_t global_circ_buffer_en : 1;                     // bit12，全局循环缓冲使能
            uint16_t msg_end_enhanced_cmd_stack_ptr : 1;            // bit13，消息结束增强命令栈指针 (RT/MT)
            uint16_t enhanced_cpu_access : 1;                       // bit14，增强 cpu 通道
            uint16_t enhanced_bc : 1;                               // bit15，增强总线控制器
        };
        uint16_t all;
    } cfgReg6_t;

    // 0x1C，RO，内建自测试状态寄存器
    typedef union buildInTestResReg_t
    {
        struct
        {
            uint16_t : 1;                          // 逻辑"0"
            uint16_t : 1;                          // 逻辑"0"
            uint16_t : 1;                          // 逻辑"0"
            uint16_t : 1;                          // 逻辑"0"
            uint16_t : 1;                          // 逻辑"0"
            uint16_t ram_test_passed : 1;          // RAM 自测试通过
            uint16_t ram_test_inprogress : 1;      // RAM 自测试进行
            uint16_t ram_test_finished : 1;        // RAM 自测试完成
            uint16_t : 1;                          // 逻辑"0"
            uint16_t : 1;                          // 逻辑"0"
            uint16_t : 1;                          // 逻辑"0"
            uint16_t protocol_test_done : 1;       // 协议自测试完成/进行
            uint16_t protocol_test_abort : 1;      // 协议自测试终止
            uint16_t protocol_test_passed : 1;     // 协议自测试通过
            uint16_t protocol_test_inprogress : 1; // 协议自测试正在进行
            uint16_t protocol_test_finished : 1;   // 协议自测试完成
        };
        uint16_t all;
    } buildInTestResReg_t;

    /******************************************************************************/
    /************************ Protocol Types Declarations *************************/
    /******************************************************************************/

    typedef union cmdWord_t
    {
        struct
        {
            uint16_t len : 5;     // 数据字计数
            uint16_t subAddr : 5; // 子地址
            uint16_t dir : 1;     // 方向 0:BC->RT，1:RT->BC
            uint16_t rtAddr : 5;  // RT 终端地址
        };
        uint16_t all;
    } cmdWord_t;

    typedef union stsWord_t
    {
        struct
        {
            uint16_t H : 1; // 终端标记
            uint16_t G : 1; // 动态总线控制
            uint16_t F : 1; // 子系统标记
            uint16_t E : 1; // 忙
            uint16_t D : 1; // 广播命令接收
            uint16_t : 3;
            uint16_t C : 1; // 服务请求
            uint16_t B : 1; // 测试手段（通常保持为“0”，和命令字中的第 10 位一起用来区分状态字和命令字）
            uint16_t A : 1; // 消息出错
            uint16_t rtAddr : 5;
        };
        uint16_t all;
    } stsWord_t;

    enum BU_MODE
    {
        MODE_BC = 0B00,
        MODE_RT = 0B10,
        MODE_MT = 0B01
    };

    // RT 存储器管理模式
    enum RT_MM_MODE
    {
        RT_MM_SINGLE_BUFFER = 0B000,
        RT_MM_DOUBLE_BUFFER = 0B000,
        RT_MM_CIRC_BUFFER_128 = 0B001,
        RT_MM_CIRC_BUFFER_256 = 0B010,
        RT_MM_CIRC_BUFFER_512 = 0B011,
        RT_MM_CIRC_BUFFER_1024 = 0B100,
        RT_MM_CIRC_BUFFER_2048 = 0B101,
        RT_MM_CIRC_BUFFER_4096 = 0B110,
        RT_MM_CIRC_BUFFER_8192 = 0B111,
        RT_MM_GLOBAL_BUFFER = 0B111
    };

    // MT 监控命令堆栈大小
    enum MT_CMD_STACK_SIZE
    {
        MT_CMD_STACK_256 = 0B00, // 256 WORD
        MT_CMD_STACK_1K = 0B01,  // 1K WORD
        MT_CMD_STACK_4K = 0B10,  // 4K WORD
        MT_CMD_STACK_16K = 0B11, // 16K WORD
    };

    // MT 监控数据堆栈大小
    enum MT_DATA_STACK_SIZE
    {
        MT_DATA_STACK_256 = 0B00,  // 512 WORD
        MT_DATA_STACK_1K = 0B001,  // 1K WORD
        MT_DATA_STACK_2K = 0B010,  // 2K WORD
        MT_DATA_STACK_4K = 0B011,  // 4K WORD
        MT_DATA_STACK_8K = 0B100,  // 8K WORD
        MT_DATA_STACK_16K = 0B101, // 16K WORD
        MT_DATA_STACK_32K = 0B110, // 32K WORD
        MT_DATA_STACK_64K = 0B111  // 64K WORD
    };

    /******************************************************************************/
    /************************ chip message block defines *************************/
    /******************************************************************************/

    // BC 块状态字
    typedef union bcBlockStatusWord_t
    {
        struct
        {
            uint16_t invalid_word : 1;             // bit0，非法字
            uint16_t incorrect_sync_type : 1;      // bit1，不正确的同步类型
            uint16_t word_count_error : 1;         // bit2，字计数错误
            uint16_t wrong_sts_address_no_gap : 1; // bit3，错误状态地址/无间隔
            uint16_t good_data_block_transfer : 1; // bit4，正确数据块传送
            uint16_t retry_count_0 : 1;            // bit5，重发计数 0
            uint16_t retry_count_1 : 1;            // bit6，重发计数 1
            uint16_t masked_sts_set : 1;           // bit7，屏蔽状态设置
            uint16_t loop_test_fail : 1;           // bit8，循环测试失败
            uint16_t no_response_timeout : 1;      // bit9，非响应超时
            uint16_t fmt_error : 1;                // bit10，格式出错
            uint16_t sts_set : 1;                  // bit11，状态设置
            uint16_t error_flag : 1;               // bit12，错误标志
            uint16_t channel_b_a : 1;              // bit13，通道 B/A
            uint16_t som : 1;                      // bit14，消息开始
            uint16_t eom : 1;                      // bit15，消息结束
        };
        uint16_t all;
    } bcBlockStatusWord_t;

    // RT 块状态字
    typedef union rtBlockStatusWord_t
    {
        struct
        {
            uint16_t command_word_error : 1;           // bit0，命令字错误
            uint16_t rt_rt_2nd_command_error : 1;      // bit1，RT-RT 第二个命令字错误
            uint16_t rt_rt_gap_sync_address_error : 1; // bit2，RT 到 RT 间隔/同步/地址错误
            uint16_t invalid_word : 1;                 // bit3，无效的字
            uint16_t incorrect_sync : 1;               // bit4，不正确的同步
            uint16_t word_count_error : 1;             // bit5，字计数错误
            uint16_t illegal_command_word : 1;         // bit6，非法命令字
            uint16_t data_stack_rollover : 1;          // bit7，数据栈翻转
            uint16_t loop_test_fail : 1;               // bit8，循环测试失败
            uint16_t no_response_timeout : 1;          // bit9，无响应超时
            uint16_t fmt_error : 1;                    // bit10，格式错误
            uint16_t rt_rt_fmt : 1;                    // bit11，RT-RT 格式
            uint16_t error_flag : 1;                   // bit12，错误标志
            uint16_t channel_b_a : 1;                  // bit13，通道 B/A
            uint16_t som : 1;                          // bit14，消息开始
            uint16_t eom : 1;                          // bit15，消息结束
        };
        uint16_t all;
    } rtBlockStatusWord_t;

    // MT 监控辨认字
    typedef union mtIDWord_t
    {
        struct
        {
            uint16_t mode_code : 1;      // bit0，方式代码
            uint16_t continuous_gap : 1; // bit1，连续数据/间隔
            uint16_t channel_b_a : 1;    // bit2，通道 B/A
            uint16_t cmd_data_sync : 1;  // bit3，命令/数据同步
            uint16_t error : 1;          // bit4，错误
            uint16_t bcst : 1;           // bit5，广播
            uint16_t this_rt : 1;        // bit6，THIS_RT*
            uint16_t word_flag : 1;      // bit7，字标志
            uint16_t tag_gap : 8;        // bit8-15，间隔时间
        };
        uint16_t all;
    } mtIDWord_t;

    // 增强 BC 消息描述块
    typedef struct enhancedBCMsgDesc_t
    {
        bcCtrlWord_rtSubaddrCtrlWord_t ctrlWord; // 0 BC 控制字
        cmdWord_t cmdWord;                       // 1 命令字
        uint16_t dataBlockPtr;                   // 2 数据块指针
        uint16_t msgGapTimeWord;                 // 3 消息间隔
        uint16_t timeTagWord;                    // 4 时间标记字
        bcBlockStatusWord_t blockStatusWord;     // 5 块状态字
        uint16_t backWord;                       // 6 回送字
        uint16_t RTStatusWord;                   // 7 RT 状态字
    } enhancedBCMsgDesc_t;

    // 非增强 BC 消息描述块
    typedef struct normalBCMsgDesc_t
    {
        bcBlockStatusWord_t blockStatusWord; // 0 块状态字
        uint16_t timeTagWord;                // 1 时间标记字
        uint16_t msgGapTimeWord;             // 2 消息间隔时间字
        uint16_t msgBlockAddr;               // 3 数据块指针
    } normalBCMsgDesc_t;

    // RT 消息描述块
    typedef struct RTMsgDesc_t
    {
        rtBlockStatusWord_t blockStatusWord; // 0 块状态字
        uint16_t timeTagWord;                // 1 时间标记字
        uint16_t dataBlockPtr;               // 2 数据块指针
        cmdWord_t cmdWord;                   // 3 命令字
    } RTMsgDesc_t;

    // BC-RT 数据块
    typedef struct BC2RTDataBlock_t
    {
        bcCtrlWord_rtSubaddrCtrlWord_t ctrlWord; // 0 控制字
        cmdWord_t cmdWord;                       // 1 命令字
        uint16_t *dataWord;                      // 2 数据字
        uint16_t backWord;                       // 3 回送数据字
        stsWord_t stsWord;                       // 4 接收状态字
    } BC2RTDataBlock_t;

    // RT-BC 数据块
    typedef struct RT2BCDataBlock_t
    {
        bcCtrlWord_rtSubaddrCtrlWord_t ctrlWord; // 0 控制字
        cmdWord_t cmdWord;                       // 1 命令字
        uint16_t backWord;                       // 2 回送数据字
        stsWord_t stsWord;                       // 3 接收状态字
        uint16_t *dataWord;                      // 4 数据字
    } RT2BCDataBlock_t;

    /******************************************************************************/
    /************************ Functions Declarations ******************************/
    /******************************************************************************/

    /******************************************************************************/
    /************************ Variable Declarations *******************************/
    /******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* defined(_BU61580_H_) */
