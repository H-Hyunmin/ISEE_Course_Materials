module Position_Control (
    input clk,
    input [15:0]posi_Sensor,
    input [15:0]positon_target,
    input [15:0] posi_kp,
    input [15:0] posi_kd,
    output reg [15:0]Position_pwm
);
    reg [15:0]posi_least,posi_Bias=16'h0000,D_posi_Bias,Last_posi_Bias,rkp,rkd;
    always @(posi_Sensor or posi_kp or posi_kd) begin
        posi_least=posi_Sensor-positon_target;
        posi_Bias=posi_least;
        rkp=posi_kp;
        rkd=posi_kd;
    end
    always @(posedge clk ) begin
        D_posi_Bias<=posi_Bias-Last_posi_Bias;
        Last_posi_Bias<=posi_Bias;
    end
    always @(D_posi_Bias) begin
        Position_pwm=rkp*posi_Bias+rkd*D_posi_Bias;
    end

endmodule