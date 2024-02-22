//~ `New testbench
`include "summation_forloop.v"
`timescale  1ns / 1ps

module tb_summation_forloop;

// summation_forloop Parameters
parameter PERIOD  = 10;


// summation_forloop Inputs
reg   reset                                = 0 ;

// summation_forloop Outputs
wire  [7:0]  out                           ;





summation_forloop  u_summation_forloop (
    .reset                   ( reset        ),

    .out                     ( out    [7:0] )
);

initial
begin
    $dumpfile("tb_summation_forloop.vcd");
    $dumpvars(0, tb_summation_forloop);
    reset=0;
    #10
    reset=1;
    #1000
    $finish;
end

endmodule