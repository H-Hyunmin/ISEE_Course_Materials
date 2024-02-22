`timescale 1ns/1ns
module Segment(
    Clk,
    Reset_N,
    Disp_Data,
    enable,
    Sel,
    DisPlay,
    DisPlay1
    );
    input Clk;
    input Reset_N;
    input [31:0]Disp_Data;
    input enable;
    output reg [7:0]Sel;
    output reg [7:0]DisPlay;
    output reg [7:0]DisPlay1;
    
    reg clk_1k;
  
    reg [15:0]div_cnt;
    reg [31:0]Data;

    always@(posedge Clk or negedge Reset_N)
    if(!Reset_N)
        div_cnt <= 0;
    else if(div_cnt >= 49)
        div_cnt <= 0;
    else
        div_cnt <= div_cnt + 1'd1;
    
    // clk_1k采用使能时钟，避免使用门控时钟
    always@(posedge Clk or negedge Reset_N)
    if(!Reset_N)begin
          clk_1k <= 0;

    end
    
    else if(div_cnt == 49)begin
      clk_1k <= 1'b1;
  
    end
        
    else begin
           clk_1k <= 0;
 
    end
   
    
    reg [2:0]Num;
    reg [9:0]Num1;
    always@(posedge Clk or negedge Reset_N)
    if(!Reset_N)begin
      Num <= 0;
      Num1 <=0;
    end
        
    else if(clk_1k) 
    begin
      Num <= Num + 1'd1;
      Num1 <=Num1 + 1'd1;
    end
        
    
    // 3_8译码器
    always@(posedge Clk)begin
      if (~enable)     case(Num)
            3'b000: Sel = 8'b0000_0001;
            3'b001: Sel = 8'b0000_0010;
            3'b010: Sel = 8'b0000_0100;
            3'b011: Sel = 8'b0000_1000;
            3'b100: Sel = 8'b0001_0000;
            3'b101: Sel = 8'b0010_0000;
            3'b110: Sel = 8'b0100_0000;
            3'b111: Sel = 8'b1000_0000;
        endcase
    else case(Num1)
            10'd128: Sel = 8'b0000_0001;
            10'd256: Sel = 8'b0000_0010;
            10'd384: Sel = 8'b0000_0100;
            10'd512: Sel = 8'b0000_1000;
            10'd640: Sel = 8'b0001_0000;
            10'd768: Sel = 8'b0010_0000;
            10'd896: Sel = 8'b0100_0000;
            10'd1023: Sel = 8'b1000_0000;
        endcase
    end
         


    
    // 8选1多路器
    reg [3:0]Disp_tmp;
    reg [3:0]Disp_tmp1;
    always@(posedge Clk) begin
        if (enable) Data <= Disp_Data;
    else Data <= Data;
    end

    always@(posedge Clk)
        case(Num)
            3'b000: Disp_tmp = Data[3:0];
            3'b001: Disp_tmp = Data[7:4];
            3'b010: Disp_tmp = Data[11:8];
            3'b011: Disp_tmp = Data[15:12];
            3'b100: Disp_tmp1 = Data[19:16];
            3'b101: Disp_tmp1 = Data[23:20];
            3'b110: Disp_tmp1 = Data[27:24];
            3'b111: Disp_tmp1 = Data[31:28];
        endcase
    always@(posedge Clk)
        case(Disp_tmp)
            0:  DisPlay = ~8'hc0;
            1:  DisPlay = ~8'hf9;
            2:  DisPlay = ~8'ha4;
            3:  DisPlay = ~8'hb0;
            4:  DisPlay = ~8'h99;
            5:  DisPlay = ~8'h92;
            6:  DisPlay = ~8'h82;
            7:  DisPlay = ~8'hf8;
            8:  DisPlay = ~8'h80;
            9:  DisPlay = ~8'h90;
            10: DisPlay = ~8'h88;
            11: DisPlay = ~8'h83;
            12: DisPlay = ~8'hc6;
            13: DisPlay = ~8'ha1;
            14: DisPlay = ~8'h79;
            15: DisPlay = ~8'h40;
        endcase  
    always@(posedge Clk)
        case(Disp_tmp1)
            0:  DisPlay1 = ~8'hc0;
            1:  DisPlay1 = ~8'hf9;
            2:  DisPlay1 = ~8'ha4;
            3:  DisPlay1 = ~8'hb0;
            4:  DisPlay1 = ~8'h99;
            5:  DisPlay1 = ~8'h92;
            6:  DisPlay1 = ~8'h82;
            7:  DisPlay1 = ~8'hf8;
            8:  DisPlay1 = ~8'h80;
            9:  DisPlay1 = ~8'h90;
            10: DisPlay1 = ~8'h88;
            11: DisPlay1 = ~8'h83;
            12: DisPlay1 = ~8'hc6;
            13: DisPlay1 = ~8'ha1;
            14: DisPlay1 = ~8'h79;
            15: DisPlay1 = ~8'h40;
        endcase  
  


endmodule
