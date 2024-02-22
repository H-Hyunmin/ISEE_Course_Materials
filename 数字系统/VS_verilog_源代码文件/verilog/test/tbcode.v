//~ `New testbench




`timescale 1ns/1ps
`include "testcode.v"






module tb_half_adder;

// half_adder Parameters
parameter PERIOD  = 10;


// half_adder Inputs
reg   a                                    = 0 ;
reg   b                                    = 0 ;

// half_adder Outputs
wire  sum                                  ;
wire  cout                                 ;




half_adder  u_half_adder (
    .a                       ( a      ),
    .b                       ( b      ),

    .sum                     ( sum    ),
    .cout                    ( cout   )
);
parameter clk_period = 10;  
reg clk;  
initial  
    clk = 0;  
always #(clk_period/2) clk = ~clk;  
always @(negedge clk) begin
    a=$random%2;
    b=$random%2;
end

initial
begin
$dumpfile("testcode.vcd");
$dumpvars(0,tb_half_adder);
    #10000
    $finish;
end



endmodule