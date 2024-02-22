`timescale 1ns/1ns
module hex8_2(
    Clk,
    Reset_N,
    Disp_Data,
    state,
    Sel,
    DisPlay,
    DisPlay1
    );
    input Clk;
    input Reset_N;
    input [31:0]Disp_Data;
    input [2:0]state; 
    output reg [7:0]Sel;
    output reg [7:0]DisPlay;
    output reg [7:0]DisPlay1;
    
    reg clk_1k;
    reg [15:0]div_cnt;


    always@(posedge Clk or negedge Reset_N)
    if(!Reset_N)
        div_cnt <= 0;
    else if(div_cnt >= 4999)
        div_cnt <= 0;
    else
        div_cnt <= div_cnt + 1'd1;
    
    // clk_1k采用使能时钟，避免使用门控时钟
    always@(posedge Clk or negedge Reset_N)
    if(!Reset_N)
        clk_1k <= 0;
    else if(div_cnt == 4999)
        clk_1k <= 1'b1;
    else
        clk_1k <= 0;
    
    
    reg [2:0]Num;
    always@(posedge Clk or negedge Reset_N)
    if(!Reset_N)
        Num <= 0;
    else if(clk_1k) begin
        if (state!=0) Num <= (Num==state-1)?  0 :Num + 1'd1;
        else Num <= 0;
    end
    
       
    
    // 3_8译码器
    always@(posedge Clk)
    case (state)
    3'b000:begin
            Sel = 8'b0000_0000;
    end
    3'b001:begin
              case(Num)
            3'b000: Sel = 8'b0000_0001;
        endcase
    end
    3'b010:begin
              case(Num)
            3'b000: Sel = 8'b0000_0001;
            3'b001: Sel = 8'b0000_0010;
        endcase
    end
    3'b011:begin
              case(Num)
            3'b000: Sel = 8'b0000_0001;
            3'b001: Sel = 8'b0000_0010;
            3'b010: Sel = 8'b0000_0100;
        endcase
    end
    3'b100:begin
              case(Num)
            3'b000: Sel = 8'b0000_0001;
            3'b001: Sel = 8'b0000_0010;
            3'b010: Sel = 8'b0000_0100;
            3'b011: Sel = 8'b0000_1000;
        endcase
    end
    3'b101:begin
              case(Num)
            3'b000: Sel = 8'b0000_0001;
            3'b001: Sel = 8'b0000_0010;
            3'b010: Sel = 8'b0000_0100;
            3'b011: Sel = 8'b0000_1000;
            3'b100: Sel = 8'b0001_0000;
    endcase
    end
    endcase

    


            //       case(Num)
            // 3'b000: Sel = 8'b0000_0001;
            // 3'b001: Sel = 8'b0000_0010;
            // 3'b010: Sel = 8'b0000_0100;
            // 3'b011: Sel = 8'b0000_1000;
            // 3'b100: Sel = 8'b0001_0000;
            // 3'b101: Sel = 8'b0010_0000;
            // 3'b110: Sel = 8'b0100_0000;
            // 3'b111: Sel = 8'b1000_0000;



    // 8选1多路器
    reg [3:0]Disp_tmp;
    reg [3:0]Disp_tmp1;
    always@(posedge Clk)
        case(Num)
            3'b000: Disp_tmp = Disp_Data[3:0];
            3'b001: Disp_tmp = Disp_Data[7:4];
            3'b010: Disp_tmp = Disp_Data[11:8];
            3'b011: Disp_tmp = Disp_Data[15:12];
            3'b100: Disp_tmp1 = Disp_Data[19:16];
            3'b101: Disp_tmp1 = Disp_Data[23:20];
            3'b110: Disp_tmp1 = Disp_Data[27:24];
            3'b111: Disp_tmp1 = Disp_Data[31:28];
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
            14: DisPlay = ~8'h86;
            15: DisPlay = ~8'h8e;
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
            14: DisPlay1 = ~8'h86;
            15: DisPlay1 = ~8'h8e;
        endcase  

endmodule
