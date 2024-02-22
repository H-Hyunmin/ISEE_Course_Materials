//~ `New testbench
`include "Balance_Control.v"
`timescale  1ns / 1ps

module tb_Balance_Control;

// Balance_Control Parameters
parameter PERIOD  = 10;


// Balance_Control Inputs
reg   clk                                  = 0 ;
reg   [15:0]  Sensor                       = 0 ;
reg   [15:0]  zhongzhi                     = 0 ;
reg   [15:0]  kp                           = 0 ;
reg   [15:0]  kd                           = 0 ;

// Balance_Control Outputs
wire  [15:0]  Blance_pwm                   ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end


Balance_Control  u_Balance_Control (
    .clk                     ( clk                ),
    .Sensor                  ( Sensor      [15:0] ),
    .zhongzhi                ( zhongzhi    [15:0] ),
    .kp                      ( kp          [15:0] ),
    .kd                      ( kd          [15:0] ),

    .Blance_pwm              ( Blance_pwm  [15:0] )
);

initial
begin
    $dumpfile("tb_Balance_Control.vcd");
    $dumpvars(0,tb_Balance_Control);
    kp=1;
    kd=1;
    zhongzhi=1000;
    Sensor=100;
    #1000;
    $finish;
end

endmodule