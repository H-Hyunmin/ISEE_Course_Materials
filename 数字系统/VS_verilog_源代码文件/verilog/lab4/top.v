`include "botton.v"
`include "FSM.v"
`include "Segment.v"

module top (
    input clk,
    input rst,
    input key0,key1,
    output  [0:0]unlock,
    output  [7:0]Sel,
    output  [7:0]DisPlay,
    output  [7:0]DisPlay1
);
   wire b0,b1;
   wire [2:0] current_state;
   reg  [31:0] data;
   botton u_botton(
       .clk(clk),
       .rst_n(rst),
       .key(key0),
       .press(b0)
   );
    botton u_botton1(
         .clk(clk),
         .rst_n(rst),
         .key(key1),
         .press(b1)
    );
   FSM u_FSM(
       .clk(clk),
       .rst(rst),
       .b0(b0),
       .b1(b1),
       .unlock(unlock),
       .debugstate(current_state)
   );
    always @(posedge clk ) begin
        case (current_state)
   3'b000: data<=32'h00000000;
   3'b001: data<=32'h00000001;
   3'b010: data<=32'h00000010;
   3'b011: data<=32'h00000100;
   3'b100: data<=32'h00001001;
   3'b101: data<=32'h00010010;
        endcase
    end
    
    Segment u_Segment(
         .Clk(clk),
         .Reset_N(rst),
         .Disp_Data(data),
         .state(current_state),
         .Sel(Sel),
         .DisPlay(DisPlay),
         .DisPlay1(DisPlay1)
    );
endmodule