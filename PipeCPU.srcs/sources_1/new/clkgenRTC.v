`timescale 1ns / 1ps

module clkgenRTC(
   input clkin, 
   input rst, 
   input clken, 
   output reg clkout
  );
    parameter clk_freq=1000000;
    parameter countlimit=100000000/2/clk_freq-1; 
   reg[31:0] clkcount;
   initial
   begin clkcount=32'd0; clkout=1'b0; end
   always @ (posedge clkin) 
    if(rst)
     begin
        clkcount<=0;
        clkout<=1'b0;
     end
    else
    begin
     if(clken)
        begin
            if(clkcount>=countlimit)
             begin
                clkcount<=32'd0;
                clkout<=~clkout;
             end
             else
             begin
                clkcount<=clkcount+1;

             end
         end
     end  
endmodule
