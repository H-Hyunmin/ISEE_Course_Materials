//~ `New testbench
`include "alu.v"
`timescale  1ns / 1ps

module tb_alu;

// alu Parameters
parameter PERIOD = 10;
parameter n  = 4;

// alu Inputs
reg   [n-1:0]  a                           = 0 ;
reg   [n-1:0]  b                           = 0 ;
reg   [2:0]  s                             = 0 ;

// alu Outputs
wire  [n-1:0]  F                           ;




alu #(
    .n ( n ))
 u_alu (
    .a                       ( a  [n-1:0] ),
    .b                       ( b  [n-1:0] ),
    .s                       ( s  [2:0]   ),

    .F                       ( F  [n-1:0] )
);

initial
begin
$dumpfile("alu.vcd");
$dumpvars;
a=5;
b=2;
s=3'b010;
#20;
a=3;
b=9;
s=3'b011;
#20;
a=1;
b=1;
s=3'b000;
#20;
a=1;
b=8;
s=3'b001;
#20;
a=3;
b=2;
s=3'b100;
#20;
a=3;
b=6;
s=3'b101;

    $finish;
end

endmodule