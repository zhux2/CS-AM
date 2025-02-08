`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/11/25 14:09:41
// Design Name: 
// Module Name: RTC
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


module RTC(
    input CLK100MHZ,  
    input cpu_clk,
    input rst,
    input addr,
    output [31:0] rtc
    );
wire rtc_clk;
clkgenRTC clkgen_rtc(.clkin(CLK100MHZ), .rst(1'b0), .clken(1'b1), .clkout(rtc_clk));

reg [63:0] timer;
always @(posedge rtc_clk or posedge rst) begin
    if (rst) timer <= 64'b0;
    else timer <= timer + 1;
end
assign rtc = (addr == 1'b1) ? timer[63:32] : timer[31:0];
//always @(posedge cpu_clk or posedge rst) begin
//    if (rst) rtc <= 32'b0;
//    else rtc <= (addr) ? timer[63:32] : timer[31:0];
//end
endmodule
