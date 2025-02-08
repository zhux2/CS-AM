`timescale 1ns / 1ps

module ALU32(
    output  [31:0] result, 
    output  zero,   
    input   [31:0] dataa,  
    input   [31:0] datab,  
    input   [3:0] aluctr        
); 

    wire SUBctr, SIGctr, ALctr, SFTctr;
    wire [2:0] OPctr;
    wire sltout, OF, CF, SF;
    wire [31:0] adderout, andout, orout, xorout, sftout;
    
    assign SUBctr = (&{~aluctr[3], ~aluctr[2], aluctr[1]}) | (&{aluctr[3], ~aluctr[2], ~aluctr[1:0]});
    assign SIGctr = &{~aluctr[3:2], aluctr[1], ~aluctr[0]};
    assign ALctr = &{aluctr[3:2], ~aluctr[1], aluctr[0]};
    assign SFTctr = &{~aluctr[3:1], aluctr[0]};
    assign OPctr[2] = (&{~aluctr[3], ~aluctr[1], aluctr[0]}) | 
                      (&{aluctr[3:2], aluctr[0]}) | 
                      (&{~aluctr[3:2], aluctr[1]});
    assign OPctr[1] = (&{~aluctr[3:2], aluctr[1]}) |
                      (&{~aluctr[3], aluctr[2], ~aluctr[0]});
    assign OPctr[0] = (&{aluctr[2:0]}) | (&{~aluctr[3], aluctr[2], ~aluctr[1:0]});
    
    Adder32 my_adder(.f(adderout), .OF(OF), .SF(SF), .ZF(zero), .CF(CF), .x(dataa), .y(datab), .sub(SUBctr));

    barrelsft32 my_barrel(.dout(sftout), .din(dataa), .shamt(datab[4:0]), .LR(SFTctr), .AL(ALctr));
    
    assign andout = dataa & datab;
    assign orout = dataa | datab;
    assign xorout = dataa ^ datab;
    assign sltout = SIGctr ? (OF ^ SF) : CF;
    
    assign result = (OPctr == 0) ? adderout : 
                    (OPctr == 1) ? andout :
                    (OPctr == 2) ? orout :
                    (OPctr == 3) ? xorout :
                    (OPctr == 4) ? sftout :
                    (OPctr == 5) ? datab : {{31{1'b0}}, sltout};
    
endmodule