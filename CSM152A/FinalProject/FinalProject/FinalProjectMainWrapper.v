
module FinalProjectMainWrapper(input Clock,
				  input [4:0] Buttons,
				  input [7:0] Switches,
				  output hsync,
				  output vsync,
				  output reg [2:0] RED,
				  output reg [2:0] GREEN,
				  output reg [1:0] BLUE,
				  output	[7:0] Leds,
				  output [3:0] Anodes,
				  output [7:0] Cathodes);
				  
	 wire clk_50mhz;
	 wire DisplayClock;
	 wire GameClock;
	 wire ScoreDisplayClock;
	 
	 wire hsync_out;
	 wire vsync_out;
	 wire vidon;
	 
	 wire [10:0] X;
	 wire [10:0] Y;
	 
	 
	 wire [3:0] ScoreThousands;
	 wire [3:0] ScoreHundreds;
	 wire [3:0] ScoreTens;
	 wire [3:0] ScoreOnes;
	 wire	[3:0] Difficulty;
	 wire	[2:0] R;
	 wire	[2:0] G;
	 wire	[1:0] B;
	 
	 assign hsync = ~hsync_out;
	 assign vsync = ~vsync_out;
	 
	 
	 ClockSplitter clkSplit(.Clock(Clock),.Difficulty(Difficulty),.DisplayClock(DisplayClock),.GameClock(GameClock),.ScoreDisplayClock(ScoreDisplayClock));
	 
	 DifficultySelector diffSel(.Clock(GameClock),.ScoreThousands(ScoreThousands),.ScoreHundreds(ScoreHundreds),.ScoreTens(ScoreTens),.ScoreOnes(ScoreOnes),.Switches(Switches),.Difficulty(Difficulty),.Leds(Leds));
	 
	 Game game(.Clock(GameClock),.X(X),.Y(Y),.Buttons(Buttons),.Difficulty(Difficulty),.ScoreThousands(ScoreThousands),.ScoreHundreds(ScoreHundreds),.ScoreTens(ScoreTens),.ScoreOnes(ScoreOnes),.R(R),.G(G),.B(B));
	 
	 SevenSegmentDisplay scoreDisplay(.Clock(ScoreDisplayClock),.Reset(Buttons[0]),.Thousands(ScoreThousands),.Hundreds(ScoreHundreds),.Tens(ScoreTens),.Ones(ScoreOnes),.Anodes(Anodes),.Cathodes(Cathodes));
	 
	 always @(DisplayClock)
	 begin
		if (vidon)
		begin
			RED = R;
			GREEN = G;
			BLUE = B;
		end
		else
		begin
			RED[2:0] = 3'b000;
			GREEN[2:0] = 3'b000;
			BLUE[1:0] = 2'b00;
		end
	 end
	 
	 vga_800x600 display(.Clock(DisplayClock),.Reset(Buttons[0]),.hsync(hsync_out),.vsync(vsync_out),.X(X),.Y(Y),.vidon(vidon));

endmodule
