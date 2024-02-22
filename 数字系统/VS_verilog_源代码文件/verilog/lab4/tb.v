`timescale  1ns / 1ps
`include "top.v"


module tb_top;

// top Parameters
parameter PERIOD  = 10;


// top Inputs
reg   clk                                  = 0 ;
reg   rst                                  = 0 ;
reg   key0                                 = 0 ;
reg   key1                                 = 0 ;


// top Outputs
wire  [0:0]  unlock                        ;
wire  [7:0]Sel ;
wire   [7:0]DisPlay;
wire   [7:0]DisPlay1;

initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



top  u_top (
    .clk                     ( clk                  ),
    .rst                     ( rst                  ),
    .key0                    ( key0                 ),
    .key1                    ( key1                 ),

 
    .unlock                  ( unlock         [0:0] ),
         .Sel(Sel [7:0]),
         .DisPlay(DisPlay[7:0]),
         .DisPlay1(DisPlay1[7:0])
);

initial
begin
    $dumpfile("tb_top.vcd");
    $dumpvars(0, tb_top);
    key0=0;
    key1=0;
    rst=0;
    #5
    rst=1;
    #10
    key1=1;
    #600
    key1=0;
    #100000
    key0=1;
    #600
    key0=0;
    #100000
    key0=1;
    #600
    key0=0;
    #100000
    key1=1;
    #600
    key1=0;
    #100000
    key0=1;
    #600
    key0=0;
    #100000
    $finish;
end

endmodule