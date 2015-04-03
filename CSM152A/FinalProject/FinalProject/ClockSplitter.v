`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:31:10 02/25/2015 
// Design Name: 
// Module Name:    ClockSplitter 
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
module ClockSplitter(
    input Clock,
    input [3:0] Difficulty,
    output reg DisplayClock,
    output reg GameClock,
	 output reg ScoreDisplayClock
    );

	parameter	displayFlipTime = 1; //50 MHz output
	parameter	gameClockFlipTime = 500000; //25 Hz
	parameter	scoreDisplayFlipTime = 250000; //200 Hz

	reg	[31:0] displayCtr;
	reg	[31:0] gameCtr;
	reg	[31:0] scoreDisplayCtr;
	
	always @(posedge Clock)
	begin
		displayCtr = displayCtr + 1;
		gameCtr = gameCtr + Difficulty;
		scoreDisplayCtr = scoreDisplayCtr + 1;
		
		if( displayCtr == displayFlipTime )
		begin
			DisplayClock = 1;
		end
		else if( displayCtr == 2 * displayFlipTime )
		begin
			DisplayClock = 0;
			displayCtr = 0;
		end
		
        if( gameCtr >= 2 * gameClockFlipTime )
		begin
			GameClock = 0;
			gameCtr = 0;
		end
		else if( gameCtr >= gameClockFlipTime )
		begin
			GameClock = 1;
		end
		
		if( scoreDisplayCtr == scoreDisplayFlipTime )
		begin
			ScoreDisplayClock = 1;
		end
		else if( scoreDisplayCtr == 2 * scoreDisplayFlipTime )
		begin
			ScoreDisplayClock = 0;
			scoreDisplayCtr = 0;
		end
	end
endmodule
