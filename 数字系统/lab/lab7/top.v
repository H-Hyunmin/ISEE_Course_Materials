`include "dp.v"
`include "cu.v"
module top (
    input clk,rst,
    input [7:0] A,
    output [7:0] Output,
    output Halt_out
);
    wire IRload,PCload,JNZmux,OutE,Aload,INmux;
    wire [2:0] IR75;
    wire Anot0;
    dp dp (
        .clk(clk),
        .rst(rst),
        .IRload(IRload),
        .PCload(PCload),
        .JNZmux(JNZmux),
        .OutE(OutE),
        .Aload(Aload),
        .INmux(INmux),
        .Input(A),
        .IR75(IR75),
        .Anot0(Anot0),
        .Output(Output));
    cu cu (
        .clk(clk),
        .rst(rst),
        .IR75(IR75),
        .Anot0(Anot0),
        .IRload(IRload),
        .PCload(PCload),
        .JNZmux(JNZmux),
        .Halt_out(Halt_out),
        .OutE(OutE),
        .Aload(Aload),
        .INmux(INmux));
endmodule