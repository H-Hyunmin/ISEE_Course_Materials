module botton(
		input 	wire		clk		,		//50MHz时钟频率
		input 	wire		rst_n	,		//复位按键
		input 	wire		key		,		//按键输入口
		output 	reg 		press			//按键按下标志输出
	);
	
	//全局参数定义
	parameter	DELAY_TIME = 50;	//延时20ms也就是1000_000个时钟周期
	
	//信号定义
	reg				key_r0		;		//同步 当前时钟周期输入状态
	reg				key_r1		;		//打拍 前一个时钟周期输入的状态
	wire			key_nedge	;		//下降沿
	
	reg		[19:0]	delay_cnt	;		//计数20ms，需要20ms/20ns = 1000_000个时钟周期
	reg				delay_flag	;		//按下的下降沿标志
	initial begin
		delay_cnt=0;
	end
	//同步计数实现
	always @(posedge clk or negedge rst_n) begin
		if(rst_n == 1'b0) begin
			key_r0 <= 1'b1;
			key_r1 <= 1'b1;
		end
		else begin
			key_r0 <= key;
			key_r1 <= key_r0;
		end
	end
	
	assign key_nedge = key_r0 & ~key_r1;	//检测下降沿
	//assign key_pedge = key_r0 & ~key_r1;	//检测上升沿
	
	//delay_cnt 计数器计满1000_000个时钟周期
	always @(posedge clk or negedge rst_n) begin
		if(!rst_n) begin
			delay_cnt <= 0;
		end
		else if(delay_flag) begin
			//按下的下降沿标志出现，则执行
			if(delay_cnt == DELAY_TIME - 1) begin
				//如果计满1000_000个时钟周期，则归零处理
				delay_cnt <= 0;
			end
			else begin
				//没有计满，则不断累加
				delay_cnt <= delay_cnt + 1;
			end
		end
	end
	
	//delay_flag
	always @(posedge clk or negedge rst_n) begin
		if(!rst_n) begin
			delay_flag <= 1'b0;
		end
		else if(key_nedge) begin
			//下降沿出现后，设置按下标志位为 1
			delay_flag <= 1'b1;
		end
		else if(delay_cnt == DELAY_TIME - 1) begin
			/*
			计数器计满1000_000个时钟周期后，
			设置按下标志位为 0
			表示按键按下结束，以备下次按键
			*/
			delay_flag <= 1'b0;
		end
	end
	
	//press
	always @(posedge clk or negedge rst_n) begin
		if(!rst_n) begin
			press <= 1'b0;
		end
		else if(delay_cnt == DELAY_TIME - 1) begin
			/*
			计满1000_000个时钟周期后
			获取当前的按压标志
			此时 press = 1
			*/
			press <= key_r0;
			// 上升沿时press<=key_r0;
		end
		else begin
			/*
			没有计满1000_000个时钟周期
			则 press = 0
			*/
			press <= 1'b0;
		end
	end

endmodule
	
