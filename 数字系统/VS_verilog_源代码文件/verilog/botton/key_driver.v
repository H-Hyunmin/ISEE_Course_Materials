module key_driver(
	
	input		wire				clk			,
	input		wire				rst_n		,
	input		wire				en			,
	
	output		reg		[3:0]		led_o		
	
	);
	
	always @(posedge clk or negedge rst_n) begin
		if(!rst_n) begin
			led_o <= 0;
		end
		else if(en) begin
			//按下的信号出现，则LED灯变化
			//将低一位移置高位并进行拼接
			led_o <= {~led_o[0], led_o[3:1]};
		end	
	end
	
endmodule
