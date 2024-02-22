//~ `New testbench
`timescale  1ns / 1ps
`include "Position_Control.v"
module tb_Position_Control;

// Position_Control Parameters
parameter PERIOD  = 10;


// Position_Control Inputs

reg   clk                                  = 0 ;
reg   [15:0]  Sensor                       = 0 ;
reg   [15:0]  positon_target               = 0 ;
reg   [15:0]  posi_kp                      = 0 ;
reg   [15:0]  posi_kd                      = 0 ;

// Position_Control Outputs
wire  [15:0]  Position_pwm                 ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end

Position_Control  u_Position_Control (
    .clk                     ( clk                    ),
    .Sensor                  ( Sensor          [15:0] ),
    .positon_target          ( positon_target  [15:0] ),
    .posi_kp                 ( posi_kp         [15:0] ),
    .posi_kd                 ( posi_kd         [15:0] ),

    .Position_pwm            ( Position_pwm    [15:0] )
);

initial
begin
$dumpfile("tb_Position_Control.vcd");
$dumpvars(0,tb_Position_Control);
  posi_kp=1;
  posi_kd=1;
  positon_target=2000;
  Sensor=5000;
    #100;
  Sensor=3000;
    #100;
    Sensor=2500;
    #100;
    Sensor=2200;
    #100;
    Sensor=2100;
    #100;
    Sensor=2075;
    #100;
    $finish;
end

endmodule