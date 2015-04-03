`timescale 1ns / 1ps

////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer:
//
// Create Date:   17:18:34 02/09/2015
// Design Name:   Main
// Module Name:   C:/Users/152/Section3/Lab3/Project3/tb.v
// Project Name:  Project3
// Target Device:  
// Tool versions:  
// Description: 
//
// Verilog Test Fixture created by ISE for module: Main
//
// Dependencies:
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
////////////////////////////////////////////////////////////////////////////////

module tb;

	// Inputs
	reg MasterClock;
	reg Pause;
	reg Adjust;
	reg Reset;

	// Outputs
	wire [2:0] MinTens;
	wire [3:0] MinOnes;
	wire [2:0] SecTens;
	wire [3:0] SecOnes;

	// Instantiate the Unit Under Test (UUT)
	Main uut (
		.MasterClock(MasterClock),
		.Pause(Pause), 
		.Adjust(Adjust), 
		.Reset(Reset), 
		.MinTens(MinTens), 
		.MinOnes(MinOnes), 
		.SecTens(SecTens), 
		.SecOnes(SecOnes),
		.An(An),
		.C(C)
	);

	initial begin
		// Initialize Inputs
		MasterClock = 0;
		Pause = 0;
		Adjust = 0;
		Reset = 1;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here
		Reset = 0;
		
		#100000;
		//$finish;
	end
	
	always 
	begin
		#5;
		MasterClock = !MasterClock;
	end
      
endmodule

