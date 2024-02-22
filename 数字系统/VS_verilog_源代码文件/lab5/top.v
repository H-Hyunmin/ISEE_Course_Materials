`include "clk_division.v"
`include "segment.v"
module top 
(
    input clk_sys,
    input mode,
    input reset,
    output [7:0] led,
    output [3:0] segena
);
    reg [63:0]bcd=64'hBBB2_017B_04B2_7DDE;//字符编码
    reg [15:0] seg_selection;//段选
    reg mode1;//模式选择
    wire ena = 1'b1;
    wire clk;   //0.5Hz时钟
    wire clk1; // 20ms时钟

// 时钟分频
    clk_division #(.F_CLK_SYS( 200_000_000 ) , .F_CLK_DIVISION( 1 ) ) clk_division_inst (
        .ena( 1'b1 ),
        .clk_in( clk_sys ),
        .clk_out( clk )
    );
// 时钟分频
        clk_division #(.F_CLK_SYS( 100_000_000 ) , .F_CLK_DIVISION( 500 ) ) clk_division_inst1 (
        .ena( 1'b1 ),
        .clk_in( clk_sys ),
        .clk_out( clk1 )
    );

    always @(posedge clk or negedge reset) begin
        if (~reset) begin //复位
            bcd<=64'hBBB2_017B_04B2_7DDE;
            seg_selection <= 16'hBBBB;
        end
        else begin//单位显示
            if (~mode) begin
                if(mode1) begin
                    bcd <= 64'hBBB2_017B_04B2_7DDE;
                    mode1<=mode;
                    end
                else begin
             seg_selection <= bcd[63:48]; 
            bcd <= {bcd[63:52],bcd[47:0],bcd[51:48]};
            mode1<=mode;
                end
        end
        else begin//四位显示
            if(~mode1) begin
                    bcd <= 64'hBBB2_017B_04B2_7DDE;
                    mode1<=mode;
                    end
                else begin
            seg_selection <= bcd[63:48]; 
            bcd <= {bcd[59:0],bcd[63:60]};
            mode1<=mode;
                end
        end
        end
           end
// 数码管模块，输入时钟和seg_selection，输出数码管段选和位选信号
           segment segment_inst (
        .clk_sys(clk_sys),
        .clk(clk1),
        .reset(reset),
        .Disp_Data(seg_selection),
        .Sel(segena),
        .led(led)
           );
endmodule