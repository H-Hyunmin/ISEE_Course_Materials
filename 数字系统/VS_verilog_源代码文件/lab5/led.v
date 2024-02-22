module led
#(parameter ZERO = 4'h0,ONE = 4'h1,TWO = 4'h2,THREE = 4'h3,
    FOUR = 4'h4,FIVE = 4'h5,SIX = 4'h6,SEVEN = 4'h7,
    EIGHT = 4'h8,NINE = 4'h9,DECIMAL_POINT = 4'hA,
    BLANK = 4'hB,MINUS = 4'hC,d = 4'hD,l = 4'hE
)
    (
    input [3:0] seg_selection,
    output reg [7:0] led
);
    always@(*)
    begin
        case(seg_selection)
            ZERO:
            led = 8'hFC;
            ONE:
            led = 8'h60;
            TWO:
            led = 8'hDA;
            THREE:
            led = 8'hF2;
            FOUR:
            led = 8'h66;
            FIVE:
            led = 8'hB6;
            SIX:
            led = 8'hBE;
            SEVEN:
            led = 8'hE0;
            EIGHT:
            led = 8'hFE;
            NINE:
            led = 8'hF6;
            DECIMAL_POINT:
            led = 8'h01;
            BLANK:
            led = 8'h00;
            MINUS:
            led = 8'h02;
            d:
            led = 8'b0111_1010;
            l:
            led = 8'b0001_1100;
            default:
            led = 8'h00;
        endcase
    end
endmodule
