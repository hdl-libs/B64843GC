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
// +FHEADER-------------------------------------------------------------------------------
// Author        : john_tito
// Module Name   : ace_interface_tb
// ---------------------------------------------------------------------------------------
// Revision      : 1.0
// Description   : File Created
// ---------------------------------------------------------------------------------------
// Synthesizable : Yes
// Clock Domains : clk
// Reset Strategy: sync reset
// -FHEADER-------------------------------------------------------------------------------

// verilog_format: off
`resetall
`timescale 1ns / 1ps
`default_nettype none
// verilog_format: on

module ace_interface_tb;

    // Parameters
    localparam real TIMEPERIOD = 5;

    // Ports
    reg           clk = 0;
    reg           rstn = 0;

    reg  [31 : 0] s_apb_paddr = 0;
    reg  [ 0 : 0] s_apb_psel = 0;
    reg           s_apb_penable = 0;
    reg           s_apb_pwrite = 0;
    reg  [31 : 0] s_apb_pwdata = 0;
    wire [ 0 : 0] s_apb_pready;
    wire [31 : 0] s_apb_prdata;
    wire [ 0 : 0] s_apb_pslverr;

    wire          b64843_select_n;
    wire          b64843_strbd_n;
    wire          b64843_ioe_n;
    wire          b64843_m_rn;
    wire          b64843_r_wn;
    wire          b64843_ready_n;
    wire [  15:0] b64843_data;
    wire [  15:0] b64843_addr;

    wire [  15:0] b64843_rdata;
    wire [  15:0] b64843_wdata;
    wire          b64843_oe_n;

    assign b64843_rdata = b64843_oe_n ? b64843_data : 16'd0;
    assign b64843_data  = b64843_oe_n ? 16'dz : b64843_wdata;

    ace_interface dut (
        .clk            (clk),
        .rstn           (rstn),
        .s_apb_paddr    (s_apb_paddr),
        .s_apb_psel     (s_apb_psel),
        .s_apb_penable  (s_apb_penable),
        .s_apb_pwrite   (s_apb_pwrite),
        .s_apb_pwdata   (s_apb_pwdata),
        .s_apb_pready   (s_apb_pready),
        .s_apb_prdata   (s_apb_prdata),
        .s_apb_pslverr  (s_apb_pslverr),
        .b64843_select_n(b64843_select_n),
        .b64843_strbd_n (b64843_strbd_n),
        .b64843_ioe_n   (b64843_ioe_n),
        .b64843_m_rn    (b64843_m_rn),
        .b64843_r_wn    (b64843_r_wn),
        .b64843_ready_n (b64843_ready_n),
        .b64843_rdata   (b64843_rdata),
        .b64843_wdata   (b64843_wdata),
        .b64843_oe_n    (b64843_oe_n),
        .b64843_addr    (b64843_addr)
    );

    B64843GC_sim_module B64843GC_sim_module_inst (
        .clk    (clk),
        .rstn   (rstn),
        .str_n  (b64843_strbd_n),
        .sel_n  (b64843_select_n),
        .rd     (b64843_r_wn),
        .ioen_n (b64843_ioe_n),
        .ready_n(b64843_ready_n),
        .data   (b64843_data),
        .mem    (b64843_m_rn),
        .addr   (b64843_addr)
    );

    reg [16:0] i = 0;
    initial begin
        begin
            s_apb_penable = 1'b0;
            wait (rstn);
            #200;

            for (i = 0; i < 17'h00FF; i = i + 1) begin
                s_apb_paddr   = i;
                s_apb_psel    = 1;
                s_apb_penable = 1;
                s_apb_pwrite  = 1;
                s_apb_pwdata  = i;
                #(TIMEPERIOD);
                #0.1;
                s_apb_paddr   = i;
                s_apb_psel    = 0;
                s_apb_penable = 0;
                s_apb_pwrite  = 1;
                s_apb_pwdata  = i;
                wait (s_apb_pready);
                #(TIMEPERIOD);
                #0.1;
            end

            // for (i = 0; i < 17'h1ffff; i = i + 1) begin
            //     s_apb_paddr   = i;
            //     s_apb_psel    = 0;
            //     s_apb_penable = 1;
            //     s_apb_pwrite  = 0;
            //     s_apb_pwdata  = i;
            //     #(TIMEPERIOD);
            //     #0.1;
            //     s_apb_paddr   = 0;
            //     s_apb_psel    = 0;
            //     s_apb_penable = 0;
            //     s_apb_pwrite  = 0;
            //     s_apb_pwdata  = 0;
            //     wait (s_apb_pready);
            //     #(TIMEPERIOD);
            //     #0.1;
            // end

            #3000;

            $finish;
        end
    end

    always #(TIMEPERIOD / 2) clk = !clk;

    // reset block
    initial begin
        rstn = 1'b0;
        #(TIMEPERIOD * 2);
        rstn = 1'b1;
    end

    // record block
    initial begin
        $dumpfile("sim/test_tb.vcd");
        $dumpvars(0, ace_interface_tb);
    end

endmodule

// verilog_format: off
`resetall
// verilog_format: on
