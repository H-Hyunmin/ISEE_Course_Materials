module pid_value (
	//system signals
	input		clk						, // 时钟信号
	input		rst_n					, // 复位信号，低电平有效
	input		signed [31:0]	  d_uk	    , // pid增量
	output	    reg signed [31:0]      uk0		  // pid输出值
);
 
reg signed [31:0] uk1 = 31'b0; // 上一时刻u(k-1)的值
always @ (d_uk) begin
	uk0 = uk1 + d_uk/1000; // 计算pid输出值
	uk1 = uk0;// 寄存上一时刻 u(k-1)的值    
end
endmodule