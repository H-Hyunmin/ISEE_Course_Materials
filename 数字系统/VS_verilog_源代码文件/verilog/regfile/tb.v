//~ `New testbench
`include "regfile.v"
`timescale  1ns / 1ps

module tb_regfile;

// regfile Parameters
parameter PERIOD  = 10;


// regfile Inputs
reg   [7:0]  D                             = 0 ;
reg   [1:0]  WA                            = 0 ;
reg   WE                                   = 0 ;
reg   clk                                  = 0 ;
reg   [1:0]  RAA                           = 0 ;
reg   [1:0]  RBA                           = 0 ;
reg   RAE                                  = 0 ;
reg   RBE                                  = 0 ;

// regfile Outputs
wire  [7:0]  portA                         ;
wire  [7:0]  portB                         ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



regfile  u_regfile (
    .D                       ( D      [7:0] ),
    .WA                      ( WA     [1:0] ),
    .WE                      ( WE           ),
    .clk                     ( clk          ),
    .RAA                     ( RAA    [1:0] ),
    .RBA                     ( RBA    [1:0] ),
    .RAE                     ( RAE          ),
    .RBE                     ( RBE          ),

    .portA                   ( portA  [7:0] ),
    .portB                   ( portB  [7:0] )
);

initial
begin
    $dumpfile("tb_regfile.vcd");
    $dumpvars(0, tb_regfile);
    D = 23 ;
    RAE=1;
    RBE=1;
    RAA=0;
    RBA=0;
    WE=1;
    WA=3;
    #10
    D = 45 ;
    WA=1;
    RAA=3;
    RBA=1;
    #10
    D = 67 ;
    WA=2;
    #10
    D = 89 ;
    WA=0;
    #10


    $finish;
end

endmodule