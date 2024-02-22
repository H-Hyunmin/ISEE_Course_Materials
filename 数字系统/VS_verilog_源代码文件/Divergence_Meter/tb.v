//~ `New testbench
`include "Divergence_Meter.v"
`timescale  1ns / 1ps

module tb_Divergence_Meter;

// Divergence_Meter Parameters
parameter PERIOD  = 10;


// Divergence_Meter Inputs
reg   clk                                  = 0 ;
reg   rst                                  = 0 ;
reg   botton                               = 0 ;

// Divergence_Meter Outputs
wire  [7:0]  Sel                           ;
wire  [7:0]  DisPlay                       ;
wire  [7:0]  DisPlay1                      ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end


Divergence_Meter  u_Divergence_Meter (
    .clk                     ( clk             ),
    .rst                     ( rst             ),
    .botton                  ( botton          ),

    .Sel                     ( Sel       [7:0] ),
    .DisPlay                 ( DisPlay   [7:0] ),
    .DisPlay1                ( DisPlay1  [7:0] )
);

initial
begin
    $dumpfile("tb_Divergence_Meter.vcd");
    $dumpvars(0, tb_Divergence_Meter);
    rst=0;
    botton=0;
    #10
    rst=1;
    #10
    botton=1;
    #30
    botton=0;
    #1000000
    $finish;
end

endmodule