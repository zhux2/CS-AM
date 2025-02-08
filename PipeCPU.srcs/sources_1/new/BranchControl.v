`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/12/01 12:15:12
// Design Name: 
// Module Name: BranchControl
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


module BranchControl(
    output NxtASrc,
    output PCsrc,
    input  zero,
    input  result0,
    input [2:0] Branch
    );
    
    assign NxtASrc = (Branch == 3'b010);
    assign PCsrc   = (Branch == 3'b001 || Branch == 3'b010) ||
                     (Branch == 3'b100 && zero == 1'b1) ||
                     (Branch == 3'b101 && zero == 1'b0) ||
                     (Branch == 3'b110 && result0 == 1'b1) ||
                     (Branch == 3'b111 && result0 == 1'b0);
endmodule
