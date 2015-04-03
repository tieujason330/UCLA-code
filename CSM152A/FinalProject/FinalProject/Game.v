`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    16:39:06 02/25/2015 
// Design Name: 
// Module Name:    Game 
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
module Game(
    input Clock,
    input [10:0] X,
    input [10:0] Y,
    input [4:0] Buttons,
	 input [3:0] Difficulty,
    output [3:0] ScoreThousands,
	 output [3:0] ScoreHundreds,
	 output [3:0] ScoreTens,
	 output [3:0] ScoreOnes,
    output [2:0] R,
    output [2:0] G,
    output [1:0] B
    );
	 
	 parameter SquareSize = 20;
	 
	 parameter initialX = SquareSize * 3;
	 parameter initialY = SquareSize * 3;
     parameter MAXTAIL = 10;
	 
	 parameter initialFoodX = SquareSize * 15;
	 parameter initialFoodY = SquareSize * 15;
	 parameter foodScore = 5;
	 
	 parameter RIGHT = 0;
	 parameter DOWN = 1;
	 parameter LEFT = 2;
	 parameter UP = 3;
	 parameter RIGHTLIMIT = 800;
	 parameter DOWNLIMIT = 600;
	 parameter LEFTLIMIT = 0;
	 parameter UPLIMIT = 0;

	 reg	GameOver;

	 reg	[10:0] snakeHeadX;
	 reg	[10:0] snakeHeadY;
	 
	 reg	[3:0] numActiveBodyPieces;
	 reg	[10:0] snakeBodyPiecesX [9:0];
	 reg	[10:0] snakeBodyPiecesY [9:0];
	 reg	[10:0] tailToAddX;
	 reg	[10:0] tailToAddY;
	 reg	addPiece;
	 reg	[3:0] pieceIndex;
	 reg    paintBody;
     

	 reg	[2:0] red;
	 reg	[2:0] green;
	 reg	[1:0] blue;
	 
	 reg	[3:0] scoreThou;
	 reg	[3:0] scoreHund;
	 reg	[3:0] scoreTen;
	 reg	[3:0] scoreOne;
	 
	 
	 reg	[1:0] direction;
	 reg	[1:0] nextDirection;
     reg    [1:0] tailDirections [9:0];
	 
	 reg	[10:0] foodX;
	 reg	[10:0] foodY;
	 
	 //handle snake movement
	 always @(posedge Clock)
	 begin
		//--------------------------------Reset--------------------------------//
		if( Buttons[0] == 1 )
		begin
			snakeHeadX = initialX;
			snakeHeadY = initialY;
			
			foodX = initialFoodX;
			foodY = initialFoodY;
			
			scoreThou = 0;
			scoreHund = 0;
			scoreTen = 0;
			scoreOne = 0;
			
			addPiece = 0;
			
			direction = RIGHT;
			nextDirection = RIGHT;
			
			numActiveBodyPieces = 0;
			
			GameOver = 0;
		end
		//--------------------------------End Reset--------------------------------//
		
		if( GameOver == 0 )
		begin
			//--------------------------------Move Snake--------------------------------//
			//Move tail
			if( numActiveBodyPieces > 0 )
			begin
                for( pieceIndex = 0; pieceIndex < numActiveBodyPieces && pieceIndex < MAXTAIL; pieceIndex = pieceIndex + 1 )
                begin
                    if( tailDirections[pieceIndex] == RIGHT )
                    begin
                        snakeBodyPiecesX[pieceIndex] = snakeBodyPiecesX[pieceIndex] + 1;
                    end
                    else if( tailDirections[pieceIndex] == DOWN )
                    begin
                        snakeBodyPiecesY[pieceIndex] = snakeBodyPiecesY[pieceIndex] + 1;
                    end
                    else if( tailDirections[pieceIndex] == LEFT )
                    begin
                        snakeBodyPiecesX[pieceIndex] = snakeBodyPiecesX[pieceIndex] - 1;
                    end
                    else
                    begin
                        snakeBodyPiecesY[pieceIndex] = snakeBodyPiecesY[pieceIndex] - 1;
                    end
                end
            end
            
            //Move head
			if( direction == RIGHT )
			begin
				snakeHeadX = snakeHeadX + 1;
			end
			else if( direction == DOWN )
			begin
				snakeHeadY = snakeHeadY + 1;
			end
			else if( direction == LEFT )
			begin
				snakeHeadX = snakeHeadX - 1;
			end
			else
			begin
				snakeHeadY = snakeHeadY - 1;
			end
			
			//Change direction once snake head is in grid space
			if( (snakeHeadX - SquareSize) % (SquareSize * 2) == 0 &&
				 (snakeHeadY - SquareSize) % (SquareSize * 2) == 0 )
			begin
                //Change all tail pieces' directions
                if( numActiveBodyPieces > 0 )
                begin
                    for( pieceIndex = 0; pieceIndex < MAXTAIL - 1; pieceIndex = pieceIndex + 1 )
                    begin
                        if( MAXTAIL - pieceIndex > 0 )
                        begin
                            tailDirections[MAXTAIL - pieceIndex - 1] = tailDirections[MAXTAIL - pieceIndex - 2];
                        end
                    end
                end
                tailDirections[0] = direction;
            
				direction = nextDirection;
				
				//if we're supposed to add a piece, add it here.
				if( addPiece == 1 )
				begin
					addPiece = 0;
					numActiveBodyPieces = numActiveBodyPieces + 1;
					snakeBodyPiecesX[numActiveBodyPieces - 1] = tailToAddX;
					snakeBodyPiecesY[numActiveBodyPieces - 1] = tailToAddY;
				end
				
				//Store position for next piece to add
				if( numActiveBodyPieces == 0 )
				begin
					tailToAddX = snakeHeadX;
					tailToAddY = snakeHeadY;
				end
				else
				begin
					tailToAddX = snakeBodyPiecesX[numActiveBodyPieces-1];
					tailToAddY = snakeBodyPiecesY[numActiveBodyPieces-1];
				end
                
                
                //Check for the snake colliding with its body
                for( pieceIndex = 0; pieceIndex < numActiveBodyPieces && pieceIndex < MAXTAIL; pieceIndex = pieceIndex + 1 )
                begin
                    if( snakeHeadX == snakeBodyPiecesX[pieceIndex] &&
                        snakeHeadY == snakeBodyPiecesY[pieceIndex] )
                    begin
                        GameOver = 1;
                    end
                end
			end
			//--------------------------------End Move Snake--------------------------------//
			
			//--------------------------------Check for collisions--------------------------------//
			//Death
			if( snakeHeadX > RIGHTLIMIT - SquareSize || 
				 snakeHeadX < LEFTLIMIT + SquareSize || 
				 snakeHeadY > DOWNLIMIT - SquareSize || 
				 snakeHeadY < UPLIMIT + SquareSize )
			begin
				GameOver = 1;
			end
			
			//Food
			if( snakeHeadX == foodX &&
				 snakeHeadY == foodY )
			begin
				//Add a body part
                if( numActiveBodyPieces < MAXTAIL )
                begin
                    addPiece = 1;
                end
			
				//Increment Score
				scoreOne = scoreOne + foodScore;
				
				if( scoreOne > 9 ) //overflow
				begin
					scoreOne = scoreOne - 10;
					
					if( scoreTen == 9 ) //overflow
					begin
						scoreTen = 0;
						
						if( scoreHund == 9 ) //overflow
						begin
							scoreHund = 0;
							
							if( scoreThou == 9 ) //overflow
							begin
								scoreThou = 0;
							end
							else
							begin
								scoreThou = scoreThou + 1;
							end
						end
						else
						begin
							scoreHund = scoreHund + 1;
						end
					end
					else
					begin
						scoreTen = scoreTen + 1;
					end
				end
				
				//Maybe check if we're trying to put it on a piece of the snake. Then recompute
				foodX = (foodX + SquareSize * 16) % RIGHTLIMIT;
				foodY = (foodY + SquareSize * 22) % DOWNLIMIT;
			end
			
			//--------------------------------End Check for collisions--------------------------------//
			
			
			//--------------------------------Respond to Buttons--------------------------------//
			if( Buttons[4] == 1 )
			begin
				if( direction == UP || direction == DOWN )
				begin
					nextDirection = RIGHT;
				end
			end
			else if( Buttons[2] == 1 )
			begin
				if( direction == UP || direction == DOWN )
				begin
					nextDirection = LEFT;
				end
			end
			else if( Buttons[1] == 1 )
			begin
				if( direction == LEFT || direction == RIGHT )
				begin
					nextDirection = UP;
				end
			end
			else if( Buttons[3] == 1 )
			begin
				if( direction == LEFT || direction == RIGHT )
				begin
					nextDirection = DOWN;
				end
			end
			//--------------------------------End Respond to Buttons--------------------------------//
		 end
	 end
		 
	 //Choose color to display
	 always @(*)
	 begin
		//If we should be painting snake
        //Check snake body
        paintBody = 0;
        if( numActiveBodyPieces > 0 )
        begin
            for( pieceIndex = 0; pieceIndex < numActiveBodyPieces && pieceIndex < MAXTAIL; pieceIndex = pieceIndex + 1 )
            begin
                if( X <= snakeBodyPiecesX[pieceIndex] + SquareSize && X + SquareSize >= snakeBodyPiecesX[pieceIndex] &&
                    Y <= snakeBodyPiecesY[pieceIndex] + SquareSize && Y + SquareSize >= snakeBodyPiecesY[pieceIndex] )
                begin
                    paintBody = 1;
                end
            end
        end
        
        //Check snake head
		if( X <= snakeHeadX + SquareSize && X + SquareSize >= snakeHeadX &&
			 Y <= snakeHeadY + SquareSize && Y + SquareSize >= snakeHeadY )
		begin
			red = 3'b111;
			green = 3'b111;
			blue = 2'b11;
		end
        else if( paintBody )
        begin
            red = 3'b111;
			green = 3'b111;
			blue = 2'b11;
        end
		
		//Paint grid lines
		else if( X % (SquareSize*2) == 0 )
		begin
			red = 3'b111;
			green = 3'b000;
			blue = 2'b00;
		end
		else if( Y % (SquareSize*2) == 0 )
		begin
			red = 3'b111;
			green = 3'b000;
			blue = 2'b00;
		end
	 
		//If we should be painting food
		else if( X <= foodX + SquareSize && X + SquareSize >= foodX &&
					Y <= foodY + SquareSize && Y + SquareSize >= foodY )
		begin
			red = 3'b111;
			green = 3'b000;
			blue = 2'b11;
		end
		
		//Otherwise, we're painting background
		else
		begin
			 if( Difficulty == 8 )
			 begin
				red = 3'b100;
				green = 3'b011;
				blue = 2'b01;
			 end
			 else if( Difficulty == 7 )
			 begin
				red = 3'b011;
				green = 3'b011;
				blue = 2'b01;
			 end
			 else if( Difficulty == 6 )
			 begin
				red = 3'b010;
				green = 3'b010;
				blue = 2'b11;
			 end
			 else if( Difficulty == 5 )
			 begin
				red = 3'b101;
				green = 3'b001;
				blue = 2'b01;
			 end
			 else if( Difficulty == 4 )
			 begin
				red = 3'b000;
				green = 3'b001;
				blue = 2'b01;
			 end
			 else if( Difficulty == 3 )
			 begin
				red = 3'b011;
				green = 3'b001;
				blue = 2'b01;
			 end
			 else if( Difficulty == 2 )
			 begin
				red = 3'b000;
				green = 3'b001;
				blue = 2'b10;
			 end
			 else //Difficulty = 1
			 begin
				red = 3'b000;
				green = 3'b110;
				blue = 2'b11;
			 end
		 end
	end
	 
	assign R = red;
	assign G = green;
	assign B = blue;
	
	assign ScoreThousands = scoreThou;
	assign ScoreHundreds = scoreHund;
	assign ScoreTens = scoreTen;
	assign ScoreOnes = scoreOne;
	
endmodule
