module stack #(parameter int WIDTH = 32, parameter DEPTH=4) (
  input logic clk,
  /* verilator lint_off UNUSED */
  input logic resetq,
  /* verilator lint_on UNUSED */
  input logic [DEPTH-1:0] ra,
  output logic [WIDTH-1:0] rd,
  input logic we,
  input logic [DEPTH-1:0] wa,
  input logic [WIDTH-1:0] wd
);

  logic [WIDTH-1:0] store[0:(2**DEPTH)-1];

  always_ff @(posedge clk) begin
    if (we) begin
      store[wa] <= wd;
    end
  end

  assign rd = store[ra];
endmodule
