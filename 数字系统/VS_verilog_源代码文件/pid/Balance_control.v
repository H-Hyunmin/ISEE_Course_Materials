module Balance_Control (
    input clk,
    input [15:0]Sensor,
    input [15:0]zhongzhi,
    input [15:0] kp,
    input [15:0] kd,
    output reg signed  [15:0]Blance_pwm
);
    reg signed [15:0]Bias,D_Bias,Last_Bias,rkp,rkd;
    always @(Sensor or kp or kd) begin
        Bias=Sensor-zhongzhi;
        rkp=kp;
        rkd=kd;
    end
    always @(posedge clk ) begin
        D_Bias<=Bias-Last_Bias;
        Last_Bias<=Bias;
    end
    always @(D_Bias) begin
        Blance_pwm=rkp*Bias+rkd*D_Bias;
    end

endmodule