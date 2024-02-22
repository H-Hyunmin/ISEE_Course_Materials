module clk_division
#(parameter F_CLK_SYS = 200_00_ , F_CLK_DIVISION = 1 )//,COUNTER_MAX = F_CLK_SYS / F_CLK_DIVISION / 2 - 1 )
    (
    input ena,
    input clk_in,
    output reg clk_out = 1'b0
    );
    integer counter = 0;
    always@(posedge clk_in)
        begin
            if( ~ena )
                begin
                    counter = 32'b0;
                    clk_out = 1'b0;
                end
            else
                begin
                    if( counter == F_CLK_SYS / F_CLK_DIVISION / 2 - 1 )
                        begin
                            counter = 32'b0;
                            clk_out = ~clk_out;
                        end
                    else
                        begin
                            counter = counter + 32'b1;
                            clk_out = clk_out;
                        end
                end
        end
endmodule