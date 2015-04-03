`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:56:39 01/28/2015 
// Design Name: 
// Module Name:    CountLeadingZeros 
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
module CountLeadingZeros(Magnitude, NumLeadingZeros, Exponent);
	input [11:0] Magnitude;
	output [3:0] NumLeadingZeros;
	output [2:0] Exponent;
	
	reg [3:0] leadingZeros;
	reg [2:0] exp;
	integer i;
	
	always @*
	begin
		leadingZeros = 12;
		
		if( Magnitude[11] == 1'b1 )
		begin
			leadingZeros = 0;
		end
		else if( Magnitude[10] == 1'b1 )
		begin
			leadingZeros = 1;
		end
		else if( Magnitude[9] == 1'b1 )
		begin
			leadingZeros = 2;
		end
		else if( Magnitude[8] == 1'b1 )
		begin
			leadingZeros = 3;
		end
		else if( Magnitude[7] == 1'b1 )
		begin
			leadingZeros = 4;
		end
		else if( Magnitude[6] == 1'b1 )
		begin
			leadingZeros = 5;
		end
		else if( Magnitude[5] == 1'b1 )
		begin
			leadingZeros = 6;
		end
		else if( Magnitude[4] == 1'b1 )
		begin
			leadingZeros = 7;
		end
		else if( Magnitude[3] == 1'b1 )
		begin
			leadingZeros = 8;
		end
		else if( Magnitude[2] == 1'b1 )
		begin
			leadingZeros = 9;
		end
		else if( Magnitude[1] == 1'b1 )
		begin
			leadingZeros = 10;
		end
		else if( Magnitude[0] == 1'b1 )
		begin
			leadingZeros = 11;
		end
		
		if( leadingZeros >= 8 )
		begin
			exp = 0;
		end
		else if( leadingZeros == 0 )
		begin
			exp = 7;
		end
		else
		begin
			exp = 8 - leadingZeros;
		end
	end
	
	assign NumLeadingZeros[3:0] = leadingZeros[3:0];
	assign Exponent[2:0] = exp[2:0];
endmodule