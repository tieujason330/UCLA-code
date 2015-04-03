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
module StateMachineModule(
	 input MasterClock,
    input Pause,
    input Adjust,
    input Reset,
    input NormalClock,
    input AdjustClock,
    output Clock,
	 output Adjusting
    );
	 
	 reg [2:0]	State;
	 reg ClockReg;
	 reg pausing;
	 reg unpausing;
	 reg isAdjusting;
	 
	 initial
	 begin
		State = 0;
		ClockReg = 0;
		pausing = 0;
		unpausing = 0;
	end
	 
	always @ (posedge MasterClock)
	begin
		if( Reset )
		begin
			State = 2;
			ClockReg = 1;
			pausing = 0;
			unpausing = 0;
		end
		else
		begin
			ClockReg = 0;
			
			if( State == 0 ) //Normal
			begin
				isAdjusting = 0;
			
				if( pausing == 1 && Pause == 0 ) //switch into pause mode
				begin
					pausing = 0;
					ClockReg = 0;
					State = 1;
				end
				else
				begin
					if( Pause == 1 ) 
					begin
						pausing = 1;
					end
					
					if( Adjust == 1 ) //switch into adjust mode
					begin
						isAdjusting = 1;
						ClockReg = AdjustClock;
						State = 3;
					end
					else //stay in normal state
					begin
						ClockReg = NormalClock;
						State = 0;
					end
				end
			end
			else
			begin
				if( State == 1 ) //Pause
				begin
					isAdjusting = 0;
				
					if( unpausing && Pause == 0 ) //return to normal state
					begin
						unpausing = 0;
						State = 0;
						ClockReg = NormalClock;
					end
					else //stay in pause state
					begin
						State = 1;
						ClockReg = 0;
						
						if( Pause == 1 )
						begin
							unpausing = 1;
						end
					end
				end
				else
				begin
					if( State == 2 ) //Reset
					begin
						isAdjusting = 0;
						ClockReg = 0;
						State = 0; //return to normal state
					end
					else
					begin
						if( State == 3 ) //Adjust
						begin
							isAdjusting = 1;
						
							if( Adjust == 0 ) //switch back to normal state
							begin
								ClockReg = NormalClock;
								State = 0;
							end
							else
							begin
								if( pausing && Pause == 0 ) //switch into adjust-pause mode
								begin
									ClockReg = 0;
									State = 4;
									pausing = 0;
								end
								else //stay in adjust mode
								begin
									ClockReg = AdjustClock;
									State = 3;
								end
								
								if( Pause == 1 )
								begin
									pausing = 1;
								end
							end
						end
						else
						begin
							if( State == 4 ) //adjust-pause
							begin
								isAdjusting = 0;
							
								if( unpausing && Pause == 0 )
								begin
									unpausing = 0;
									ClockReg = AdjustClock;
									State = 3;
								end
								else //stay in adjust-pause state
								begin
									ClockReg = 0;
									State = 4;
								
									if( Pause == 1 )
									begin
										unpausing = 1;
									end
								end
							end
							else
							begin //bad state. just reset
								State = 2;
								ClockReg = 1;
								pausing = 0;
								unpausing = 0;
							end
						end
					end
				end
			end
		end
	end

	assign Clock = ClockReg;
	assign Adjusting = isAdjusting;

endmodule
