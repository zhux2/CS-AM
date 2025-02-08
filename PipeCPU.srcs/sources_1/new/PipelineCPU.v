`timescale 1ns / 1ps

module PipelineCPU(
	input 	      clk,
	input 	      reset,
	input  [31:0] initPC,
	output [31:0] InstrMemaddr,      //指令存储器地址
	input  [31:0] InstrMemdataout,   //指令内容
	output [31:0] DataMemaddr,       //数据存储器地址
	input  [31:0] DataMemdataout,   //数据存储器输出数据
	output [31:0] DataMemdatain,    //数据存储器写入数据
	output [2:0]  DataMemop,         //数据读写字节数控制信号
	output        DataMemwe,         //数据存储器写入使能信号
	output        DataMemre,
	output [31:0] dbgdata            //debug调试信号，输出16位指令存储器地址有效地址
);
	reg [31:0] PC = 0;
	wire [31:0] nxtPC, branchTarget, busa, busb, imm, alures;
	wire [6:0] opcode, funct7;
	wire [4:0] rd, rs1, rs2;
	wire [2:0] funct3, ExtOp, Branch, MemOp;
	wire [1:0] ALUBSrc;
	wire [3:0] ALUctr;
	wire RegWr, ALUASrc, MemtoReg, MemWr, NxtASrc, NxtBSrc, zero, PCsrc;

// pipeline register
reg [31:0] PC_ID, instr_ID;

reg [31:0] PC_DE, imm_DE, rs1_DE, rs2_DE;
reg [2:0]  Branch_DE, MemOp_DE;
reg [3:0]  ALUctr_DE;
reg [1:0]  ALUBSrc_DE;
reg [4:0]  rd_DE, r1_DE, r2_DE;
reg RegWr_DE, MemtoReg_DE, MemWr_DE, ALUASrc_DE;

reg [31:0] ALUres_EM, rs2_EM;
reg [4:0]  rd_EM;
reg [2:0]  MemOp_EM;
reg MemWr_EM, RegWr_EM, MemtoReg_EM;

reg [31:0] MemData_MW, ALUres_MW;
reg MemtoReg_MW;
reg [4:0] rd_MW;
reg RegWr_MW;

// Hazard
wire stallID, stallDE;
wire flushID, flushDE;
	
// IF
always @(negedge clk or posedge reset) begin
    if (reset) PC <= initPC;
    else if (stallID) PC <= PC;
    else if (PCsrc) PC <= branchTarget;
    else PC <= nxtPC;
end
assign nxtPC = PC + 4;
assign InstrMemaddr = PC;
assign dbgdata = PC;

always @(negedge clk or posedge reset) begin
    if (reset) {PC_ID, instr_ID} <= 64'b0;
    else if (flushID) {PC_ID, instr_ID} <= 64'b0;
    else if (~stallID) {PC_ID, instr_ID} <= {PC, InstrMemdataout};
end

	
// ID
wire [31:0] busw;
InstrParse cpu_InstrParse(.opcode(opcode), .funct3(funct3), .funct7(funct7), .rd(rd), .rs1(rs1), .rs2(rs2), .instr(instr_ID));
Control cpu_ctr(.ExtOp(ExtOp), .RegWr(RegWr), .ALUASrc(ALUASrc), .ALUBSrc(ALUBSrc), .ALUctr(ALUctr),
                .Branch(Branch), .MemtoReg(MemtoReg), .MemWr(MemWr), .MemOp(MemOp), .opcode(opcode), .funct3(funct3), .funct7(funct7));

regfile32  cpu_reg(.busa(busa), .busb(busb), .clk(clk), .ra(rs1), .rb(rs2), .rw(rd_MW), .busw(busw), .we(RegWr_MW));
InstrToImm cpu_imm(.instr(instr_ID), .ExtOp(ExtOp), .imm(imm));

always @(negedge clk or posedge reset) begin
    if (reset) {RegWr_DE, MemWr_DE, MemtoReg_DE, Branch_DE} <= 6'b0;
    else if (flushDE) begin
        {RegWr_DE, MemtoReg_DE, MemWr_DE, Branch_DE} <= 6'b0;
    end
    else begin
        {PC_DE, imm_DE, rs1_DE, rs2_DE} <= {PC_ID, imm, busa, busb};
        {r1_DE, r2_DE, rd_DE} <= {rs1, rs2, rd};
        {ALUASrc_DE, ALUBSrc_DE, ALUctr_DE, Branch_DE} <= {ALUASrc, ALUBSrc, ALUctr, Branch};
        {MemOp_DE, RegWr_DE, MemtoReg_DE, MemWr_DE} <= {MemOp, RegWr, MemtoReg, MemWr};
    end
end

// EX
wire [31:0] alu_a, alu_b, haz_a, haz_b;
assign haz_a = (RegWr_EM && rd_EM == r1_DE && rd_EM != 5'b0) ? ALUres_EM :
               (RegWr_MW && rd_MW == r1_DE && rd_MW != 5'b0) ? busw : rs1_DE;
assign haz_b = (RegWr_EM && rd_EM == r2_DE && rd_EM != 5'b0) ? ALUres_EM :
               (RegWr_MW && rd_MW == r2_DE && rd_MW != 5'b0) ? busw : rs2_DE;
assign alu_a = (ALUASrc_DE) ? PC_DE : haz_a;
assign alu_b = (ALUBSrc_DE === 2'b10) ? imm_DE :
               (ALUBSrc_DE === 2'b01) ? 4 : haz_b;
ALU32 cpu_alu(.result(alures), .zero(zero), .dataa(alu_a), .datab(alu_b), .aluctr(ALUctr_DE));

wire [31:0] brTarsrc;
assign brTarsrc = (~NxtASrc) ? PC_DE : haz_a;
assign branchTarget = brTarsrc + imm_DE;
assign flushID = PCsrc;
assign flushDE = PCsrc | stallID;
BranchControl cpu_bctr(.NxtASrc(NxtASrc), .PCsrc(PCsrc), .zero(zero), .result0(alures[0]), .Branch(Branch_DE));

//load use
assign stallID = MemtoReg_DE && (rd_DE != 5'b0) && (rd_DE == rs1 || rd_DE == rs2);

always @(negedge clk or posedge reset) begin
    if (reset) {MemWr_EM, RegWr_EM} <= 2'b00;
    else begin
        {ALUres_EM, rs2_EM, rd_EM} <= {alures, haz_b, rd_DE};
        {MemOp_EM, MemWr_EM, RegWr_EM, MemtoReg_EM} <= {MemOp_DE, MemWr_DE, RegWr_DE, MemtoReg_DE};
    end
end

// ME
assign DataMemaddr   = ALUres_EM;
assign DataMemdatain = rs2_EM;
assign DataMemop     = MemOp_EM;
assign DataMemwe     = MemWr_EM;
assign DataMemre     = MemtoReg_EM;

always @(negedge clk or posedge reset) begin
    if (reset) RegWr_MW <= 1'b0;
    else begin
        {MemData_MW, ALUres_MW} <= {DataMemdataout, ALUres_EM};
        {RegWr_MW, MemtoReg_MW, rd_MW} <= {RegWr_EM, MemtoReg_EM, rd_EM};
    end
end

// WB
assign busw = (MemtoReg_MW) ? MemData_MW : ALUres_MW;

endmodule
