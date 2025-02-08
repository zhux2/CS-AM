`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/24 16:07:05
// Design Name: 
// Module Name: VGAImage
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module VGAImage(
    input   wire            pix_clk  ,
    input   wire    [11:0]  pix_x  ,
    input   wire    [11:0]  pix_y  ,
    input   wire            pix_valid,
    input  [18:0]           wr_addr, //compute by software, use save instruction to save it
    input  [31:0]           wr_data,
    input                   we,
    input                   cpu_clk,
    output  wire    [11:0]  pix_data 
    );
parameter H_Pixels = 640,
           V_Pixels = 480;
           
wire [18:0] pix_addr;
wire [5:0] tmp_data;
assign pix_addr = pix_x + pix_y * H_Pixels;
vga_img my_img(.clka(cpu_clk), .wea(we), .addra(wr_addr[18:0]), .dina({wr_data[17:16], wr_data[9:8], wr_data[1:0]}),
               .clkb(pix_clk), .addrb(pix_addr), .doutb(tmp_data));
               
assign pix_data = {{2{tmp_data[5:4]}}, {2{tmp_data[3:2]}}, {2{tmp_data[1:0]}}};

endmodule
