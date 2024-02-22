//~ `New testbench
`include "Segment.v"
`timescale  1ns / 1ps

module tb_hex8_2;

// hex8_2 Parameters
parameter PERIOD  = 10;


// hex8_2 Inputs
reg   Clk                                  = 0 ;
reg   Reset_N                              = 0 ;
reg   [31:0]  Disp_Data                    = 0 ;

// hex8_2 Outputs
wire  [7:0]  Sel                           ;
wire  [7:0]  DisPlay                       ;
wire [7:0]  DisPlay1                      ;

initial
begin
    forever #(PERIOD/2)  Clk=~Clk;
end


hex8_2  u_hex8_2 (
    .Clk                     ( Clk               ),
    .Reset_N                 ( Reset_N           ),
    .Disp_Data               ( Disp_Data  [31:0] ),

    .Sel                     ( Sel        [7:0]  ),
    .DisPlay                 ( DisPlay    [7:0]  ),
    .DisPlay1               ( DisPlay1   [7:0]  )
);

initial
begin
    $dumpfile("tb_hex8_2.vcd");
    $dumpvars(0, tb_hex8_2);
    Reset_N = 0;
    #5
    Reset_N=1;
    Disp_Data=32'h12345678;
    #1000000
    $finish;
end

endmodule