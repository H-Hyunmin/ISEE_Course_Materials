//~ `New testbench
`include "three_buffer.v"
`timescale  1ns / 1ps

module tb_three_buffer;

// three_buffer Parameters
parameter PERIOD  = 10;
parameter n = 4;

// three_buffer Inputs
reg   E                                    = 0 ;
reg   [n-1:0]  A                           = 0 ;

// three_buffer Outputs
wire  [n-1:0]  Y                           ;




three_buffer  u_three_buffer (
    .E                       ( E          ),
    .A                       ( A  [n-1:0] ),

    .Y                       ( Y  [n-1:0] )
);

initial
begin
    $dumpfile("tb_three_buffer.vcd");
    $dumpvars(0, tb_three_buffer);
    E = 0;
    A = 0;
    #10;
    A = 12;
    #10;
    E = 1;
    #10;
    A = 10;
    #10;
    E = 0;
    #10;
    $finish;
end

endmodule