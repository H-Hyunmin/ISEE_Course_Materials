module half_adder(
      input a,             //第一个加数a
      input b,             //第二个加数b
      output sum,          //和
      output cout          //位
);     
		
    assign sum=a ^ b;     //sum=a⊕b
    assign cout=a & b;	  //cout=ab

endmodule
