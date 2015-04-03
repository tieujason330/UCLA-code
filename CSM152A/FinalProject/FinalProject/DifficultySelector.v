`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:15:07 02/25/2015 
// Design Name: 
// Module Name:    DifficultySelector 
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
module DifficultySelector(
	 input Clock,
    input [3:0] ScoreThousands,
	 input [3:0] ScoreHundreds,
	 input [3:0] ScoreTens,
	 input [3:0] ScoreOnes,
    input [7:0] Switches,
	 output [3:0] Difficulty,
	 output [7:0] Leds
    );
	 
	 parameter Score8Thresh = 70;
	 parameter Score7Thresh = 60;
	 parameter Score6Thresh = 50;
	 parameter Score5Thresh = 40;
	 parameter Score4Thresh = 30;
	 parameter Score3Thresh = 20;
	 parameter Score2Thresh = 10;
	 
	 reg	[31:0] totalScore;
	 
	 reg	[3:0] diffReg;
	 reg	[7:0] ledsReg;
	 
	 always @(Clock)
	 begin
		 totalScore = ScoreThousands * 1000 + ScoreHundreds * 100 + ScoreTens * 10 + ScoreOnes;
	 
		 if( Switches[7] == 1 || totalScore >= Score8Thresh )
		 begin
			diffReg = 8;
			ledsReg = 8'b10000000;
		 end
		 else if( Switches[6] == 1 || totalScore >= Score7Thresh )
		 begin
			diffReg = 7;
			ledsReg = 8'b01000000;
		 end
		 else if( Switches[5] == 1 || totalScore >= Score6Thresh )
		 begin
			diffReg = 6;
			ledsReg = 8'b00100000;
		 end
		 else if( Switches[4] == 1 || totalScore >= Score5Thresh )
		 begin
			diffReg = 5;
			ledsReg = 8'b00010000;
		 end
		 else if( Switches[3] == 1 || totalScore >= Score4Thresh )
		 begin
			diffReg = 4;
			ledsReg = 8'b00001000;
		 end
		 else if( Switches[2] == 1 || totalScore >= Score3Thresh )
		 begin
			diffReg = 3;
			ledsReg = 8'b00000100;
		 end
		 else if( Switches[1] == 1 || totalScore >= Score2Thresh )
		 begin
			diffReg = 2;
			ledsReg = 8'b00000010;
		 end
		 else
		 begin
			diffReg = 1;
			ledsReg = 8'b00000001;
		 end
	 end
	 
	 assign Difficulty = diffReg;
	 assign Leds = ledsReg;
	 
endmodule
