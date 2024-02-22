`include "Segment.v"
`include "botton.v"
`include "random.v"

module Divergence_Meter (
    input clk,
    input rst,
    input botton,
    output  [7:0]Sel,
    output  [7:0]DisPlay,
    output  [7:0]DisPlay1
);
    wire [31:0]seed=32'h12345678;
    wire [31:0]random;
    wire botton_flag;
    botton u_botton(
        .clk(clk),
        .rst_n(rst),
        .key(botton),
        .delay_flag(botton_flag)
    );
    random u_random(
        .clk(clk),
        .rst_n(rst),
        .seed(seed),
        .random(random)
    );
             Segment u_Segment(
        .Clk(clk),
        .Reset_N(rst),
        .Disp_Data(random),
        .enable(botton_flag),
        .Sel(Sel),
        .DisPlay(DisPlay),
        .DisPlay1(DisPlay1)
             );


endmodule