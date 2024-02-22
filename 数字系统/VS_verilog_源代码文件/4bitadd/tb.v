//~ `New testbench
`include "adder.v"
`timescale  1ns / 1ps

module tb_adder;

// adder Parameters
parameter PERIOD  = 10;


// adder Inputs
reg   [3:0]  a                             = 0 ;
reg   [3:0]  b                             = 0 ;

// adder Outputs
wire  [3:0]  sum                           ;
wire  cout                                 ;




adder  u_adder (
    .a                       ( a     [3:0] ),
    .b                       ( b     [3:0] ),

    .sum                     ( sum   [3:0] ),
    .cout                    ( cout        )
);

initial
begin 
$dumpfile("4bitadder.vcd");
$dumpvars;
a=5;
b=2;
#20;
a=3;
b=9;
#20;
a={$random}%10;
b={$random}%10;;
#20;
    $finish;
end

endmodule