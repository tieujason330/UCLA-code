`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:52:58 01/28/2015 
// Design Name: 
// Module Name:    fpcvt 
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
module fpcvt(D, S, E, F
    );
	input [11:0] D;
	output S;
	output [2:0] E;
	output [3:0] F;
	
	wire S;
	wire [2:0] E;
	wire [3:0] F;
	
  // TODO Your implementation goes here
	wire [11:0] Magnitude;
	wire [3:0] NumLeadingZeros;
	wire [2:0] Exponent;
	wire [3:0] Significand;
	wire FifthBit;

	Convert2CToSM m1(.D(D), .S(S), .Magnitude(Magnitude));
	CountLeadingZeros m2(.Magnitude(Magnitude), .NumLeadingZeros(NumLeadingZeros), .Exponent(Exponent));
	ExtractLeadingBits m3(.NumLeadingZeros(NumLeadingZeros), .Magnitude(Magnitude), .Significand(Significand), .FifthBit(FifthBit));
	Rounding m4(.Exponent(Exponent), .Significand(Significand), .FifthBit(FifthBit), .E(E), .F(F));
endmodule
