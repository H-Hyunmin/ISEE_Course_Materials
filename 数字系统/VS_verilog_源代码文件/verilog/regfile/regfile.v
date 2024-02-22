module regfile 
 (
    input [7:0]  D,
    input [1:0] WA,
    input WE,
    input clk,
    input [1:0] RAA,RBA,
    input RAE, RBE,
    output reg [7:0]  portA,
    output reg [7:0]  portB
);
    reg [7:0]  reg1 , reg2, reg3, reg4;
    always @(posedge clk) begin
      if (WE) 
       case (WA)
       0: reg1 <= D;
         1: reg2 <= D;
            2: reg3 <= D;
                3: reg4 <= D;
         endcase
    end

    always @(RAA,RAE) begin
      if (RAE) begin
         case (RAA)
         0: portA <= reg1;
            1: portA <= reg2;
                2: portA <= reg3;
                 3: portA <= reg4;
                 default : portA <= 8'h00;
            endcase
        end
        else begin
          portA <= 8'h00;
        end
    end


    always @(RBA,RBE) begin
      if (RBE) begin
            case (RBA)
            0: portB <= reg1;
                1: portB <= reg2;
                    2: portB <= reg3;
                    3: portB <= reg4;
                    default : portB <= 8'h00;
                endcase
            end
            else begin
            portB <= 8'h00;
            end
    end
    endmodule