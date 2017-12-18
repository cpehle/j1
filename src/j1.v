`include "common.h"
module j1 #(int WIDTH = 32, int DEPTH = 4, int ADDR_WIDTH = 16) (
   input logic		    clk,
   input logic		    resetq,
   output logic		    io_wr,
   output logic [ADDR_WIDTH-1:0] mem_addr,
   output logic		    mem_wr,
   output logic [WIDTH-1:0] dout,
   input logic [WIDTH-1:0]  mem_din,
   input logic [WIDTH-1:0]  io_din,
   output logic [12:0]	    code_addr,
   input logic [15:0]	    insn
   );

   typedef struct packed {
      logic [DEPTH-1:0] dsp; /// data stack pointer
      logic [WIDTH-1:0] st0; /// first element on the stack
      logic [12:0]	pc;  /// program counter
      logic [DEPTH-1:0] rsp; /// return stack pointer
   } J1State;

  // J1State s, sn;


   logic [DEPTH-1:0]	dsp;      // Data stack pointer
   logic [DEPTH-1:0]	dspN;

   logic [WIDTH-1:0]	st0;     // Top of data stack
   logic [WIDTH-1:0]	st0N;
   logic		dstkW; // D stack write
   logic [DEPTH-1:0]	dspI; // signed increment D stack

   logic [12:0]		pc, pcN;
   logic [DEPTH-1:0]	rsp, rspN;
   logic		rstkW; // R stack write
   logic [DEPTH-1:0]	rspI;  // signed increment R stack
   logic [WIDTH-1:0]	rstkD; // R stacks write value
   logic		reboot = 1;
   logic [12:0]		pc_plus_1 = pc + 1;
   logic [WIDTH-1:0]	immediate;
   logic		is_immediate;

   logic [1:0] 		op1_sel;
   logic [WIDTH-1:0] 	op1;
   
   
   assign mem_addr = st0N[ADDR_WIDTH-1:0];
   assign code_addr = {pcN};

   // The D and R stacks
   logic [WIDTH-1:0]	st1, rst0;

   
   Op1Sel op1sel(/*AUTOINST*/
		 // Outputs
		 .op1			(op1[WIDTH-1:0]),
		 // Inputs
		 .op1_sel		(op1_sel[1:0]),
		 .is_immediate		(is_immediate),
		 .st0			(st0[WIDTH-1:0]),
		 .rst0			(rst0[WIDTH-1:0]),
		 .mem_din		(mem_din[WIDTH-1:0]),
		 .io_din		(io_din[WIDTH-1:0]),
		 .immediate		(immediate[WIDTH-1:0]));
   
   
   stack #(.WIDTH(WIDTH), .DEPTH(DEPTH)) dstack(.clk(clk),
					       .resetq(resetq),
					       .ra(dsp),
					       .rd(st1),
					       .we(dstkW),
					       .wa(dspN),
					       .wd(st0));

   stack #(.WIDTH(WIDTH), .DEPTH(DEPTH)) rstack(.clk(clk),
						.resetq(resetq),
						.ra(rsp),
						.rd(rst0),
						.we(rstkW),
						.wa(rspN),
						.wd(rstkD));

   always_comb begin
      // Compute the new value of st0
      immediate = { {(WIDTH - 15){1'b0}}, insn[14:0] };
      is_immediate = insn[15];
      casez ({insn[15:8]})
	8'b1??_?????: st0N = immediate;    // literal
	8'b000_?????: st0N = st0;                     // jump
	8'b010_?????: st0N = st0;                     // call
	8'b001_?????: st0N = st1;                     // conditional jump
	8'b011_?0000: st0N = st0;                     // ALU operations...
	8'b011_?0001: st0N = st1;
	8'b011_?0010: st0N = st0 + st1;
	8'b011_?0011: st0N = st0 & st1;
	8'b011_?0100: st0N = st0 | st1;
	8'b011_?0101: st0N = st0 ^ st1;
	8'b011_?0110: st0N = ~st0;
	8'b011_?0111: st0N = {WIDTH{(st1 == st0)}};
	8'b011_?1000: st0N = {WIDTH{($signed(st1) < $signed(st0))}};
	8'b011_?1001: st0N = st1 >> 1;
	8'b011_?1010: st0N = st1 << 1;
	8'b011_?1011: st0N = rst0;
	8'b011_?1100: st0N = mem_din;
	8'b011_?1101: st0N = io_din;
	8'b011_?1110: st0N = {{(WIDTH - 8){1'b0}}, rsp, dsp};
	8'b011_?1111: st0N = {WIDTH{(st1 < st0)}};
	default: st0N = {WIDTH{1'bx}};
      endcase
   end

   logic func_T_N =   (insn[6:4] == 1);
   logic func_T_R =   (insn[6:4] == 2);
   logic func_write = (insn[6:4] == 3);
   logic func_iow =   (insn[6:4] == 4);
   // logic [3:0] alu_func = insn[11:8];
   wire is_alu = (insn[15:13] == 3'b011);


   assign mem_wr = !reboot & is_alu & func_write;
   assign dout = st1;
   assign io_wr = !reboot & is_alu & func_iow;
   assign rstkD = (insn[13] == 1'b0) ? {{(WIDTH - 14){1'b0}}, pc_plus_1, 1'b0} : st0;


   always_comb begin
      casez ({insn[15:13]})
	// immediate constants increase the stack pointer by one
	3'b1??:   {dstkW, dspI} = {1'b1, 4'b0001};
	// conditional jump
	3'b001:   {dstkW, dspI} = {1'b0, 4'b1111};
	// alu operations optionally write to the data stack
	3'b011:   {dstkW, dspI} = {func_T_N, {insn[1], insn[1], insn[1:0]}};
	default:  {dstkW, dspI} = {1'b0, 4'b0000};
      endcase
      dspN = dsp + dspI;

      casez ({insn[15:13]})
	// call instruction
	3'b010:   {rstkW, rspI} = {1'b1, 4'b0001};
	// alu operations optionally write to the return stack
	3'b011:   {rstkW, rspI} = {func_T_R, {insn[3], insn[3], insn[3:2]}};
	default:  {rstkW, rspI} = {1'b0, 4'b0000};
      endcase
      rspN = rsp + rspI;

      casez ({reboot, insn[15:13], insn[7], |st0})
	6'b1_???_?_?:   pcN = 0;
	6'b0_000_?_?, 6'b0_010_?_?, 6'b0_001_?_0:   pcN = insn[12:0];
	6'b0_011_1_?:   pcN = rst0[13:1];
	default:        pcN = pc_plus_1;
      endcase
   end

  always_ff @(negedge resetq or posedge clk) begin
       if (!resetq) begin
	  reboot <= 1'b1;
	  { pc, dsp, st0, rsp } <= 0;
       end else begin
	  reboot <= 0;
	  { pc, dsp, st0, rsp } <= { pcN, dspN, st0N, rspN };
       end
  end
endmodule
