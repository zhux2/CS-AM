`timescale 1ns / 1ps

module testbench();
reg clk;
reg rst = 0;
reg [15:0] sw;
wire cpu_clk;
wire [31:0] iaddr, instr, daddr, data;
wire cpu_re, dwe, sw_re, txt_we, tcol_we, led_we, vgaconf_we;

initial begin
    clk = 0;
    forever #1 clk = ~clk;
end

ComputerSystem mycs(
    .CLK100MHZ(clk), //for test
    .rst(rst),
    .sw(sw),
    .debugiaddr(iaddr), .debuginstr(instr), .debugdaddr(daddr), .debugdata(data),
    .dbdwe(dwe), .dbre(cpu_re), .dbsw_re(sw_re), .dbtxt_we(txt_we), .dbtcol_we(tcol_we), .dbled_we(led_we), 
    .dbvgaconf_we(vgaconf_we), .cpu_clk_(cpu_clk)
    );

initial begin
//    $readmemh("amtest-0.hex", mycs.mypmem.mypmem0.ram);
//    $readmemh("amtest-1.hex", mycs.mypmem.mypmem1.ram);
//    $readmemh("amtest-2.hex", mycs.mypmem.mypmem2.ram);
//    $readmemh("amtest-3.hex", mycs.mypmem.mypmem3.ram);
//    $display("Hey it's instruction : %h", mycs.mypmem.mypmem0.ram[0]);
    #5 rst = 1'b1; sw = 16'h0001;
    #5 rst = 1'b0;
    #100 $stop;
end
endmodule

