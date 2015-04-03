`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   17:20:29 01/28/2015
// Design Name:   fpcvt
// Module Name:   C:/Users/152/Section3/Lab2/project2/testbench.v
// Project Name:  project2
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: fpcvt
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module testbench;

	// Inputs
	reg [11:0] D;

	// Outputs
	wire S;
	wire [2:0] E;
	wire [3:0] F;

	// Instantiate the Unit Under Test (UUT)
	fpcvt uut (
		.D(D), 
		.S(S), 
		.E(E), 
		.F(F)
	);

	initial begin
		// Initialize Inputs
		D = 3'b100;

		// Wait 100 ns for global reset to finish
		#1000;
        
		// Add stimulus here
		$display ("D: %b", D);
		$display ("S: %b", S);
		$display ("E: %b", E);
		$display ("F: %b", F);
	end
      
endmodule

