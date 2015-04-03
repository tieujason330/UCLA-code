`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:18:09 02/18/2015 
// Design Name: 
// Module Name:    Debouncer 
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
module Debouncer(
    input FasterClock,
    input PauseIn,
    input AdjustIn,
    input SelectIn,
    output reg PauseOut,
    output reg AdjustOut,
    output reg SelectOut
    );

	always @ (posedge FasterClock)
	begin
		PauseOut = PauseIn;
		AdjustOut = AdjustIn;
		SelectOut = SelectIn;
	end

endmodule
