`timescale  1ns / 1ps
`include "key_led.v"

module tb_key_led;

// key_led Parameters
parameter PERIOD  = 20;


// key_led Inputs
reg   clk                                  = 0 ;
reg   rst_n                                = 0 ;
reg   key_in                               = 0 ;

// key_led Outputs
wire  [3:0]  led                           ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end

key_led  u_key_led (
    .clk                     ( clk           ),
    .rst_n                   ( rst_n         ),
    .key_in                  ( key_in        ),

    .led                     ( led     [3:0] )
);


initial
begin
    $dumpfile("tb_key_led.vcd");
    $dumpvars(0, tb_key_led);
    rst_n = 0;
    #10
    rst_n=1;
    key_in=0;
    #1000
    key_in=1;
    #10
    key_in=0;
    #5
    key_in=1;
    #7
    key_in=0;
        #5
    key_in=1;
    #7
    key_in=0;
        #5
    key_in=1;
    #7
    key_in=0;
    #10
    key_in=1;
    #50000;
    key_in=0;
    #100000;
    $finish;
end

endmodule