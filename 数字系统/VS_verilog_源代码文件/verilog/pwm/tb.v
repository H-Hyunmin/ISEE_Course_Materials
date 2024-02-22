//~ `New testbench
`include "pwm.v"
`timescale  1ns / 1ps

module tb_PWM;

// PWM Parameters
parameter PERIOD  = 10;


// PWM Inputs
reg   clk                                  = 0 ;
reg   rst_n                                = 0 ;
reg   pwm_en                               = 0 ;
reg   [15:0]  pwm_div                      = 0 ;
reg   [6:0]  pwm_duty                      = 0 ;

// PWM Outputs
wire  pwm                                  ;


initial
begin
    forever #(PERIOD/2)  clk=~clk;
end



PWM  u_PWM (
    .clk                     ( clk              ),
    .rst_n                   ( rst_n            ),
    .pwm_en                  ( pwm_en           ),
    .pwm_div                 ( pwm_div   [15:0] ),
    .pwm_duty                ( pwm_duty  [6:0]  ),

    .pwm                     ( pwm              )
);

initial
begin
    $dumpfile("tb_PWM.vcd");
    $dumpvars(0, tb_PWM);
    rst_n=0;
    #10 
    rst_n=1;
    pwm_en                               = 1 ;
     pwm_div                      = 1 ;
     pwm_duty                      = 10 ;
    #5500
    pwm_duty                      = 90 ;
    #9500
    pwm_duty                      = 70 ;
    #10000
    $finish;
end

endmodule