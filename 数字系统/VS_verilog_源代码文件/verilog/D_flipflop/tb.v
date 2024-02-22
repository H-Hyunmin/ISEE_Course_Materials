//~ `New testbench
`include "D_flipflop.v"
`timescale  1ns / 1ps

module tb_D_flipflop;

// D_flipflop Parameters
parameter n = 4;
parameter PERIOD  = 16;


// D_flipflop Inputs
reg   [n-1:0]  D                           = 0 ;
reg   clk                                  = 0 ;

// D_flipflop Outputs
wire  [n-1:0]  Q                           ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end

D_flipflop  u_D_flipflop (
    .D                       ( D    [n-1:0] ),
    .clk                     ( clk          ),

    .Q                       ( Q    [n-1:0] )
);

initial
begin
    $dumpfile("tb_D_flipflop.vcd");
    $dumpvars(0, tb_D_flipflop);
    D = 0;
    #10
    D=8;
    #10
    D=4;
    #10
    D=2;
    #10
    D=1;
    #10
    $finish;
end

endmodule