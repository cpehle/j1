module Op1Sel #(parameter WIDTH = 32) (input logic [1:0] op1_sel,
				       input logic		is_immediate,
				       input logic [WIDTH-1:0]	st0,
				       input logic [WIDTH-1:0]	rst0,
				       input logic [WIDTH-1:0]	mem_din,
				       input logic [WIDTH-1:0]	io_din,
				       input logic [WIDTH-1:0]	immediate,
				       output logic [WIDTH-1:0] op1
				       );
   always_comb begin
      if (is_immediate) begin
	 op1 = immediate;
      end else begin
	 case (op1_sel[1:0])
	   2'b00 : op1 = st0;
	   2'b01 : op1 = rst0;
	   2'b10 : op1 = mem_din;
	   2'b11 : op1 = io_din;
	 endcase
      end
   end
endmodule
