`timescale 1ns/1ns
module segment
#(parameter ZERO = 4'h0,ONE = 4'h1,TWO = 4'h2,THREE = 4'h3,
    FOUR = 4'h4,FIVE = 4'h5,SIX = 4'h6,SEVEN = 4'h7,
    EIGHT = 4'h8,NINE = 4'h9,DECIMAL_POINT = 4'hA,
    BLANK = 4'hB,MINUS = 4'hC,d = 4'hD,l = 4'hE
)
(
    clk_sys,
    clk,
    reset,
    Disp_Data,
    Sel,
    led,
    );
    input clk_sys;
    input clk;
    input reset;
    input [15:0]Disp_Data;
    output reg [3:0]Sel;
    output reg [7:0]led;

    
    reg [1:0]Num;
    always@(posedge clk or negedge reset)
    if(~reset)
        Num <= 0;
    else  Num <= Num + 1'd1;
    
    // 3_8译码器
    always@(posedge clk_sys)
        case(Num)
            3'b000: Sel = 4'b0001;
            3'b001: Sel = 4'b0010;
            3'b010: Sel = 4'b0100;
            3'b011: Sel = 4'b1000;
        endcase
    
    // 8选1多路器
    reg [3:0]Disp_tmp;
    always@(posedge clk_sys)
        case(Num)
            3'b000: Disp_tmp = Disp_Data[3:0];
            3'b001: Disp_tmp = Disp_Data[7:4];
            3'b010: Disp_tmp = Disp_Data[11:8];
            3'b011: Disp_tmp = Disp_Data[15:12];

        endcase
    always@(posedge clk_sys)
        case(Disp_tmp)
 ZERO:
            led = 8'hFC;
            ONE:
            led = 8'h60;
            TWO:
            led = 8'hDA;
            THREE:
            led = 8'hF2;
            FOUR:
            led = 8'h66;
            FIVE:
            led = 8'hB6;
            SIX:
            led = 8'hBE;
            SEVEN:
            led = 8'hE0;
            EIGHT:
            led = 8'hFE;
            NINE:
            led = 8'hF6;
            DECIMAL_POINT:
            led = 8'h01;
            BLANK:
            led = 8'h00;
            MINUS:
            led = 8'h02;
            d:
            led = 8'b0111_1010;
            l:
            led = 8'b0001_1100;
            default:
            led = 8'h00;
        endcase  


endmodule
