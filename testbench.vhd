--
--		Testbench to show off de1 simulator
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

use work.de1simulator.all;

entity de1testbench is
end de1testbench;

architecture rtl of de1testbench is
	signal tb_clk : std_logic;
	signal tb_stop : std_logic := '0';
	signal leds_red : std_logic_vector(9 downto 0) := "1111111111";
	signal leds_green : std_logic_vector(7 downto 0) := "11111111";
	signal keys : std_logic_vector (3 downto 0);
	signal switches : std_logic_vector ( 9 downto 0);
	signal clk_stop: std_logic := '0';
	signal hex_displays : std_logic_vector (31 downto 0);
	signal tb_counter : std_logic_vector (3 downto 0) := "0000";

	signal vga_red : std_logic_vector (3 downto 0);
	signal vga_green : std_logic_vector (3 downto 0);
	signal vga_blue : std_logic_vector (3 downto 0);
	signal vga_hsync : std_logic;
	signal vga_vsync : std_logic;

	component hexdriver
		port (
		input : in std_logic_vector(3 downto 0);
		output : out std_logic_vector(7 downto 0)
	);
	end component;
 
begin

hd0:	hexdriver port map (input => tb_counter, 
							output => hex_displays(7 downto 0));

clk0:	entity work.rt_clk
	generic map(ms => 100)
	port map (clk => tb_clk, stop => clk_stop);

de1_0:	entity work.de1
	port map (LED_RED => leds_red, LED_GREEN => leds_green, KEY => keys, 
			  SWITCH => switches, HEX => hex_displays, VGA_R => vga_red, 
			  VGA_G => vga_green, VGA_B => vga_blue, VGA_HSYNC => vga_hsync, 
			  VGA_VSYNC => vga_vsync, 
			  simulator_exit => tb_stop  );

pr0:	process (tb_clk)
	begin

		if rising_edge(tb_clk) then
			leds_green(0) <= not leds_green(0);

			tb_counter <= std_logic_vector(unsigned(tb_counter) + 1);

		end if;

	end process;

pr1: 	process (tb_stop)
	begin
	if rising_edge(tb_stop) then
		clk_stop <= '1';
	end if;
	end process;

vga0:	process
	begin

		for row in 0 to 524 loop

			for column in 0 to 799 loop
		
				if column > 16 and column < 96 + 16 then -- Hsync pulse
					vga_hsync <= '0';
				else
					vga_hsync <= '1';
				end if;

				if row > 10 and row < 10 + 2 then -- Vsync pulse
					vga_vsync <= '0';
				else
					vga_vsync <= '1';
				end if;

				if (column / 2) mod 3 = 0 then
					vga_red <= "1111";
				else
					vga_red <= "0000";
				end if;
	
				if (column / 2) mod 3 = 1 then
					vga_green <= "1111";
				else
					vga_green <= "0000";
				end if;
		
				if (column / 2) mod 3 = 2 then
					vga_blue <= "1111";
				else
					vga_blue <= "0000";
				end if;

				-- exit simulator?
			
				wait for 40 ns;

			end loop;

		end loop;	

		wait;

	end process;

leds_red <= switches;
leds_green(4 downto 1) <= keys;

end rtl;
