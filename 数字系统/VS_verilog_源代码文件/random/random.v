module random(

  input      wire               clk,
  input      wire               rst_n,
  input      wire      [31:0]   seed,
  
  output     reg       [31:0]   random
);
  reg [31:0] data;
  always @ (posedge clk,negedge rst_n) begin
    if (!rst_n) begin
      data <= 32'b0;
      random <= 32'b0;
    end
      
    else if (data==32'b0)
      data <= seed; 
    else begin
      data[0] <= data[31];
      data[1] <= data[0] ^ data[31];
      data[2] <= data[1] ^ data[31];
      data[3] <= data[2] ^ data[31];
      data[4] <= data[3];
      data[5] <= data[4] ^ data[31];
      data[6] <= data[5];
      data[7] <= data[6] ^ data[31];
      data[8] <= data[7];
      data[9] <= data[8];
      data[10] <= data[9];
      data[11] <= data[10];
      data[12] <= data[11];
      data[13] <= data[12];
      data[14] <= data[13];
      data[15] <= data[14];
      data[16] <= data[15];
      data[17] <= data[16];
      data[18] <= data[17];
      data[19] <= data[18];
      data[20] <= data[19];
      data[21] <= data[20];
      data[22] <= data[21];
      data[23] <= data[22];
      data[24] <= data[23];
      data[25] <= data[24];
      data[26] <= data[25];
      data[27] <= data[26];
      data[28] <= data[27];
      data[29] <= data[28];
      data[30] <= data[29];
      data[31] <= data[30];
    end
      if (data[3:0]<4'hA) random[3:0] <= data[3:0];
      else random[3:0] <= ~data[3:0];
      if (data[7:4]<4'b1010) random[7:4] <= data[7:4];
      else random[7:4] <= ~data[7:4];
      if (data[11:8]<4'b1010) random[11:8] <= data[11:8];
      else random[11:8] <= ~data[11:8];
      if (data[15:12]<4'b1010) random[15:12] <= data[15:12];
      else random[15:12] <= ~data[15:12];
      if (data[19:16]<4'b1010) random[19:16] <= data[19:16];
      else random[19:16] <= ~data[19:16];
      if (data[23:20]<4'b1010) random[23:20] <= data[23:20];
      else random[23:20] <= ~data[23:20];
      if (data[27:24]<4'b1010) random[27:24] <= data[27:24];
      else random[27:24] <= ~data[27:24];
      if (data[31:28]<4'b1010) random[31:28] <= data[31:28];
      else random[31:28] <= ~data[31:28];
 end
endmodule