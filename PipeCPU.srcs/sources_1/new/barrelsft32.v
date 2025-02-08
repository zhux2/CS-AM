`timescale 1ns / 1ps


module barrelsft32(
    output [31:0] dout,
    input [31:0] din,
    input [4:0] shamt,     
    input LR,           
    input AL            
);
    wire Rin, Lin;
    assign Rin = (AL) ? din[31] : 1'b0;
    assign Lin = 1'b0;
    
    wire [31:0] lev1, lev2, lev4, lev8, lev16;
    
    assign lev1 = (LR) ? (shamt[0] ? {din[30:0], Lin} : din) : (shamt[0] ? {Rin, din[31:1]} : din);
    assign lev2 = (LR) ? (shamt[1] ? {lev1[29:0], Lin, Lin} : lev1) : (shamt[1] ? {Rin, Rin, lev1[31:2]} : lev1);
    assign lev4 = (LR) ? (shamt[2] ? {lev2[27:0], {4{Lin}}} : lev2) : (shamt[2] ? {{4{Rin}}, lev2[31:4]} : lev2); 
    assign lev8 = (LR) ? (shamt[3] ? {lev4[23:0], {8{Lin}}} : lev4) : (shamt[3] ? {{8{Rin}}, lev4[31:8]} : lev4);
    assign lev16 = (LR) ? (shamt[4] ? {lev8[15:0], {16{Lin}}} : lev8) : (shamt[4] ? {{16{Rin}}, lev8[31:16]} : lev8);
    assign dout = lev16;
    
endmodule
