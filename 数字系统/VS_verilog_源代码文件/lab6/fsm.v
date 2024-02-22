module fsm (
     input Clock,Reset,
     input N0d0,Nd0,
     output reg Nload,Cload,Out,MuxC,MuxN
);
    parameter s0 = 0,s1 = 1,s2 = 2,s3 = 3,s4 = 4;
    reg [2:0] state;
    always @(posedge Clock or negedge Reset) begin
        if(~Reset)
            state <= s0;
        else
            case(state)
                s0: state <= s1;
                s1:begin
                  if (Nd0) begin
                    state <= s4;
                  end
                    else if (N0d0) begin
                        state <= s3;
                    end
                    else begin
                        state <= s2;
                    end
                end 
                s2: state <= s1;
                s3: state <= s2;
                s4: state <= s4;
                default : state <= s0;
            endcase
    end
    always @(state) begin
        case (state)
            s0: begin
                Nload <= 1'b1;
                Cload <= 1'b1;
                Out <= 1'b0;
                MuxC <= 1'b1;
                MuxN <= 1'b1;
            end
            s1: begin
                Nload <= 1'b0;
                Cload <= 1'b0;
                Out <= 1'b0;
                MuxC <= 1'b0;
                MuxN <= 1'b0;
            end
            s2: begin
                Nload <= 1'b1;
                Cload <= 1'b0;
                Out <= 1'b0;
                MuxC <= 1'b0;
                MuxN <= 1'b0;
            end
            s3: begin
                Nload <= 1'b0;
                Cload <= 1'b1;
                Out <= 1'b0;
                MuxC <= 1'b0;
                MuxN <= 1'b0;
            end
            s4: begin
                Nload <= 1'b0;
                Cload <= 1'b0;
                Out <= 1'b1;
                MuxC <= 1'b0;
                MuxN <= 1'b0;
            end
            default: begin
                Nload <= 1'b0;
                Cload <= 1'b0;
                Out <= 1'b0;
                MuxC <= 1'b0;
                MuxN <= 1'b0;
            end
        endcase
    end
endmodule


