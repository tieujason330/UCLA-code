`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:31:55 01/28/2015 
// Design Name: 
// Module Name:    Rounding 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module Rounding(Exponent, Significand, FifthBit, E, F);
	input [2:0] Exponent;
	input [3:0] Significand;
	input FifthBit;
	
	output [2:0] E;
	output [3:0] F;
	
	reg overflowBit;
	reg [2:0] regE;
	reg [3:0] regF;
	
	always @*
	begin
		if( Significand[3:0] == 4'b1111 && FifthBit == 1'b1 )
		begin
			overflowBit = 1;
			regF[3:0] = 4'b1000;
		end
		else
		begin
			overflowBit = 0;
			regF[3:0] = Significand + FifthBit;
		end
		
		if( Exponent[2:0] == 3'b111 && overflowBit == 1'b1 )
		begin
			regE[2:0] = 3'b111;
			regF[3:0] = 4'b1111;
		end
		else
		begin
			regE[2:0] = Exponent + overflowBit;
		end
	end
	
	assign E = regE;
	assign F = regF;
endmodule