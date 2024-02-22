`include "adder.v"
`include "mux21.v"
`include "register.v"
`include "shifter.v"
`include "tri_buffer.v"
module dp(
    input Clock,Reset,
    input MuxN,LoadN,MuxC,LoadC,Out,
    input [7:0]N,
    output Data,
    output Nd0,N0d0
);
    wire[7:0] dp_MuxN,dp_shift,dp_N;
    wire[3:0] dp_MuxC,dp_sum,dp_C;
    wire C4;
    mux21#(8) muxN(
        .S(MuxN),
        .D1(N),
        .D0(dp_shift),
        .Y(dp_MuxN)
    );
    mux21#(4) muxC(
        .S(MuxC),
        .D1(4'b0000),
        .D0(dp_sum),
        .Y(dp_MuxC)
    );
    register#(8) regN(
        .clock(Clock),
        .clear(Reset),
        .load(LoadN),
        .D(dp_MuxN),
        .Q(dp_N)
    );
    register#(4) regC(
        .clock(Clock),
        .clear(Reset),
        .load(LoadC),
        .D(dp_MuxC),
        .Q(dp_C)
    );
    shifter#(8) shift(
        .s(2'b10),
        .date(dp_N),
        .outdate(dp_shift)
    );
    adder sum(
        .a(dp_C),
        .b(4'b0001),
        .sum(dp_sum)
    );
    assign C4=(dp_C==4'b0100)?1'b1:1'b0;
    assign Nd0=(dp_N==0)?1'b1:1'b0;
    assign N0d0=dp_N[0];
    tri_buffer#(1) buffer(
        .E(Out),
        .D(C4),
        .Y(Data)
    );
endmodule