module cu (
    input clk,
    input rst,
    input [2:0] IR75,
    input Anot0,
    output reg IRload,
    output reg PCload,
    output reg JNZmux,
    output reg Halt_out,
    output reg OutE,
    output reg Aload,
    output reg INmux
);
    parameter Fetch = 3'b000,Decode = 3'b001,Input = 3'b011,Output = 3'b100,Dec=3'b101,JNZ=3'b110,Halt=3'b111;
    reg[2:0] state,next_state;
    always @(posedge clk or negedge rst  ) begin
        if(!rst) begin
            state <= Fetch;
        end
        else begin
            state <= next_state;
        end        
    end
    always @(posedge clk or negedge rst) begin
        if (!rst) begin
            IRload<=1;
            PCload<=1;
            JNZmux<=0;
            Halt_out<=0;
            OutE<=0;
            Aload<=0;
            INmux<=0;
        end
        else begin case (next_state)
            Fetch: begin
                IRload<=1;
                PCload<=1;
                JNZmux<=0;
                Halt_out<=0;
                OutE<=0;
                Aload<=0;
                INmux<=0;
            end
            Decode: begin
                IRload<=0;
                PCload<=0;
                JNZmux<=0;
                Halt_out<=0;
                OutE<=0;
                Aload<=0;
                INmux<=0;
            end
            Input: begin
                IRload<=0;
                PCload<=0;
                JNZmux<=0;
                Halt_out<=0;
                OutE<=0;
                Aload<=1;
                INmux<=1;
            end
            Output: begin
                IRload<=0;
                PCload<=0;
                JNZmux<=0;
                Halt_out<=0;
                OutE<=1;
                Aload<=0;
                INmux<=0;
            end
            Dec: begin
                IRload<=0;
                PCload<=0;
                JNZmux<=0;
                Halt_out<=0;
                OutE<=0;
                Aload<=1;
                INmux<=0;
            end
            JNZ: begin
                IRload<=0;
                PCload<=(Anot0==1)?1:0;
                JNZmux<=1;
                Halt_out<=0;
                OutE<=0;
                Aload<=0;
                INmux<=0;
            end
            Halt: begin
                IRload<=0;
                PCload<=0;
                JNZmux<=0;
                Halt_out<=1;
                OutE<=0;
                Aload<=0;
                INmux<=0;
            end
            default: begin
                IRload<=0;
                PCload<=0;
                JNZmux<=0;
                Halt_out<=0;
                OutE<=0;
                Aload<=0;
                INmux<=0;
            end
        endcase
        end
    end

    always @(state or IR75 ) begin
        case (state)
        Fetch: begin
            next_state=Decode;
        end
        Decode: begin
            if (IR75==3'b011) next_state=Input;
            else if (IR75==3'b100) next_state=Output;
            else if (IR75==3'b101) next_state=Dec;
            else if (IR75==3'b110) next_state=JNZ;
            else if (IR75==3'b111) next_state=Halt;
            else next_state=Fetch;
        end
        Input: begin
           next_state=Fetch;
        end
        Output: begin
            next_state=Fetch;
        end
        Dec: begin
            next_state=Fetch;
        end
        JNZ: begin
            next_state=Fetch;
        end
        Halt: begin
            next_state=Halt;
        end
        default: begin
            next_state=Fetch;
        end
        endcase
    end
endmodule