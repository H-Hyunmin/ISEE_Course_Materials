`include "Balance_Control.v"
`include "Position_Control.v"

module control (
    input clk,
    input [15:0]kp,
    input [15:0]kd,
    input [15:0]zhongzhi,
    input [15:0]positon_target,
    input [15:0]posi_kp,
    input [15:0]posi_kd,
    input [15:0]posi_Sensor,
    input [15:0]Sensor, //xadc角度输入
    output[15:0]Motor//pwm控制量输出
);
wire [15:0]Blance_pwm,Position_pwm;
Balance_Control u1(
    .clk(clk),
    .Sensor(Sensor),
    .zhongzhi(zhongzhi),
    .kp(kp),
    .kd(kd),
    .Blance_pwm(Blance_pwm)
);

Position_Control u2(
    .clk(clk),
    .posi_Sensor(posi_Sensor),
    .positon_target(positon_target),
    .posi_kp(posi_kp),
    .posi_kd(posi_kd),
    .Position_pwm(Position_pwm)
);

   assign Motor=Blance_pwm-Position_pwm;
endmodule