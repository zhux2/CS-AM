`timescale 1ns / 1ps


module InstrToImm(
	input [31:0] instr,   //32位指令
	input [2:0] ExtOp,    //扩展控制码
	output [31:0] imm
);
	wire [31:0] immI, immU, immS, immB, immJ;
	assign immI = {{20{instr[31]}}, instr[31:20]};
	assign immU = {instr[31:12], 12'b0};
	assign immS = {{20{instr[31]}}, instr[31:25], instr[11:7]};
	assign immB = {{20{instr[31]}}, instr[7], instr[30:25], instr[11:8], 1'b0};
	assign immJ = {{12{instr[31]}}, instr[19:12], instr[20], instr[30:21], 1'b0};
	
	assign imm = (ExtOp == 3'b000) ? immI :
	             (ExtOp == 3'b001) ? immU :
	             (ExtOp == 3'b010) ? immS :
	             (ExtOp == 3'b011) ? immB :
	             (ExtOp == 3'b100) ? immJ : immI;

endmodule
