//~ `New testbench
`timescale  1ns / 1ps
`include "decoder.v"

module tb_decoder;

// decoder Parameters
parameter PERIOD  = 10;


// decoder Inputs
reg   E                                    = 0 ;
reg   [2:0]  A                             = 0 ;

// decoder Outputs
wire  [7:0]  F                             ;




decoder  u_decoder (
    .E                       ( E        ),
    .A                       ( A  [2:0] ),

    .F                       ( F  [7:0] )
);

initial
begin
$dumpfile("decoder.vcd");
$dumpvars;
E=1;
A=3'b000;
#20;
E=1;
A=3'b001;
#20;
A=3'b010;
#20;
A=3'b011;
#20;
A=3'b100;
#20;
A=3'b101;
#20;
A=3'b110;
#20;
A=3'b111;
#20;
E=0;
A=3'b010;
#20;

    $finish;
end

endmodule