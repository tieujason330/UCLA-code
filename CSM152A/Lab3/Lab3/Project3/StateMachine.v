`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    17:37:10 02/04/2015 
// Design Name: 
// Module Name:    StateMachine 
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
module StateMachine(
    input Pause,
    input Adjust,
    input Reset,
	 input NormalClock,
	 input AdjustClock,
    output Clock,
	 output ResetSignal
    );
	 
	 reg	State[2:0];
	 reg	ClkSelReg[1:0];
	 reg	ClkReg;
	 reg	ResetReg;
	 
	 always @*
	 begin
		ClkReg = 0;
		ResetReg = 0;
	 
		 if( State == 0 ) //Normal
		 begin
			if( Reset == 1 )
			begin
				ClkReg = NormalClock;
				ResetReg = 1;
				State = 2;
			end
			else if( Pause == 1 )
			begin
				ClkReg = 0;
				State = 1;
			end
			else if( Adjust == 1 )
			begin
				ClkReg = AdjustClock;
				State = 3;
			end
		 end
		 else if( State == 1 ) //Pause
		 begin
			if( Pause == 1 )
			begin
				ClkReg = NormalClock;
				State = 0;
			end
		 end
		 else if( State == 2 ) //Reset
		 begin
			ClkReg = NormalClock;
			State = 0;
		 end
		 else if( State == 3 ) //Adjust
		 begin
			if( Reset == 1 )
			begin
				ClkReg = NormalClock;
				ResetReg = 1;
				State = 2;
			end
			else if( Adjust == 0 )
			begin
				ClkReg = NormalClock;
				State = 0;
			end
			else if( Pause == 1 )
			begin
				ClkReg = 0;
				State = 4;
			end
		 end
		 else if( State == 4 && Pause == 1 ) //AdjustPause
		 begin
			ClkReg = AdjustClock;
			State = 3;
		 end
	end

	assign Clock = ClkReg;
	assign ResetSignal = ResetReg;

endmodule
