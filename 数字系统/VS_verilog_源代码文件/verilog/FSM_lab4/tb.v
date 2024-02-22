//~ `New testbench
`include "FSM.v"
`timescale  1ns / 1ps

module tb_FSM;

// FSM Parameters
parameter PERIOD   = 10;
parameter s_reset  = 0;
parameter s_1      = 1;
parameter s_10     = 2;
parameter s_100    = 3;
parameter s_1001   = 4;
parameter s_10010  = 5;

// FSM Inputs
reg   clk                                  = 0 ;
reg   rst                                  = 0 ;
reg   b0                                   = 0 ;
reg   b1                                   = 0 ;

// FSM Outputs
wire  unlock                               ;
wire  [2:0]  debugstate                    ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



FSM #(
    .s_reset ( s_reset ),
    .s_1     ( s_1     ),
    .s_10    ( s_10    ),
    .s_100   ( s_100   ),
    .s_1001  ( s_1001  ),
    .s_10010 ( s_10010 ))
 u_FSM (
    .clk                     ( clk               ),
    .rst                     ( rst               ),
    .b0                      ( b0                ),
    .b1                      ( b1                ),

    .unlock                  ( unlock            ),
    .debugstate              ( debugstate  [2:0] )
);

initial
begin
    $dumpfile("tb_FSM.vcd");
    $dumpvars(0,tb_FSM);
    b0 = 0;
    b1 = 0;
    rst = 0;
    #5
    rst = 1;
    #10
    b1=1;
    #10
    b1=0;
    b0=1;
    #10
    b0=1;
    b1=0;
    #10
    b0=0;
    b1=0;
    #30
    b0=0;
    b1=1;
    #10
    b0=1;
    b1=0;
    #10
    b0=0;
    b1=1;
    #10


    $finish;
end

endmodule