//~ `New testbench
`include "shift.v"
`timescale  1ns / 1ps

module tb_shift;

// shift Parameters
parameter PERIOD  = 10;
parameter n=8;

// shift Inputs
reg   [1:0]  s                             = 0 ;
reg   [n-1:0]  date                        = 0 ;

// shift Outputs
wire  [n-1:0]  outdate                     ;



shift  u_shift (
    .s                       ( s        [1:0]   ),
    .date                    ( date     [n-1:0] ),

    .outdate                 ( outdate  [n-1:0] )
);

initial
begin
    $dumpfile("tb_shift.vcd");
    $dumpvars(0, tb_shift);
    s = 0;
    date = 8'b10110010;
    #10;
    s = 1;
    #10;
    s = 2;
    #10;
    s = 3;
    #10;
    $finish;
end

endmodule