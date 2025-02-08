`timescale 1ns / 1ps

module Light(
    input cpu_clk, rst, CLK100MHZ,
    input  [3:0]  led_addr,
    input         led_data,
    input         led_we,
    input  [2:0]  seg_addr,
    input  [3:0]  seg_data,
    input         seg_we,
    output [15:0] LED,
    output [6:0]  SEG,
    output [7:0]  AN 
    );
reg [15:0] reg_led;
reg [3:0] reg_seg0, reg_seg1, reg_seg2, reg_seg3, reg_seg4, reg_seg5, reg_seg6, reg_seg7;

always @(posedge cpu_clk or posedge rst) begin
    if (rst) reg_led <= 16'h0000;
    else if (led_we) begin
        reg_led[led_addr] <= led_data;
    end
end

assign LED = reg_led;

always @(posedge cpu_clk or posedge rst) begin
    if (rst) begin 
        {reg_seg0, reg_seg1, reg_seg2, reg_seg3, 
         reg_seg4, reg_seg5, reg_seg6, reg_seg7} <= 32'b0; end
    else if (seg_we) begin
        case(seg_addr)
            0: reg_seg0 <= seg_data;
            1: reg_seg1 <= seg_data;
            2: reg_seg2 <= seg_data;
            3: reg_seg3 <= seg_data;
            4: reg_seg4 <= seg_data;
            5: reg_seg5 <= seg_data;
            6: reg_seg6 <= seg_data;
            7: reg_seg7 <= seg_data;
        endcase
    end
end

seg7decimal my_seg(.x({reg_seg7, reg_seg6, reg_seg5, reg_seg4,
                       reg_seg3, reg_seg2, reg_seg1, reg_seg0}),
                   .clk(CLK100MHZ), .seg(SEG), .an(AN));

endmodule
