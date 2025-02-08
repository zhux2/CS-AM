`timescale 1ns / 1ps

module Pmem(
    input cpuclk,
    input  [31:0] iaddr,
    output [31:0] idataout,
    input  [31:0] daddr,
    output [31:0] ddataout,
    input  [31:0] ddatain,
    input  [2:0] MemOp,
    input  dwe
    );
wire [7:0] byte0, byte1, byte2, byte3;
wire [7:0] inst0, inst1, inst2, inst3;
wire [7:0] din0,  din1,  din2,  din3 ;
wire [3:0] we;
pmem0 mypmem0(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst0), 
              .clkb(cpuclk), .web(we[0]), .addrb(daddr[17:2]), .dinb(din0), .doutb(byte0));
pmem1 mypmem1(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst1), 
              .clkb(cpuclk), .web(we[1]), .addrb(daddr[17:2]), .dinb(din1), .doutb(byte1));    
pmem2 mypmem2(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst2), 
              .clkb(cpuclk), .web(we[2]), .addrb(daddr[17:2]), .dinb(din2), .doutb(byte2));
pmem3 mypmem3(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst3), 
              .clkb(cpuclk), .web(we[3]), .addrb(daddr[17:2]), .dinb(din3), .doutb(byte3));
              
//mem8b mypmem0(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst0), 
//              .clkb(memclk), .web(we[0]), .addrb(daddr[17:2]), .dinb(din0), .doutb(byte0));
//mem8b mypmem1(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst1), 
//              .clkb(memclk), .web(we[1]), .addrb(daddr[17:2]), .dinb(din1), .doutb(byte1));    
//mem8b mypmem2(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst2), 
//              .clkb(memclk), .web(we[2]), .addrb(daddr[17:2]), .dinb(din2), .doutb(byte2));
//mem8b mypmem3(.clka(cpuclk), .wea(1'b0),  .addra(iaddr[17:2]), .dina(8'b0), .douta(inst3), 
//              .clkb(memclk), .web(we[3]), .addrb(daddr[17:2]), .dinb(din3), .doutb(byte3));
              
assign idataout = {inst3, inst2, inst1, inst0};

assign we[0] = dwe & ((MemOp[1:0] == 2'b00 && daddr[1:0] == 2'b00) | (MemOp[1:0] == 2'b10) | (MemOp[1:0] == 2'b01 && daddr[1] == 1'b0));
assign we[1] = dwe & ((MemOp[1:0] == 2'b00 && daddr[1:0] == 2'b01) | (MemOp[1:0] == 2'b10) | (MemOp[1:0] == 2'b01 && daddr[1] == 1'b0));
assign we[2] = dwe & ((MemOp[1:0] == 2'b00 && daddr[1:0] == 2'b10) | (MemOp[1:0] == 2'b10) | (MemOp[1:0] == 2'b01 && daddr[1] == 1'b1));
assign we[3] = dwe & ((MemOp[1:0] == 2'b00 && daddr[1:0] == 2'b11) | (MemOp[1:0] == 2'b10) | (MemOp[1:0] == 2'b01 && daddr[1] == 1'b1));

assign din0 = ddatain[7:0];
assign din1 = (MemOp[1:0] == 2'b00 && daddr[1:0] == 2'b01) ? ddatain[7:0] : ddatain[15:8];
assign din2 = (MemOp[1:0] == 2'b10) ? ddatain[23:16] : ddatain[7:0];
assign din3 = (MemOp[1:0] == 2'b10) ? ddatain[31:24] : 
              (MemOp[1:0] == 2'b00) ? ddatain[7:0]   : ddatain[15:8];

assign ddataout = (MemOp == 3'b010) ? {byte3, byte2, byte1, byte0} :
                  (MemOp == 3'b100) ? 
                    ((daddr[1:0] == 2'b00) ? {{24{1'b0}}, byte0} :
                     (daddr[1:0] == 2'b01) ? {{24{1'b0}}, byte1} :
                     (daddr[1:0] == 2'b10) ? {{24{1'b0}}, byte2} : {{24{1'b0}}, byte3} ) :
                  (MemOp == 3'b101) ? 
                    ((daddr[1] == 1'b0) ? {{16{1'b0}}, byte1, byte0} : {{16{1'b0}}, byte3, byte2}) :
                  (MemOp == 3'b000) ? 
                    ((daddr[1:0] == 2'b00) ? {{24{byte0[7]}}, byte0} :
                     (daddr[1:0] == 2'b01) ? {{24{byte1[7]}}, byte1} :
                     (daddr[1:0] == 2'b10) ? {{24{byte2[7]}}, byte2} : {{24{byte3[7]}}, byte3} ) :
		 //MemOp == 3'b001
                    ((daddr[1] == 1'b0) ? {{16{byte1[7]}}, byte1, byte0} : {{16{byte3[7]}}, byte3, byte2});
endmodule
