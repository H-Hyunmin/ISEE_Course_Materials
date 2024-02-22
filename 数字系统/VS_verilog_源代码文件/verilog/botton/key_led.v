`include "key_debounce.v"
`include "key_driver.v"

module key_led(
	input						clk			,		//系统时钟
	input 						rst_n		,		//复位 低电平有效
	
	input		wire			key_in		,		//顶层模块定义wire型
	output 		wire	[3:0]	led				//输出led信号
	);

	wire 		press		;						//定义一条线，连接按键按下标志信号

	//模块例化
	key_debounce u_key_debounce(
		.clk			(clk	),
		.rst_n			(rst_n	),
		.key			(key_in	),
		.press			(press	)			//按键按下标志
	);
	
	key_driver u_key_driver(
		.clk			(clk	),
		.rst_n			(rst_n	),
		.en				(press	),
		.led_o			(led	)
	);

endmodule

