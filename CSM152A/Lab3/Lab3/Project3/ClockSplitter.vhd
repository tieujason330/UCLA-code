----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    17:07:46 02/04/2015 
-- Design Name: 
-- Module Name:    ClockSplitter - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ClockSplitter is
    Port ( MasterClock : in  STD_LOGIC;
           1HzClock : out  STD_LOGIC;
           2HzClock : out  STD_LOGIC;
           FasterClock : out  STD_LOGIC;
           BlinkClock : out  STD_LOGIC);
end ClockSplitter;

architecture Behavioral of ClockSplitter is

begin


end Behavioral;

