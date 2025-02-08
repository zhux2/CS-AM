`timescale 1ns / 1ps

module KBSim(
    input cpu_clk,   
    input PS2_CLK,    
    input PS2_DATA, 
    input rst,
    input kb_we,
    output reg [31:0] key_val
);
    // CS-AM relevant
    wire [7:0] keymap;
    reg  [7:0] keyqueue[31:0];
    reg [31:0] keydown;
    reg  [4:0] key_f, key_r;
    
    reg [7:0] curcode;
    reg curkeydown;
    //reg CapsLockOn, NumLockOn, ShiftOn, CtrlOn, AltOn, Shift;
    reg FO, EO;
    parameter reg [7:0] _F0_ = 8'b1111_0000,
                        _E0_ = 8'b1110_0000,
                        _CAPS_ = 8'b0101_1000,
                        _NUML_ = 8'b0111_0111,
                        _SHIF_ = 8'b0001_0010,
                        _CTRL_ = 8'b0001_0100,
                        _ALT_ = 8'b0001_0001;
    reg FO2, FO1;
    wire kclkf, kdataf;
    reg [7:0]datacur;             
    reg [7:0]dataprev; 
    reg [3:0]cnt;               
    reg flag;  
    
    assign kclkf = PS2_CLK;
    assign kdataf = PS2_DATA;
    always@(negedge(kclkf) or posedge rst)begin
    if (rst) begin
        cnt<=4'b0000;
        flag <= 1'b0;
    end
    else begin
     case(cnt)
            1:datacur[0]<=kdataf;
            2:datacur[1]<=kdataf;
            3:datacur[2]<=kdataf;
            4:datacur[3]<=kdataf;
            5:datacur[4]<=kdataf;
            6:datacur[5]<=kdataf;
            7:datacur[6]<=kdataf;
            8:begin datacur[7]<=kdataf; flag <= 1'b1; end
            9:flag<=1'b0;        
            //10:begin flag<=1'b0; end    
          endcase
        if(cnt<=9) cnt<=cnt+1;
        else if(cnt==10)  cnt<=0;
        end
    end
    
    reg enqueue;
    always @(negedge kclkf or posedge rst)begin
        if (rst) begin
            {FO, EO, FO2, FO1} <= 4'h0;
            {curcode} <= 8'h00;
            key_r <= 5'b0; enqueue <= 1'b0;
        end
        else if (flag) begin
            if (datacur === _F0_) {FO2, FO1} <= 2'b01;
            else if(FO1 === 1'b1) {FO2, FO1} <= 2'b10; 
            else {FO2, FO1} <= 2'b00;
            if (dataprev!=datacur || FO2 === 1'b1) begin         
                dataprev<=datacur;
                if (datacur === _F0_) FO <= 1'b1;
                else if (datacur === _E0_) begin EO <= 1'b1; end
                else begin
                    if (FO) begin 
                        FO <= 1'b0; EO <= 1'b0;
                        curkeydown <= 1'b0;
                    end
                    else curkeydown <= 1'b1;
                    if (EO) begin
                        if (datacur[7:4] == 4'h1) curcode <= {4'ha, datacur[3:0]};
                        else if (datacur[7:4] == 4'h6) curcode <= {4'h8, datacur[3:0]};
                        else curcode <= {4'h9, datacur[3:0]}; //datacur[7:4] == 4'h7
                    end
                    else curcode <= datacur;
                    enqueue <= 1'b1;
                end
            end
        end
        else if (enqueue) begin
            enqueue <= 1'b0;
            key_r <= key_r + 1;
            keyqueue[key_r] <= keymap;
            keydown[key_r] <= curkeydown;
        end
    end
    
    kbcodemap kbmap(.keymap(keymap), .kbcode(curcode));
    
    always @(posedge cpu_clk or posedge rst) begin
        if (rst) key_f <= 5'b0; 
        else if (kb_we) begin
            if (key_f === key_r) key_val <= 32'b0;
            else begin
                key_f <= key_f + 1;
                key_val <= {16'b0, keydown[key_f], 7'b0, keyqueue[key_f]};
            end
        end
    end
        
endmodule
