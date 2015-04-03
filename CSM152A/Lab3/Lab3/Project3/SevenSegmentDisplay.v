`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:09:03 02/11/2015 
// Design Name: 
// Module Name:    SevenSegmentDisplay 
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
module SevenSegmentDisplay(
    input Clock,
    input BlinkClock,
    input Reset,
	 input Adjusting,
	 input Select,
    input [2:0] MinutesTens,
    input [3:0] MinutesOnes,
    input [2:0] SecondsTens,
    input [3:0] SecondsOnes,
    output [3:0] Anodes,
    output [7:0] Cathodes
    );

	reg [1:0] anodeIndex;
	reg [3:0] valueToDisplay;
	
	reg [3:0] anodeReg;
	reg [7:0] cathodeReg;
	
	always @ (posedge Clock, posedge Reset)
	begin
		if( Reset )
		begin
			anodeIndex = 0;
			valueToDisplay = 0;
			anodeReg = 4'b0000;
			cathodeReg = 8'b10110110;
		end
		else
		begin
			//Select Anode and value to display
			if( anodeIndex == 3 )
			begin
				anodeReg = 4'b0111;
				valueToDisplay[3] = 0;
				valueToDisplay[2:0] = MinutesTens[2:0];
			end
			if( anodeIndex == 2 )
			begin
				anodeReg = 4'b1011;
				valueToDisplay[3:0] = MinutesOnes[3:0];
			end
			if( anodeIndex == 1 )
			begin
				anodeReg = 4'b1101;
				valueToDisplay[3] = 0;
				valueToDisplay[2:0] = SecondsTens[2:0];
			end
			if( anodeIndex == 0 )
			begin
				anodeReg = 4'b1110;
				valueToDisplay[3:0] = SecondsOnes[3:0];
			end
			
			//Select Cathodes to light up
			if(valueToDisplay == 0)
			begin
				cathodeReg = 8'b11000000;
			end
			if(valueToDisplay == 1)
			begin
				cathodeReg = 8'b11111001;
			end
			if(valueToDisplay == 2)
			begin
				cathodeReg = 8'b10100100;
			end
			if(valueToDisplay == 3)
			begin
				cathodeReg = 8'b10110000;
			end
			if(valueToDisplay == 4)
			begin
				cathodeReg = 8'b10011001;
			end
			if(valueToDisplay == 5)
			begin
				cathodeReg = 8'b10010010;
			end
			if(valueToDisplay == 6)
			begin
				cathodeReg = 8'b10000010;
			end
			if(valueToDisplay == 7)
			begin
				cathodeReg = 8'b11111000;
			end
			if(valueToDisplay == 8)
			begin
				cathodeReg = 8'b10000000;
			end
			if(valueToDisplay == 9)
			begin
				 cathodeReg = 8'b10010000;
			end
			
			if(valueToDisplay > 9)
			begin
				cathodeReg = 8'b10110110; //error
			end
			
			
			//Blink
			if( BlinkClock == 1 && Adjusting == 1 ) //Not working yet.
			begin
				if( Select == 0 )
				begin
					anodeReg[3:2] = 2'b11;
				end
				else
				begin
					anodeReg[1:0] = 2'b11;
				end
			end
			
			//Progress to next anode
			if( anodeIndex == 3 )
			begin
				anodeIndex = 0;
			end
			else
			begin
				anodeIndex = anodeIndex + 1;
			end
		end
	end

	assign Anodes = anodeReg;
	assign Cathodes = cathodeReg;

endmodule
