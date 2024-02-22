//~ `New testbench
`include "rom.v"
`timescale  1ns / 1ps

module tb_rom;

// rom Parameters
parameter PERIOD  = 10;
parameter size = 4;

// rom Inputs
reg   E                                    = 0 ;
reg   [size-1:0]  addr                     = 0 ;

// rom Outputs
wire  [7:0]  data                          ;




rom  u_rom (
    .E                       ( E                ),
    .addr                    ( addr  [size-1:0] ),

    .data                    ( data  [7:0]      )
);

initial
begin
    $dumpfile("tb_rom.vcd");
    $dumpvars(0, tb_rom);
    E = 1 ;
    addr = 0 ;
    #10
    addr = 1 ;
    #10
    addr = 2 ;
    #10
    addr = 3 ;
    #10
    addr = 4 ;
    #10
    E=0;
    addr = 0 ;
    #10
    $finish;
end

endmodule