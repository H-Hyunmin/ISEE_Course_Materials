//~ `New testbench
`include "D_latches.v"
`timescale  1ns / 1ps

module tb_D_latches;

// D_latches Parameters
parameter PERIOD  = 10;


// D_latches Inputs
reg   E                                    = 0 ;
reg   D                                    = 0 ;

// D_latches Outputs
wire  Q                                    ;



D_latches  u_D_latches (
    .E                       ( E   ),
    .D                       ( D   ),

    .Q                       ( Q   )
);

initial
begin
    $dumpfile("tb_D_latches.vcd");
    $dumpvars(0, tb_D_latches);
    E = 0;
    D = 1;
    #10;
    E = 1;
    #10;
    E=0;
    #10;
    D = 0;
    #10;
    E = 1;
    #10;
    $finish;
end

endmodule