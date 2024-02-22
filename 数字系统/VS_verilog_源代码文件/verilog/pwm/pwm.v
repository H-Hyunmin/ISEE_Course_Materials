`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/05/02 19:47:56
// Design Name: 
// Module Name: pwm
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module PWM(
    input clk,
    input rst_n,
    
    input pwm_en,
    input [15:0] pwm_div,
    input [6:0] pwm_duty,
    
    output reg pwm
);

    reg [15:0] pwm_div_cnt;
    reg [6:0] pwm_duty_cnt;

    always @(posedge clk)begin
        if(~rst_n)begin
            pwm_div_cnt <= 16'd0;
        end
        else if(pwm_en)begin
            if(pwm_div_cnt == pwm_div-1)
                pwm_div_cnt <= 16'd0;
            else
                pwm_div_cnt <= pwm_div_cnt + 1'b1;
        end
        else begin
            pwm_div_cnt <= 16'd0;
        end
    end
    
    always @(posedge clk)begin
        if(~rst_n)begin
            pwm_duty_cnt <= 7'd0;
        end
        else if(pwm_en)begin
            if(pwm_div_cnt == pwm_div-1)begin
                if(pwm_duty_cnt == 7'd99)
                    pwm_duty_cnt <=7'd0;
                else
                    pwm_duty_cnt <= pwm_duty_cnt + 1'b1;
            end
        end
        else begin
            pwm_duty_cnt <= 16'd0;
        end
    end
    
    always @(posedge clk)begin
        if(~rst_n)begin
            pwm <= 1'd0;
        end
        
        else if(pwm_en)begin
                pwm <= (pwm_duty > pwm_duty_cnt) ? 1:0;
        end
        else begin
                pwm <=1'd0;
        end
     end
     
endmodule
