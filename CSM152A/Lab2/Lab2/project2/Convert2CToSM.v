`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:54:44 01/28/2015 
// Design Name: 
// Module Name:    Convert2CToSM 
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
module Convert2CToSM(D, S, Magnitude);
	
	input [11:0] D;
	output S;
	output [11:0] Magnitude;

	reg [11:0] mag;

	always @*
	begin
		mag = (D ^ { 12{S} }) + S;
	end
	
	assign S = D[11];
	assign Magnitude = mag;
	
endmodule
