
module vga_800x600(input wire Clock,
input wire Reset,
output reg hsync,
output reg vsync,
output reg [10:0] X,
output reg [10:0] Y,
output reg vidon
    );


parameter TotalHorizontalPixels = 11'd1040;
parameter HorizontalSyncWidth = 11'd120;
parameter VerticalSyncWidth = 11'd6;

parameter TotalVerticalLines = 11'd666;
parameter HorizontalBackPorchTime = 11'd184 ;
parameter HorizontalFrontPorchTime = 11'd984 ;
parameter VerticalBackPorchTime = 11'd43 ;
parameter VerticalFrontPorchTime = 11'd643;

reg VerticalSyncEnable;

reg [10:0] HorizontalCounter;
reg [10:0] VerticalCounter;

//Counter for the horizontal and vertical sync signal
always @(posedge Clock)
begin
	if( Reset == 1 )
    begin
		HorizontalCounter <= 0;
        VerticalCounter<=0;
    end
	else
    begin
        if( HorizontalCounter == TotalHorizontalPixels - 1 )
        begin //the counter has reached the end of a horizontal line
            HorizontalCounter<=0;
            VerticalSyncEnable <= 1;
        end
        else
        begin 
            HorizontalCounter<=HorizontalCounter+1; 
            VerticalSyncEnable <=0;
        end
        
        if( VerticalSyncEnable == 1 )
        begin
            if( VerticalCounter == TotalVerticalLines - 1 )
            begin
                VerticalCounter<=0;
            end
            else
            begin
                VerticalCounter<=VerticalCounter+1;
            end
        end
    end
end

//Generate the hsync pulse and vsync pulse
//Horizontal Sync is low when HorizontalCounter is 0-127

always @(*)
begin
	if( HorizontalCounter < HorizontalSyncWidth )
    begin
		hsync = 1;
    end
	else
    begin
		hsync = 0;
    end
        
    if( VerticalCounter < VerticalSyncWidth )
    begin
		vsync = 1;
    end
	else
    begin
		vsync = 0;
    end
end



always @(posedge Clock)
begin
	if( HorizontalCounter < HorizontalFrontPorchTime && 
        HorizontalCounter > HorizontalBackPorchTime && 
        VerticalCounter < VerticalFrontPorchTime && 
        VerticalCounter > VerticalBackPorchTime )
    begin
        vidon <= 1;
        X<= HorizontalCounter - HorizontalBackPorchTime;
        Y<= VerticalCounter - VerticalBackPorchTime;
    end
	else
    begin
        vidon <= 0;
        X<=0;
        Y<=0;
    end
end

endmodule
