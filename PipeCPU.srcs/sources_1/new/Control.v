`timescale 1ns / 1ps

module Control(
    output [2:0] ExtOp,
    output RegWr,
    output ALUASrc,
    output [1:0] ALUBSrc,
    output [3:0] ALUctr,
    output [2:0] Branch,
    output MemtoReg,
    output MemWr,
    output [2:0] MemOp,
    input  [6:0] opcode,
    input  [2:0] funct3,
    input  [6:0] funct7    
   );
    wire Ului, Uauipc, Iarith, R, J, Ijalr, B, Iload, S, NOP;
    
    assign Ului   = (opcode === 7'b0110111);
    assign Uauipc = (opcode === 7'b0010111);
    assign Iarith = (opcode === 7'b0010011);
    assign R      = (opcode === 7'b0110011);
    assign J      = (opcode === 7'b1101111);
    assign Ijalr  = (opcode === 7'b1100111);
    assign B      = (opcode === 7'b1100011);
    assign Iload  = (opcode === 7'b0000011);
    assign S      = (opcode === 7'b0100011); 
    assign NOP    = (opcode === 7'b0000000);
 
    assign RegWr  = (B | S | NOP) ? 1'b0 : 1'b1;
    assign ExtOp  = (Ului | Uauipc) ? 3'b001 :
                    (S) ? 3'b010 :
                    (J) ? 3'b100 :
                    (B) ? 3'b011 : 3'b000;
    assign ALUASrc = (Uauipc | J | Ijalr) ? 1'b1 : 1'b0;
    assign ALUBSrc = (Ului | Uauipc | Iarith | Iload | S) ? 2'b10 :
                     (R | B) ? 2'b00 :
                     (J | Ijalr) ? 2'b01 : 2'b00;
    assign ALUctr  = (Ului) ? 4'b1111 : 
                     (Iarith) ? ((funct3 === 3'b101 && funct7[5] === 1'b1) ? 4'b1101 : {1'b0, funct3}) :
                     (R) ? {funct7[5], funct3} :
                     (B) ? ((funct3[1] === 1'b1) ? 4'b0011 : 4'b0010) : 4'b0000;
    
    assign MemtoReg = Iload;
    assign MemWr    = S;
    assign Branch   = (J) ? 3'b001 : 
                      (Ijalr) ? 3'b010 :
                      (B) ? ((funct3 === 3'b000) ? 3'b100 : (funct3 === 3'b001) ? 3'b101 : (funct3 === 3'b100) ? 3'b110 : (funct3 === 3'b101) ? 3'b111 : funct3) :
                      3'b000;
    assign MemOp    = funct3;
endmodule
