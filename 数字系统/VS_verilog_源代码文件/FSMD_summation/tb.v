//~ `New testbench
`include "summation.v"
`timescale  1ns / 1ps

module tb_summation;

// summation Parameters
parameter PERIOD = 10;
parameter s0  = 0;

// summation Inputs
reg   clk                                  = 0 ;
reg   reset                                = 0 ;

// summation Outputs
wire  [7:0]  out                           ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



summation #(
    .s0 ( s0 ))
 u_summation (
    .clk                     ( clk          ),
    .reset                   ( reset        ),

    .out                     ( out    [7:0] )
);

initial
begin
    $dumpfile("tb_summation.vcd");
    $dumpvars(0, tb_summation);
    reset=0;
    #10
    reset=1;
    #1000
    $finish;
end

endmodule