`include "dp.v"
`include "fsm.v"
module top (
    input Clock,Reset,
    output Data,
    input [7:0]N
);
wire MuxN,LoadN,MuxC,LoadC,Out,Nd0,N0d0;
  dp dp(
      .Clock(Clock),
      .Reset(Reset),
      .MuxN(MuxN),
      .LoadN(LoadN),
      .MuxC(MuxC),
      .LoadC(LoadC),
      .Out(Out),
      .N(N),
      .Data(Data),
      .Nd0(Nd0),
      .N0d0(N0d0)
  );
  fsm fsm(
      .Clock(Clock),
      .Reset(Reset),
      .N0d0(N0d0),
      .Nd0(Nd0),
      .Nload(LoadN),
      .Cload(LoadC),
      .Out(Out),
      .MuxC(MuxC),
      .MuxN(MuxN)
  );
endmodule