//~ `New testbench
`include "full.v"

`timescale  1ns / 1ps

module tb_full_adder;

// full_adder Parameters
parameter PERIOD  = 10;


// full_adder Inputs
reg   a                                    = 0 ;
reg   b                                    = 0 ;
reg   cout                                 = 0 ;

// full_adder Outputs
wire  sum                                  ;
wire  cin                                  ;




full_adder  u_full_adder (
    .a                       ( a      ),
    .b                       ( b      ),
    .cout                    ( cout   ),

    .sum                     ( sum    ),
    .cin                     ( cin    )
);

parameter clk_period = 10;  
reg clk;  
initial  
    clk = 0;  
always #(clk_period/2) clk = ~clk;  
always @(negedge clk) begin
    a=$random%2;
    b=$random%2;
    cout=$random%2;
end


initial
begin
    $dumpfile("full.vcd");
    $dumpvars(0,tb_full_adder);
    #10000
    $finish;
end

endmodule