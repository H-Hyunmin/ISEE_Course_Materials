module summation (
   input clk,reset,
   output reg [7:0] out
);
parameter s0=0,s1=1,s2=2,s3=3,s4=4;
reg [2:0] state;
reg [7:0] sum;
reg [7:0] i;
always @(posedge clk or negedge reset) begin
   if (reset==0) begin
     out<=0;
     state<=s0;
   end
   else begin
     case (state)
      s0:begin
        sum<=0;
        i<=1;
        out<=0;
        state<=s1;
      end
      s1:begin
        sum<=sum+i;
        out<=0;
        state<=s2;
      end
      s2:begin
        out<=0;
        i<=i+1;
        state<=s3;
      end
      s3:begin
        out<=0;
        if(i!=11) 
          state<=s1;
        else 
        state<=s4;
      end
      s4:begin
        out<=sum;
        state<=s4;
      end
      default:begin
        out<=0;
        state<=s0;
      end
     endcase
   end
end
endmodule