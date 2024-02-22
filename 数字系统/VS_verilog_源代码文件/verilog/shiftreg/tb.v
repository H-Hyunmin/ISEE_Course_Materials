//~ `New testbench
`include "shiftreg.v"
`timescale  1ns / 1ps

module tb_shiftreg;

// shiftreg Parameters
parameter PERIOD  = 10;
parameter n = 8;


// shiftreg Inputs
reg   clk                                  = 0 ;
reg   shift                                = 0 ;
reg   serial_in                            = 0 ;

// shiftreg Outputs
wire  reg[n-1:0] Q                         ;



initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



shiftreg  u_shiftreg (
    .clk                     ( clk            ),
    .shift                   ( shift          ),
    .serial_in               ( serial_in      ),

    .Q            ( Q  [n-1:0]    )
);

initial
begin
    $dumpfile("tb_shiftreg.vcd");
    $dumpvars(0, tb_shiftreg);
    shift=0;
    #10
    shift=1;
    serial_in=1;
    #10
    serial_in=0;
    #10
    serial_in=1;
    #10
    serial_in=1;
    #5
    shift=0;
    #10
    serial_in=0;
    #10
    serial_in=0;
    #10
    $finish;
end

endmodule