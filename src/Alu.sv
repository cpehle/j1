module Alu #(parameter WIDTH = 32) (input logic [3:0] func,
				    input logic [WIDTH-1:0] tos,
				    input logic [WIDTH-1:0] nos,
				    output logic [WIDTH-1:0] out
				    );
   always_comb
     case (func[3:0])
       4'b0000: out = tos;
       4'b0001: out = ~tos;
       4'b0010: out = {{(WIDTH-1){1'b0}}, tos < nos};
       4'b0011: out = {{(WIDTH-1){1'b0}}, (tos == nos)};
       4'b0100: out = tos<<1;
       4'b0101: out = tos>>1;
//       4'b0110: out = {tos[14:0],tos[15]};
//       4'b0111: out = {tos[15:8]};
       4'b0110,
       4'b1000,
       4'b0111: out = nos;
       4'b1001: out = nos + tos;
       4'b1010: out = nos - tos;
       4'b1011: out = nos & tos;
       4'b1100: out = nos | tos;
       4'b1101: out = nos ^ tos;
       4'b1110: out = tos+1;
       4'b1111: out = tos-1;
     endcase
endmodule
