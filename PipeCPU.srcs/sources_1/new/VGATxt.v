`timescale 1ns / 1ps

module VGATxt(
    input   wire            pix_clk  ,
    input   wire    [11:0]  pix_x  ,
    input   wire    [11:0]  pix_y  ,
    input   wire            pix_valid,
    input                   cpu_clk,
    input  [11:0]           txt_addr,
    input   [7:0]           txt_data,
    input                   txt_we,
    input  [11:0]           col_addr,
    input   [7:0]           col_data,
    input                   tcol_we,  
    output  wire    [11:0]  pix_data 
 );
parameter H_Pixels = 640,
           V_Pixels = 480;
parameter H_Txt = 80;
parameter [11:0] WHITE_RGB   = 12'b1111_1111_1111,
                  BLACK_RGB   = 12'b0000_0000_0000,
                  RED_RGB     = 12'b1111_0000_0000,
                  GREEN_RGB   = 12'b0000_1111_0000,
                  YELLOW_RGB  = 12'b1111_1111_0000,
                  BLUE_RGB    = 12'b0000_0000_1111,
                  MAGENTA_RGB = 12'b1111_0000_1111,
                  CYAN_RGB    = 12'b0000_1111_1111;
                  
wire [11:0] pix_txtaddr;
wire [7:0] ascii_code, txtcol;
assign pix_txtaddr = (pix_y / 16) * H_Txt + (pix_x / 8);
vga_txt  my_txt (.clka(cpu_clk), .wea(txt_we), .addra(txt_addr), .dina(txt_data), 
                 .clkb(pix_clk), .addrb(pix_txtaddr), .doutb(ascii_code));
vga_tcol my_tcol(.clka(cpu_clk), .wea(tcol_we), .addra(col_addr), .dina(col_data), 
                 .clkb(pix_clk), .addrb(pix_txtaddr), .doutb(txtcol));

wire [11:0] ascii_addr;
wire [7:0] image;
assign ascii_addr = ascii_code * 16 + (pix_y % 16);
vga_ascii my_ascii(.clka(pix_clk),.addra(ascii_addr),.douta(image));

wire white;
assign white = (pix_x % 8 == 0) ? image[3] :
               (pix_x % 8 == 1) ? image[2] :
               (pix_x % 8 == 2) ? image[1] :
               (pix_x % 8 == 3) ? image[0] :
               (pix_x % 8 == 4) ? image[7] :
               (pix_x % 8 == 5) ? image[6] :
               (pix_x % 8 == 6) ? image[5] : image[4];

wire [11:0] fg, bg;
assign fg = (txtcol[6:4] == 3'b000) ? BLACK_RGB   :
            (txtcol[6:4] == 3'b001) ? RED_RGB     :
            (txtcol[6:4] == 3'b010) ? GREEN_RGB   :
            (txtcol[6:4] == 3'b011) ? YELLOW_RGB  :
            (txtcol[6:4] == 3'b100) ? BLUE_RGB    :
            (txtcol[6:4] == 3'b101) ? MAGENTA_RGB :
            (txtcol[6:4] == 3'b110) ? CYAN_RGB    : WHITE_RGB;
assign bg = (txtcol[2:0] == 3'b000) ? BLACK_RGB   :
            (txtcol[2:0] == 3'b001) ? RED_RGB     :
            (txtcol[2:0] == 3'b010) ? GREEN_RGB   :
            (txtcol[2:0] == 3'b011) ? YELLOW_RGB  :
            (txtcol[2:0] == 3'b100) ? BLUE_RGB    :
            (txtcol[2:0] == 3'b101) ? MAGENTA_RGB :
            (txtcol[2:0] == 3'b110) ? CYAN_RGB    : WHITE_RGB;

assign pix_data = (pix_valid) ? ((white) ? fg : bg ) : BLACK_RGB;
endmodule
