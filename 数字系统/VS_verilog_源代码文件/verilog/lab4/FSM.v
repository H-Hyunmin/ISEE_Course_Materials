module FSM (
    input clk,
    input rst,
    input b0,b1,
    output  reg  unlock,
    output  [2:0] debugstate
    );
    parameter s_reset = 0;
    parameter s_1 = 1;
    parameter s_10 = 2;
    parameter s_100 = 3;
    parameter s_1001 = 4;
    parameter s_10010 = 5;
    reg[2:0] state,next_state;
    
    assign debugstate=state;
    always@(posedge clk or negedge rst) begin
        if(!rst) begin
            state <= s_reset;
        end
        else begin
            state <= next_state;
        end
    end

    always @(posedge clk or negedge rst) begin
        if (!rst) unlock=0;
        else begin case (next_state)
            s_reset: unlock<=0;
            s_1: unlock<=0;
            s_10: unlock<=0;
            s_100: unlock<=0;
            s_1001: unlock<=0;
            s_10010: unlock<=1;
            default: unlock<=0;
        endcase
        end
    end

    always @(state or b0 or b1)
    begin
        case (state)
        s_reset: begin
            if ({b0,b1}==2'b01) next_state=s_1;
            else next_state=s_reset;
        end
        s_1: begin
            if ({b0,b1}==2'b10) next_state=s_10;
            else next_state=s_1;
        end
        s_10: begin
            if ({b0,b1}==2'b10) next_state=s_100;
            else if ({b0,b1}==2'b01) next_state=s_1;
            else next_state=s_10;
        end
        s_100: begin
            if ({b0,b1}==2'b01) next_state=s_1001;
            else if ({b0,b1}==2'b10) next_state=s_reset;
            else next_state=s_100;
        end
        s_1001: begin
            if ({b0,b1}==2'b10) next_state=s_10010;
            else if ({b0,b1}==2'b01) next_state=s_1;
            else next_state=s_1001;
        end
        s_10010: begin
            if ({b0,b1}==2'b10) next_state=s_reset;
            else if ({b0,b1}==2'b01) next_state=s_100;
            else next_state=s_10010;
        end
        endcase
    end
endmodule