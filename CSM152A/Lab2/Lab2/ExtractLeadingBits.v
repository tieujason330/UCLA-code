`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:32:40 01/28/2015 
// Design Name: 
// Module Name:    ExtractLeadingBits 
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
module ExtractLeadingBits(NumLeadingZeros, Magnitude, Significand, FifthBit);
	input [3:0] NumLeadingZeros;
	input [11:0] Magnitude;
	output [3:0] Significand;
	output FifthBit;
	
	reg [11:0] shiftedMagnitude;
	reg [3:0] sig;
	reg fifth;
	
	always @*
	begin
		shiftedMagnitude = Magnitude << NumLeadingZeros;
		sig[3:0] = shiftedMagnitude[11:8];
		fifth = shiftedMagnitude[7];
	end
	
	assign Significand = sig;
	assign FifthBit = fifth;
endmodule