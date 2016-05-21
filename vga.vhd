--
--
--	VGA interface for DE1 simulator
--
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
library work;
use work.de1simulator.all;

entity vga_interface is
	port	 (RED : in std_logic_vector (3 downto 0);
			  GREEN : in std_logic_vector (3 downto 0);
			  BLUE : in std_logic_vector (3 downto 0);
			  HSYNC : in std_logic;
			  VSYNC : in std_logic
			);
end vga_interface;

architecture rtl of vga_interface is
	signal pixel : std_logic_vector (31 downto 0) := x"00000000";
begin

process
	variable dummy : integer;
begin

	wait until VSYNC = '0';
	wait until VSYNC = '1';
	
	for row in 0 to 522 loop

			wait until HSYNC = '0';
			wait until HSYNC = '1';

			for column in 0 to 703 loop

				if column > 48 and row > 33 then -- back porches
					pixel(23 downto 20) <= RED;
					pixel(15 downto 12) <= GREEN;
					pixel(7 downto 4) <= BLUE;

					dummy := de1_set_video_ram(column - 48, row - 33, to_integer(unsigned(pixel)));	
				end if; 		

				if row > 513 then
					 dummy := de1_set_vretrace(1);
				else 
					 dummy := de1_set_vretrace(0);
				end if;

				wait for 40 ns; -- 25 MHz
			
			end loop;

	end loop;

end process;

end rtl;
