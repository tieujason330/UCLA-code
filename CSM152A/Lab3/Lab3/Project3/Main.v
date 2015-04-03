`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    22:01:41 02/17/2015 
// Design Name: 
// Module Name:    Main 
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
module Main(
    input MasterClock,
    input Pause,
	 input Select,
    input Adjust,
    input Reset,
    output [2:0] MinTens,
    output [3:0] MinOnes,
    output [2:0] SecTens,
    output [3:0] SecOnes,
    output [3:0] An,
    output [7:0] C,
	 output [7:0] Led
    );
	 
	wire NormalClock, AdjustClock, FasterClock, BlinkClock;
	wire DebouncedPause, DebouncedAdjust, DebouncedSelect;
	wire Clock; //To connect state machine output to counter
	wire Adjusting;

	ClockSplitter m1(.MasterClock(MasterClock),.Reset(Reset),.NormalClock(NormalClock),.AdjustClock(AdjustClock),.FasterClock(FasterClock),.BlinkClock(BlinkClock));

	Debouncer m2(.FasterClock(FasterClock),.PauseIn(Pause),.AdjustIn(Adjust),.SelectIn(Select),.PauseOut(DebouncedPause),.AdjustOut(DebouncedAdjust),.SelectOut(DebouncedSelect));

	StateMachineModule m3(.MasterClock(MasterClock),.Pause(DebouncedPause),.Adjust(DebouncedAdjust),.Reset(Reset),.NormalClock(NormalClock),.AdjustClock(AdjustClock),.Clock(Clock),.Adjusting(Adjusting));

	CounterModule m4(.Clock(Clock),.Reset(Reset),.Select(DebouncedSelect),.Adjust(DebouncedAdjust),.MinutesTens(MinTens),.MinutesOnes(MinOnes),.SecondsTens(SecTens),.SecondsOnes(SecOnes));

	SevenSegmentDisplay m5(.Clock(FasterClock),.BlinkClock(BlinkClock),.Reset(Reset),.Adjusting(Adjusting),.Select(DebouncedSelect),.MinutesTens(MinTens),.MinutesOnes(MinOnes),.SecondsTens(SecTens),.SecondsOnes(SecOnes),.Anodes(An),.Cathodes(C));

	assign Led = 0;

endmodule
