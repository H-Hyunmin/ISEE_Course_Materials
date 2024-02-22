`include "decrement.v"
`include "increment.v"
`include "mux21.v"
`include "Register.v"
`include "rom_16x8.v"
`include "tri_buffer.v"
module dp (
    input clk,rst,
    input IRload,PCload,
    input JNZmux,INmux,
    input Aload,
    input OutE,
    input [7:0] Input,
    output [2:0]IR75,
    output Anot0,
    output [7:0] Output
);
    wire[7:0] dp_ROMQ,dp_IR;
    wire[3:0] dp_increment,dp_PC,dp_JNZmux;
    wire[7:0] dp_INmux,dp_A,dp_decrement;
    Register #(8) regIR (.clk(clk),.rst(rst),.load(IRload),.D(dp_ROMQ),.Q(dp_IR));
    Register #(4) regPC (.clk(clk),.rst(rst),.load(PCload),.D(dp_JNZmux),.Q(dp_PC));
    Register #(8) regA (.clk(clk),.rst(rst),.load(Aload),.D(dp_INmux),.Q(dp_A));
    mux21 #(8) muxIN (.A(Input),.B(dp_decrement),.S(INmux),.Y(dp_INmux));
    mux21 #(4) muxJNZ (.A(dp_IR[3:0]),.B(dp_increment),.S(JNZmux),.Y(dp_JNZmux));
    increment inc (.A(dp_PC),.F(dp_increment));
    decrement dec (.A(dp_A),.F(dp_decrement));
    rom rom (.E(1'b1),.addr(dp_PC),.data(dp_ROMQ));
    tri_buffer #(8) buffer (.E(OutE),.D(dp_A),.Y(Output));
    assign IR75 = dp_IR[7:5];
    assign Anot0 = (dp_A!=0)? 1'b1 : 1'b0;

endmodule