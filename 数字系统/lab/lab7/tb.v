//~ `New testbench
`timescale  1ns / 1ps
`include "top.v"

module tb_top;

// top Parameters
parameter PERIOD  = 10;


// top Inputs
reg   clk                                  = 0 ;
reg   rst                                  = 0 ;
reg   [7:0]  A                             = 0 ;

// top Outputs
wire  [7:0]  Output                        ;
wire  Halt_out                             ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



top  u_top (
    .clk                     ( clk             ),
    .rst                     ( rst             ),
    .A                       ( A         [7:0] ),

    .Output                  ( Output    [7:0] ),
    .Halt_out                ( Halt_out        )
);

initial
begin
$dumpfile("tb_top.vcd");
$dumpvars(0,tb_top);
A=8'h42;
rst=1;
#1
rst=0;
#1
rst=1;
#10000
    $finish;
end

endmodule