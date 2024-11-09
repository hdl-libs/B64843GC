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
// Module Name   : ace_interface
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

module ace_interface #(
    parameter integer EXPAND_ADDRESS_BITS = 2,
    parameter integer VALID_ADDRESS_BITS  = 13
) (
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PADDR" *)
    input  wire [31:0] s_apb_paddr,    // Address (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PENABLE" *)
    input  wire        s_apb_penable,  // Enable (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PRDATA" *)
    output reg  [31:0] s_apb_prdata,   // Read Data (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PREADY" *)
    output reg         s_apb_pready,   // Slave Ready (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PSEL" *)
    input  wire        s_apb_psel,     // Slave Select (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PSLVERR" *)
    output wire        s_apb_pslverr,  // Slave Error Response (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PWDATA" *)
    input  wire [31:0] s_apb_pwdata,   // Write Data (required)
    (* X_INTERFACE_INFO = "xilinx.com:interface:apb:1.0 S_APB PWRITE" *)
    input  wire        s_apb_pwrite,   // Write Control (required)
    //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_RTAD_LAT" *)
    output wire        ace_rtad_lat,   //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_RTAD" *)
    output wire [ 4:0] ace_rtad,       //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_RTADP" *)
    output wire        ace_rtadp,      //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_INT_N" *)
    input  wire        ace_int_n,      //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_CS_N" *)
    output reg         ace_cs_n,       //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_STRB_N" *)
    output reg         ace_strb_n,     //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_M_RN" *)
    output reg         ace_m_rn,       //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_R_WN" *)
    output reg         ace_r_wn,       //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_RDY_N" *)
    input  wire        ace_rdy_n,      //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_IOE_N" *)
    input  wire        ace_ioe_n,      //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_RDATA" *)
    input  wire [15:0] ace_rdata,      //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_WDATA" *)
    output reg  [15:0] ace_wdata,      //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_OE_N" *)
    output reg         ace_oe_n,       //
    (* X_INTERFACE_INFO = "john_tito:interface:B64843GC:1.1 M_ACE M_ADDR" *)
    output reg  [15:0] ace_addr,       //
    //
    (* X_INTERFACE_INFO = "xilinx.com:signal:interrupt:1.0 interrupt interrupt" *)
    (* X_INTERFACE_PARAMETER = "SENSITIVITY LEVEL_HIGH" *)
    output wire        interrupt,      //  (required)
    //
    (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 aclk ace_clk" *)
    (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF M_ACE, ASSOCIATED_RESET ace_rst_n" *)
    output wire        ace_clk,        // CLK
    (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 aresetn ace_rst_n" *)
    (* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
    output reg         ace_rst_n,      // RST_N
    //
    (* X_INTERFACE_INFO = "xilinx.com:signal:clock:1.0 aclk aclk" *)
    (* X_INTERFACE_PARAMETER = "ASSOCIATED_BUSIF S_APB, ASSOCIATED_RESET aresetn" *)
    input  wire        aclk,           //
    (* X_INTERFACE_INFO = "xilinx.com:signal:reset:1.0 aresetn aresetn" *)
    (* X_INTERFACE_PARAMETER = "POLARITY ACTIVE_LOW" *)
    input  wire        aresetn         //
);

    localparam IDLE = 2'b00;
    localparam RW = 2'b01;
    localparam UPDATA = 2'b10;

    reg [1:0] cstate;
    reg [1:0] nstate;
    reg       s_apb_penable_reg;
    reg       s_apb_preq;
    reg [7:0] rst_cnt;

    assign s_apb_pslverr = 1'b0;
    assign ace_clk       = aclk;
    assign ace_rtadp     = 1'b0;
    assign ace_rtad      = 5'b00001;
    assign ace_rtad_lat  = 1'b1;
    assign interrupt     = ~ace_int_n;

    always @(posedge aclk) begin
        if (!aresetn) begin
            s_apb_penable_reg <= 1'b0;
            s_apb_preq        <= 1'b0;
        end else begin
            s_apb_penable_reg <= (s_apb_penable & s_apb_psel);
            s_apb_preq        <= (s_apb_penable & s_apb_psel) & ~s_apb_penable_reg;
        end
    end

    always @(posedge aclk) begin
        if (!aresetn) begin
            ace_m_rn  <= 1'b1;
            ace_r_wn  <= 1'b1;
            ace_addr  <= 16'd0;
            ace_wdata <= 16'd0;
            ace_oe_n  <= 1'b1;
        end else begin
            if ((nstate == IDLE) && (s_apb_penable == 1'b1) && (s_apb_pready == 1'b0)) begin
                ace_m_rn                        <= {s_apb_paddr[EXPAND_ADDRESS_BITS+VALID_ADDRESS_BITS]};
                ace_addr[0+:VALID_ADDRESS_BITS] <= {s_apb_paddr[EXPAND_ADDRESS_BITS+:VALID_ADDRESS_BITS]};
                ace_wdata                       <= s_apb_pwdata[15:0];
                ace_r_wn                        <= ~s_apb_pwrite;
                ace_oe_n                        <= ~s_apb_pwrite;
            end else if (nstate == IDLE) begin
                ace_m_rn  <= 1'b1;
                ace_r_wn  <= 1'b1;
                ace_addr  <= 16'h0000;
                ace_wdata <= 16'd0;
                ace_oe_n  <= 1'b1;
            end else begin
                ace_m_rn  <= ace_m_rn;
                ace_r_wn  <= ace_r_wn;
                ace_addr  <= ace_addr;
                ace_wdata <= ace_wdata;
                ace_oe_n  <= ace_oe_n;
            end
        end
    end

    always @(posedge aclk) begin
        if (!aresetn) begin
            s_apb_pready <= 1'b0;
        end else begin
            if (nstate == UPDATA) begin
                s_apb_pready <= 1'b1;
            end else begin
                s_apb_pready <= 1'b0;
            end
        end
    end

    always @(posedge aclk) begin
        if (!aresetn) begin
            s_apb_prdata <= 0;
        end else begin
            if ((nstate == UPDATA) && (ace_r_wn == 1'b1)) begin
                s_apb_prdata <= {16'h00, ace_rdata};
            end else begin
                s_apb_prdata <= s_apb_prdata;
            end
        end
    end

    always @(posedge aclk) begin
        if (!aresetn) begin
            ace_cs_n   <= 1'b1;
            ace_strb_n <= 1'b1;
        end else begin
            if ((nstate == RW) || (nstate == UPDATA)) begin
                ace_cs_n   <= 1'b0;
                ace_strb_n <= 1'b0;
            end else begin
                ace_cs_n   <= 1'b1;
                ace_strb_n <= 1'b1;
            end
        end
    end

    always @(posedge aclk) begin
        if (!aresetn) cstate <= IDLE;
        else cstate <= nstate;
    end

    always @(*) begin
        if (!aresetn) begin
            nstate = IDLE;
        end else begin
            case (cstate)
                IDLE: begin
                    if (s_apb_preq == 1'b1) nstate = RW;
                    else nstate = IDLE;
                end
                RW: begin
                    if (ace_rdy_n == 1'b0) nstate = UPDATA;
                    else nstate = RW;
                end
                UPDATA: begin
                    nstate = IDLE;
                end
                default: nstate = IDLE;
            endcase
        end
    end

    always @(posedge aclk) begin
        if (!aresetn) begin
            rst_cnt <= 8'hFF;
        end else begin
            if (rst_cnt) begin
                rst_cnt <= rst_cnt - 8'h01;
            end else begin
                rst_cnt <= 8'h00;
            end
        end
        ace_rst_n <= ~|rst_cnt;
    end

endmodule

// verilog_format: off
`resetall
// verilog_format: on
