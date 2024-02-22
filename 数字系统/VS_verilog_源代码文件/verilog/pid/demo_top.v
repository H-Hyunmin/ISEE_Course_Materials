`include "error.v"
`include "incre_value.v"
`include "pid_value.v"
module  demo_top(
	//system signals
	input		clk						, // 时钟信号
	input		rst_n					, // 复位信号，低电平有效
	input		signed [31:0]		target	, // 目标值
	input		signed [31:0]		   y	, // 实际输出值
	input		[15:0]		  kp	, // 比例系数
	input		[15:0]		  ki	, // 积分系数
	input		[15:0]		  kd	, // 微分系数
	output		signed [31:0]	      uk0	  // pid输出值
);
 
wire	signed [31:0] ek0; // e(k)
wire	signed [31:0] ek1; // e(k-1)
wire	signed [31:0] ek2; // e(k-2)
error error_demo(.clk(clk),
				 .rst_n(rst_n),
				 .target(target),
				 .y(y),
				 .ek0(ek0),
				 .ek1(ek1),
				 .ek2(ek2)
	            );
 
 
wire signed [31:0] d_uk;// pid增量
incre_value incre_value_demo(.ek0(ek0),
							 .ek1(ek1),
							 .ek2(ek2),
							 .kp(kp),
							 .ki(ki),
							 .kd(kd),
							 .d_uk(d_uk)
				             );
 
pid_value pid_value_demo(.clk(clk),
						 .rst_n(rst_n),
						 .d_uk(d_uk),
						 .uk0(uk0)
				        );
 
endmodule