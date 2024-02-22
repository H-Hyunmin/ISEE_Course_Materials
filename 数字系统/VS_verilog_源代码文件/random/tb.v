//~ `New testbench
`include "random.v"
`timescale  1ns / 1ps

module tb_random;

// random Parameters
parameter PERIOD  = 10;


// random Inputs
reg   clk                                  = 0 ;
reg   rst_n                                = 0 ;
reg   [31:0]  seed                         = 0 ;
// random Outputs
wire  [31:0]  random                         ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end


random  u_random (
    .clk                     ( clk            ),
    .rst_n                   ( rst_n          ),
    .seed                    ( seed    [31:0] ),

    .random                    ( random    [31:0] )
);

initial
begin
    $dumpfile("tb_random.vcd");
    $dumpvars(0, tb_random);
    rst_n=0;
    seed=$random;
    #10
    rst_n=1;
    #10
    #1000000
    $finish;
end

endmodule