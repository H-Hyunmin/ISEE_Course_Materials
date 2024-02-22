//~ `New testbench
`include "top.v"
`timescale  1ns / 1ps
module tb_top;
// top Parameters
parameter PERIOD  = 10;
// top Inputs
reg   clk                                  = 0 ;
reg   rxd                                  = 0 ;
// top Outputs
wire  txd                                  ;
initial
begin
    forever #(PERIOD/2)  clk=~clk;
end
top  u_top (
    .clk                     ( clk   ),
    .rxd                     ( rxd   ),
    .txd                     ( txd   )
);
initial
begin
     $dumpfile("tb_top.vcd");
        $dumpvars(0,tb_top);
    rxd = 1;
    #100
    rxd = 0;
    #10
    rxd = 1;
    #10
    rxd = 0;
    #10
    rxd = 1;
    #10
    rxd = 0;
    #10
    rxd = 0;
    #10
    rxd = 1;
    #10
    rxd = 1;
    #10
    rxd = 0;
    #10
    rxd = 1;
    #200
    $finish;
end

endmodule