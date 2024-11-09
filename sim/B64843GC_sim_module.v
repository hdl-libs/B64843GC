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
// Module Name   : B64843GC_sim_module
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

module B64843GC_sim_module #(
    parameter RD_IOEN_DELAY  = 1,
    parameter RD_IOEN_HOLD   = 0,
    parameter RD_DATA_DELAY  = 3,
    parameter RD_DATA_HOLD   = 0,
    parameter RD_READY_DELAY = 5,

    parameter WR_IOEN_DELAY  = 1,
    parameter WR_IOEN_HOLD   = 0,
    parameter WR_READY_DELAY = 5
) (
    input  wire        clk,
    input  wire        rstn,
    input  wire        str_n,
    input  wire        sel_n,
    input  wire        rd,
    output reg         ioen_n,
    output reg         ready_n,
    inout  wire [15:0] data,
    input  wire        mem,
    input  wire [15:0] addr
);

    localparam FSM_IDLE = 8'h00;
    localparam FSM_LEAD = 8'h01;
    localparam FSM_RD_WAIT = 8'h02;
    localparam FSM_RD_ACTIVE = 8'h03;
    localparam FSM_RD_TRAIL = 8'h04;
    localparam FSM_WR_WAIT = 8'h05;
    localparam FSM_WR_ACTIVE = 8'h06;
    localparam FSM_WR_TRAIL = 8'h07;


    reg  [15:0] last_wr_data = 0;
    reg  [31:0] cnt = 0;

    reg  [15:0] addr_int = 0;
    reg         rm_int = 0;
    reg         rw_int = 0;
    reg  [15:0] data_in_int = 0;

    wire [15:0] data_in;
    reg  [15:0] data_out = 0;
    reg         oen = 1'b1;

    assign data_in = oen ? data : 16'd0;
    assign data    = oen ? 16'dz : data_out;

    reg [7:0] cstate = FSM_IDLE;
    reg [7:0] nstate = FSM_IDLE;

    always @(posedge clk) begin
        if (!rstn) begin
            cstate <= FSM_IDLE;
        end else begin
            cstate <= nstate;
        end
    end

    always @(*) begin
        if (!rstn) begin
            nstate = FSM_IDLE;
        end else begin
            case (cstate)
                FSM_IDLE: begin
                    if ((str_n == 1'b0) && (sel_n == 1'b0)) begin
                        nstate = FSM_LEAD;
                    end else begin
                        nstate = FSM_IDLE;
                    end
                end

                FSM_LEAD: begin
                    if (rw_int == 1'b1) begin
                        nstate = FSM_RD_WAIT;
                    end else begin
                        nstate = FSM_WR_WAIT;
                    end
                end

                FSM_RD_WAIT: begin
                    if ((RD_READY_DELAY == cnt)) begin
                        nstate = FSM_RD_ACTIVE;
                    end else begin
                        nstate = FSM_RD_WAIT;
                    end
                end

                FSM_RD_ACTIVE: begin
                    nstate = FSM_RD_TRAIL;
                end

                FSM_RD_TRAIL: begin
                    if ((str_n == 1'b1)) begin
                        nstate = FSM_IDLE;
                    end else begin
                        nstate = FSM_RD_TRAIL;
                    end
                end

                FSM_WR_WAIT: begin
                    if ((WR_READY_DELAY == cnt)) begin
                        nstate = FSM_WR_ACTIVE;
                    end else begin
                        nstate = FSM_WR_WAIT;
                    end
                end

                FSM_WR_ACTIVE: begin
                    nstate = FSM_WR_TRAIL;
                end

                FSM_WR_TRAIL: begin
                    if ((str_n == 1'b1)) begin
                        nstate = FSM_IDLE;
                    end else begin
                        nstate = FSM_WR_TRAIL;
                    end
                end

                default: nstate = FSM_IDLE;
            endcase
        end
    end

    always @(posedge clk) begin
        if (!rstn) begin
            ioen_n <= 1'b1;
        end else begin
            case (nstate)
                FSM_RD_WAIT, FSM_RD_ACTIVE, FSM_RD_TRAIL: begin
                    if ((cnt >= RD_IOEN_DELAY - 1)) begin
                        if (RD_IOEN_HOLD > 0) begin
                            if (cnt < RD_IOEN_DELAY + RD_IOEN_HOLD - 1) begin
                                ioen_n <= 1'b0;
                            end else begin
                                ioen_n <= 1'b1;
                            end
                        end else begin
                            ioen_n <= 1'b0;
                        end
                    end else begin
                        ioen_n <= 1'b1;
                    end
                end
                FSM_WR_WAIT, FSM_WR_ACTIVE, FSM_WR_TRAIL: begin
                    if ((cnt >= WR_IOEN_DELAY - 1)) begin
                        if (WR_IOEN_HOLD > 0) begin
                            if (cnt < WR_IOEN_DELAY + WR_IOEN_HOLD - 1) begin
                                ioen_n <= 1'b0;
                            end else begin
                                ioen_n <= 1'b1;
                            end
                        end else begin
                            ioen_n <= 1'b0;
                        end
                    end else begin
                        ioen_n <= 1'b1;
                    end
                end
                default: ioen_n <= 1'b1;
            endcase
        end
    end

    always @(posedge clk) begin
        if (!rstn) begin
            data_out <= 0;
            oen      <= 1'b1;
        end else begin
            case (nstate)
                FSM_RD_WAIT, FSM_RD_ACTIVE, FSM_RD_TRAIL: begin
                    if ((cnt >= RD_DATA_DELAY - 1)) begin
                        if (RD_DATA_HOLD > 0) begin
                            if (cnt < RD_DATA_DELAY + RD_DATA_HOLD - 1) begin
                                data_out <= last_wr_data;
                                oen      <= 1'b0;
                            end else begin
                                data_out <= 0;
                                oen      <= 1'b1;
                            end
                        end else begin
                            data_out <= last_wr_data;
                            oen      <= 1'b0;
                        end
                    end else begin
                        data_out <= 0;
                        oen      <= 1'b1;
                    end
                end
                default: begin
                    data_out <= 0;
                    oen      <= 1'b1;
                end
            endcase
        end
    end

    always @(posedge clk) begin
        if (!rstn) begin
            ready_n <= 1'b1;
        end else begin
            case (nstate)
                FSM_RD_WAIT: begin
                    if ((cnt >= RD_READY_DELAY - 1)) begin
                        ready_n <= 1'b0;
                    end else begin
                        ready_n <= 1'b1;
                    end
                end
                FSM_WR_WAIT: begin
                    if ((cnt >= WR_READY_DELAY - 1)) begin
                        ready_n <= 1'b0;
                    end else begin
                        ready_n <= 1'b1;
                    end
                end
                default: ready_n <= 1'b1;
            endcase
        end
    end

    always @(posedge clk) begin
        if (!rstn) begin
            cnt <= 0;
        end else begin
            case (nstate)
                FSM_LEAD, FSM_RD_WAIT, FSM_RD_ACTIVE, FSM_WR_WAIT, FSM_WR_ACTIVE: begin
                    if (!(&cnt)) begin
                        cnt <= cnt + 1;
                    end
                end
                FSM_WR_TRAIL, FSM_RD_TRAIL: begin
                    cnt <= cnt;
                end
                default: cnt <= 0;
            endcase
        end
    end

    always @(negedge clk) begin
        if (!rstn) begin
            addr_int    <= 0;
            data_in_int <= 0;
            rm_int      <= 1'b1;
            rw_int      <= 1'b1;
        end else begin
            case (nstate)
                FSM_LEAD: begin
                    addr_int    <= addr;
                    rm_int      <= mem;
                    rw_int      <= rd;
                    data_in_int <= data_in;
                end
                FSM_RD_WAIT, FSM_RD_ACTIVE, FSM_WR_WAIT, FSM_WR_ACTIVE, FSM_WR_TRAIL, FSM_RD_TRAIL: begin
                    addr_int    <= addr_int;
                    data_in_int <= data_in_int;
                    rm_int      <= rm_int;
                    rw_int      <= rw_int;
                end
                default: begin
                    addr_int    <= 0;
                    data_in_int <= 0;
                    rm_int      <= 1'b1;
                    rw_int      <= 1'b1;
                end
            endcase
        end
    end

    always @(posedge clk) begin
        if (!rstn) begin
            last_wr_data <= 0;
        end else begin
            case (nstate)
                FSM_WR_ACTIVE: begin
                    last_wr_data <= data_in_int;
                end
                default: begin
                    last_wr_data <= last_wr_data;
                end
            endcase
        end
    end

endmodule

// verilog_format: off
`resetall
// verilog_format: on
