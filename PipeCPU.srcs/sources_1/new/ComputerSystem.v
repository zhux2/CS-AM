`timescale 1ns / 1ps

module ComputerSystem(
    input CLK100MHZ,
    input rst,
    input PS2_CLK,
    input PS2_DATA,
    input [15:0] sw,
    output [6:0]SEG,
    output [7:0]AN,     
    output [15:0] LED,   
    output [3:0] VGA_R,
    output [3:0] VGA_G,
    output [3:0] VGA_B,
    output  VGA_HS,
    output  VGA_VS
//    ,output [31:0] debugiaddr, debugdata,
//    output [31:0] debuginstr, debugdaddr, 
//    output dbdwe, dbre, dbsw_re, dbtxt_we, dbtcol_we, dbled_we, dbvgaconf_we, cpu_clk_
    );
wire pix_clk;
wire cpu_clk;
clkgenCPU clkgen_cpu(.clkin(CLK100MHZ), .rst(1'b0), .clken(1'b1), .clkout(cpu_clk));
clkgen    ckjgen_pix(.clkin(CLK100MHZ), .rst(1'b0), .clken(1'b1), .clkout(pix_clk));

//CPU
wire [31:0] iaddr,idataout;
wire [31:0] daddr,ddataout, cpu_din, cpu_dout;
wire cpu_we, dwe, cpu_re;
wire [2:0]  dop;
wire [15:0] cpudbgdata;
PipelineCPU  mycpu(.clk(cpu_clk), 
                 .reset(rst), .initPC(32'b0),
				 .InstrMemaddr(iaddr), .InstrMemdataout(idataout),
				 .DataMemaddr(daddr), .DataMemdataout(cpu_dout), .DataMemdatain(cpu_din),
				 .DataMemop(dop), .DataMemwe(cpu_we), .DataMemre(cpu_re), .dbgdata(cpudbgdata));
				 //reg [31:0] tmp = 32'h00000004;
Pmem mypmem(.cpuclk(cpu_clk), .iaddr(iaddr), .idataout(idataout), 
                              .daddr(daddr), .ddataout(ddataout), .ddatain(cpu_din), .MemOp(dop), .dwe(dwe));
assign dwe = cpu_we & (daddr[31:28] == 4'h0);

//assign debuginstr = idataout;
//assign debugdaddr = daddr;
//assign debugiaddr = iaddr;
//assign dbdwe = dwe;
//assign dbre = cpu_re;
//assign debugdata = cpu_dout;
//assign cpu_clk_ = cpu_clk;
//IO mapping
wire kb_re, vga_we, rtc_re, txt_we, tcol_we, led_we, seg_we, sw_re, vgaconf_we;
assign txt_we = cpu_we & (daddr[31:28] == 4'h2);
assign tcol_we= cpu_we & (daddr[31:28] == 4'h3);
assign kb_re  = cpu_re & (daddr[31:28] == 4'h4);
assign rtc_re = cpu_re & (daddr[31:28] == 4'h5);
assign vga_we = cpu_we & (daddr[31:28] == 4'h6);
assign led_we = cpu_we & (daddr[31:28] == 4'h7);
assign seg_we = cpu_we & (daddr[31:28] == 4'h8);
assign sw_re  = cpu_re & (daddr[31:28] == 4'h9);
assign vgaconf_we = cpu_we & (daddr[31:28] == 4'ha);

//assign dbsw_re = sw_re;
//assign dbtxt_we = txt_we;
//assign dbtcol_we = tcol_we;
//assign dbled_we = led_we;
//assign dbvgaconf_we = vgaconf_we;
//Devices
//KB
wire [31:0] key;
KBSim mykb (.cpu_clk(cpu_clk), .PS2_CLK(PS2_CLK), .PS2_DATA(PS2_DATA), .rst(rst), .kb_we(kb_re), .key_val(key));

//RTC
wire [31:0] rtc;
RTC   myrtc(.CLK100MHZ(CLK100MHZ), .rst(rst), .addr(daddr[2]), .rtc(rtc));

//VGA
wire [11:0] pix_x, pix_y;
wire pix_valid;
VGACtrl vga_ctrl(.pix_clk(pix_clk), .pix_rst(rst), .pix_x(pix_x), .pix_y(pix_y), 
                 .hsync(VGA_HS), .vsync(VGA_VS), .pix_valid(pix_valid));
//VGA_CONF
reg vga_s;
always @(posedge cpu_clk or posedge rst) begin
    if (rst) vga_s <= 1'b0;
    else if (vgaconf_we) vga_s <= cpu_din[0];
end
//VGA_FB
wire [11:0] pix_data_img;
VGAImage vga_img (.pix_clk(pix_clk), .pix_x(pix_x), .pix_y(pix_y), .pix_valid(pix_valid), 
                  .wr_addr(daddr[18:0]), .wr_data(cpu_din), .we(vga_we), .cpu_clk(cpu_clk), .pix_data(pix_data_img));
//TXT_FB
wire [11:0] pix_data_txt;
VGATxt   vga_txt (.pix_clk(pix_clk), .pix_x(pix_x), .pix_y(pix_y), .pix_valid(pix_valid), 
                  .cpu_clk(cpu_clk), .txt_addr(daddr[11:0]), .txt_data(cpu_din[7:0]), .txt_we(txt_we), 
                  .col_addr(daddr[11:0]), .col_data(cpu_din[7:0]), .tcol_we(tcol_we), .pix_data(pix_data_txt));
//VGA output
wire [11:0] vga_data;
assign vga_data = (vga_s) ? pix_data_txt : pix_data_img; 
assign VGA_R = vga_data[11:8];
assign VGA_G = vga_data[7:4];
assign VGA_B = vga_data[3:0];

assign cpu_dout = (kb_re)  ? key : 
                  (rtc_re) ? rtc :
                  (sw_re)  ? {{16{1'b0}}, sw}  : ddataout;
//assign LED = rtc[31:16];
//Light
Light mylight(.cpu_clk(cpu_clk), .rst(rst), .CLK100MHZ(CLK100MHZ), 
              .led_addr(daddr[3:0]), .led_data(cpu_din[0]), .led_we(led_we),
              .seg_addr(daddr[2:0]), .seg_data(cpu_din[3:0]), .seg_we(seg_we),
              .LED(LED), .SEG(SEG), .AN(AN));
endmodule
