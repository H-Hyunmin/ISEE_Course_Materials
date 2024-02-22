`timescale  1ns / 1ps
`include "top.v"
module tb_top;

// top Parameters
parameter PERIOD  = 10;


// top Inputs
reg   clk_sys                              = 0 ;
reg   mode                                 = 0 ;
reg   reset                                = 0 ;

// top Outputs
wire  [7:0]  led                           ;
wire  [3:0]  segena                        ;


initial
begin
    forever #(PERIOD/2)  clk_sys=~clk_sys;
end


top  u_top (
    .clk_sys                 ( clk_sys        ),
    .mode                    ( mode           ),
    .reset                   ( reset          ),

    .led                     ( led      [7:0] ),
    .segena                  ( segena   [3:0] )
);

initial
begin
    $dumpfile("tb_top.vcd");
    $dumpvars(0,tb_top);
    reset = 0;
    mode=1;
    #10
    reset = 1;
    #1000000;
    $finish;
end

endmodule