//~ `New testbench
`include "Segment_lab4.v"
`timescale  1ns / 1ps

module tb_hex8_2;

// hex8_2 Parameters
parameter PERIOD  = 10;


// hex8_2 Inputs
reg   Clk                                  = 0 ;
reg   Reset_N                              = 0 ;
reg   [31:0]  Disp_Data                    = 0 ;
reg   [2:0]  state                         = 0 ;

// hex8_2 Outputs
wire  [7:0]  Sel                           ;
wire  [7:0]  DisPlay                       ;
wire  [7:0]  DisPlay1                      ;


initial
begin
    forever #(PERIOD/2)  Clk=~Clk;
end


hex8_2  u_hex8_2 (
    .Clk                     ( Clk               ),
    .Reset_N                 ( Reset_N           ),
    .Disp_Data               ( Disp_Data  [31:0] ),
    .state                   ( state      [2:0]  ),

    .Sel                     ( Sel        [7:0]  ),
    .DisPlay                 ( DisPlay    [7:0]  ),
    .DisPlay1                ( DisPlay1   [7:0]  )
);

initial
begin
    $dumpfile("tb_hex8_2.vcd");
    $dumpvars(0, tb_hex8_2);
    Reset_N = 0;
    #5
    Reset_N=1;
    #10
    state=0;
    Disp_Data=32'h0000000;
    #1000000
    state=1;
    Disp_Data=32'h0000001;
    #1000000
    state=2;
    Disp_Data=32'h0000010;
    #1000000
    $finish;
end

endmodule