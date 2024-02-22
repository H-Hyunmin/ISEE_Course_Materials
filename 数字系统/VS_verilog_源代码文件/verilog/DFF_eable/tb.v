//~ `New testbench
`include "DFF_eable.v"
`timescale  1ns / 1ps

module tb_DFF_eable;

// DFF_eable Parameters
parameter PERIOD = 10;
parameter n  = 4;

// DFF_eable Inputs
reg   clk                                  = 0 ;
reg   reset                                = 0 ;
reg   eable                                = 0 ;
reg   [n-1:0]  D                           = 0 ;

// DFF_eable Outputs
wire  [n-1:0]  Q                           ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end


DFF_eable #(
    .n ( n ))
 u_DFF_eable (
    .clk                     ( clk            ),
    .reset                   ( reset          ),
    .eable                   ( eable          ),
    .D                       ( D      [n-1:0] ),

    .Q                       ( Q      [n-1:0] )
);

initial
begin
    $dumpfile("tb_DFF_eable.vcd");
    $dumpvars(0, tb_DFF_eable);
    reset = 1;
    #10
    reset=0;
    eable=1;
    D=0;
    #6
    D=12;
    #6
    D=8;
    #6
    D=4;
    #6
    D=2;
    #6
    eable=0;
    #6
    D=1;
    #6
    D=9;
    #6
    eable=1;
    #6
    D=5;
    #6
    D=13;
    #6
    $finish;
end

endmodule