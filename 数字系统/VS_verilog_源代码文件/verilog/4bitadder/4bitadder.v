module adder(
      input [3:0]a,b,            
      output [3:0]sum,         
      output cout         
);     
		
 assign {cout,sum}=a+b;
endmodule
