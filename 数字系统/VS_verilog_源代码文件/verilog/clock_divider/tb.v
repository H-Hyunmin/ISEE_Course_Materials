//~ `New testbench
`include "clock_divider.v"
`timescale  1ns / 1ps

module tb_clock_divider;

// clock_divider Parameters
parameter PERIOD  = 2;


// clock_divider Inputs
reg   clk_in                               = 0 ;
reg reset                                  = 0 ;

// clock_divider Outputs
wire  clk                                    ;


initial
begin
    forever #(PERIOD/2)  clk_in=~clk_in;
end

 
clock_divider  u_clock_divider (
    .clk_in                  ( clk_in   ),
    .reset                   ( reset    ),
    .clk                     ( clk      )
);

initial
begin
    $dumpfile("tb_clock_divider.vcd");
    $dumpvars(0, tb_clock_divider);
    clk_in = 0;
    #10
    reset=1;
    #10
    reset=0;
    #1000

    $finish;
end

endmodule