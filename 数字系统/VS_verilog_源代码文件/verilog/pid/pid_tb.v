//~ `New testbench
`include "demo_top.v"

`timescale  1ns / 1ps

module tb_demo_top;

// demo_top Parameters
parameter PERIOD  = 10;


// demo_top Inputs
reg   clk                                  = 0 ;
reg   rst_n                                = 0 ;
reg   [31:0]  target                        = 0 ;
reg   [31:0]  y                             = 0 ;
reg   [15:0]  kp                            = 0 ;
reg   [15:0]  ki                            = 0 ;
reg   [15:0]  kd                            = 0 ;

// demo_top Outputs
wire  [31:0]  uk0                          ;



demo_top  u_demo_top (
    .clk                     ( clk            ),
    .rst_n                   ( rst_n          ),
    .target                  ( target  [31:0]  ),
    .y                       ( y       [31:0]  ),
    .kp                      ( kp      [15:0]  ),
    .ki                      ( ki      [15:0]  ),
    .kd                      ( kd      [15:0]  ),
    .uk0                     ( uk0     [31:0] )
);

parameter clk_period = 10;  
initial  
    clk = 0;  
always #(clk_period/2) clk = ~clk;  
// always @(negedge clk) begin
//     y=y+uk0;
// end



initial
begin
    $dumpfile("demo_top.vcd");
    $dumpvars(0, tb_demo_top);
    clk   = 0;
    target = 10000000;
    y      = 0;
    kp     = 700;
    kd     = 0;
    ki     = 0;

    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;
    y=uk0;
    #10;


    
    $finish;
end

endmodule