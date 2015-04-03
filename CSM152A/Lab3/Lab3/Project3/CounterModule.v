`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:02:12 02/09/2015 
// Design Name: 
// Module Name:    CounterModule 
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
module CounterModule(
    input Clock,
    input Reset,
	 input Select,
	 input Adjust,
    output [2:0] MinutesTens,
    output [3:0] MinutesOnes,
    output [2:0] SecondsTens,
    output [3:0] SecondsOnes
    );

	reg [2:0] minTensReg;
	reg [3:0] minOnesReg;
	reg [2:0] secTensReg;
	reg [3:0] secOnesReg;

	always @ (posedge Clock, posedge Reset)
	begin
		if( Reset == 1 ) //Reset
		begin
			minTensReg = 0;
			minOnesReg = 0;
			secOnesReg = 0;
			secTensReg = 0;
		end
		else //Increment
		begin
			if( Adjust == 1 )
			begin
				if( Select == 0 ) //increment minutes
				begin
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
				begin //increment seconds
					if( secOnesReg == 9 ) //overflow
					begin
						secOnesReg = 0;
						
						if( secTensReg == 5 ) //overflow
						begin
							secTensReg = 0;
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
			else //count normally
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
	end
	
	assign MinutesOnes = minOnesReg;
	assign MinutesTens = minTensReg;
	assign SecondsOnes = secOnesReg;
	assign SecondsTens = secTensReg;

endmodule
