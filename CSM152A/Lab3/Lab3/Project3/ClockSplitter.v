`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:09:10 02/04/2015 
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
    input MasterClock,
    input Reset,
    output reg NormalClock,
    output reg AdjustClock,
    output reg FasterClock,
    output reg BlinkClock
    );

	parameter normalFlipTime = 50000000; // 1 Hz clock flips every 50000000 cycles
	parameter adjustFlipTime = 25000000; // 2 Hz clock flips every 25000000 cycles
	parameter fastFlipTime = 250000; //200 Hz clock flips every 500000 cycles
	parameter blinkFlipTime = 12500000; //4 Hz clock flips every 12500000 cycles
	
//	parameter normalFlipTime = 500; // 1 Hz clock flips every 50000000 cycles
//	parameter adjustFlipTime = 250; // 2 Hz clock flips every 25000000 cycles
//	parameter fastFlipTime = 75; //100 Hz clock flips every 500000 cycles
//	parameter blinkFlipTime = 300; //4 Hz clock flips every 12500000 cycles
	
	reg	[31:0] normalCtr;
	reg	[31:0] adjCtr;
	reg	[31:0] fastCtr;
	reg	[31:0] blinkCtr;
	
	initial
	begin
		normalCtr = 0;
		adjCtr = 0;
		fastCtr = 0;
		blinkCtr = 0;
	end
	
	always @ (posedge MasterClock, posedge Reset)
	begin
		if( Reset )
		begin
			normalCtr = 0;
			NormalClock = 0;
			adjCtr = 0;
			AdjustClock = 0;
			fastCtr = 0;
			FasterClock = 0;
			blinkCtr = 0;
			BlinkClock = 0;
		end
		else
		begin
			//Do we need to truncate? We should never count to 2^32
			normalCtr = normalCtr + 1;
			adjCtr = adjCtr + 1;
			fastCtr = fastCtr + 1;
			blinkCtr = blinkCtr + 1;
			
			if( normalCtr == normalFlipTime )
			begin
				NormalClock = 1;
			end
			if( normalCtr == normalFlipTime * 2 )
			begin
				NormalClock = 0;
				normalCtr = 0;
			end
			
			if( adjCtr == adjustFlipTime )
			begin
				AdjustClock = 1;
			end
			if( adjCtr == adjustFlipTime * 2 )
			begin
				AdjustClock = 0;
				adjCtr = 0;
			end
			
			if( fastCtr == fastFlipTime )
			begin
				FasterClock = 1;
			end
			if( fastCtr == fastFlipTime * 2 )
			begin
				FasterClock = 0;
				fastCtr = 0;
			end
			
			if( blinkCtr == blinkFlipTime )
			begin
				BlinkClock = 1;
			end
			if( blinkCtr == blinkFlipTime * 2 )
			begin
				BlinkClock = 0;
				blinkCtr = 0;
			end
		end
	end
endmodule
