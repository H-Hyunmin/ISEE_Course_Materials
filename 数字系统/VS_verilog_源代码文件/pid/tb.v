//~ `New testbench
`timescale  1ns / 1ps
`include "control.v"

module tb_control;

// control Parameters
parameter PERIOD  = 10;


// control Inputs
reg   clk                                  = 0 ;
reg   [15:0]  kp                           = 0 ;
reg   [15:0]  kd                           = 0 ;
reg   [15:0]  zhongzhi                     = 0 ;
reg   [15:0]  positon_target               = 0 ;
reg   [15:0]  posi_kp                      = 0 ;
reg   [15:0]  posi_kd                      = 0 ;
reg   [15:0]  posi_Sensor                  = 0 ;
reg   [15:0]  Sensor                       = 0 ;

// control Outputs
wire  [15:0]  Motor                        ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



control  u_control (
    .clk                     ( clk                    ),
    .kp                      ( kp              [15:0] ),
    .kd                      ( kd              [15:0] ),
    .zhongzhi                ( zhongzhi        [15:0] ),
    .positon_target          ( positon_target  [15:0] ),
    .posi_kp                 ( posi_kp         [15:0] ),
    .posi_kd                 ( posi_kd         [15:0] ),
    .posi_Sensor             ( posi_Sensor     [15:0] ),
    .Sensor                  ( Sensor          [15:0] ),

    .Motor                   ( Motor           [15:0] )
);

initial
begin
    $dumpfile("control.vcd");
    $dumpvars(0,tb_control);
    
    kp=1;
    kd=1;
    posi_kp=1;
    posi_kd=1;
    zhongzhi=1000;
    positon_target=2000;
    Sensor=700;
    posi_Sensor=1500;
    #100
    Sensor=800;
    posi_Sensor=1700;
    $finish;
end

endmodule