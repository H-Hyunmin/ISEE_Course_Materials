`timescale  1ns / 1ps
`include "top.v"
module tb_top;

// top Parameters
parameter PERIOD  = 10;


// top Inputs
reg   Clock                                = 0 ;
reg   Reset                                = 0 ;
reg   [7:0]  N                             = 0 ;

// top Outputs
wire  Data                                 ;


initial
begin
    forever #(PERIOD/2)  Clock=~Clock;
end



top  u_top (
    .Clock                   ( Clock        ),
    .Reset                   ( Reset        ),
    .N                       ( N      [7:0] ),

    .Data                    ( Data         )
);

initial
begin
    $dumpfile("tb_top.vcd");
    $dumpvars(0,tb_top);
    N=8'b10101100;
    Reset = 0;
    #10;
    Reset = 1;
    #10;
    
    #10000;
    $finish;
end

endmodule