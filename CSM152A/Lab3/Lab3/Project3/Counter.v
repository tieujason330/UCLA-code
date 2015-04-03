`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:12:21 02/09/2015 
// Design Name: 
// Module Name:    Counter 
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
module Counter(
    input Clock,
	 input Reset,
    output [3:0] MinutesOnes,
    output [2:0] MinutesTens,
    output [3:0] SecondsOnes,
    output [2:0] SecondsTens
    );

	reg [3:0] minOnesReg;
	reg [2:0] minTensReg;
	reg [3:0] secOnesReg;
	reg [2:0] secTensReg;


	always @ (posedge Clock)
	begin
		if( Reset == 1 ) //Reset
		begin
			secOnesReg = 0;
			secTensReg = 0;
			minOnesReg = 0;
			minTensReg = 0;
		end
		else //Increment
		begin
			if( secOnesReg == 9 ) //overflow
			begin
				secOnesReg = 0;
				
				if( secTensReg == 5 ) //overflow
				begin
					secTensReg = 0;
					
					if( minOnesReg == 9 ) //overflow
					begin
						minOnesReg = 0;
						
						if( minTensReg == 5 ) //overflow
						begin
							minTensReg = 0;
						end
						else
						begin
							minTensReg = minTensReg + 1;
						end
					end
					else
					begin
						minOnesReg = minOnesReg + 1;
					end
				end
				else
				begin
					secTensReg = secTensReg + 1;
				end
			end
			else
			begin
				secOnesReg = secOnesReg + 1;
			end
		end
	end

	
	
	assign MinutesOnes = minOnesReg;
	assign MinutesTens = minTensReg;
	assign SecondsOnes = secOnesReg;
	assign SecondsTens = secTensReg;
	
endmodule
