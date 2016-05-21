
package de1simulator is 

	function de1_init(dummy: integer) return integer;
	attribute foreign of de1_init : function is "VHPIDIRECT init";

	function de1_shutdown(dummy: integer) return integer;
	attribute foreign of de1_shutdown : function is "VHPIDIRECT shutdown";

	function de1_ui_has_exited(dummy: integer) return integer;
	attribute foreign of de1_ui_has_exited : function is "VHPIDIRECT ui_exited";

	function de1_set_led(red_green: integer; led: integer; on_off: string) return integer;
	attribute foreign of de1_set_led : function is "VHPIDIRECT set_led";   

	function de1_get_slide_switch(switch: integer) return integer;
	attribute foreign of de1_get_slide_switch: function is "VHPIDIRECT get_slide_switch";

	function de1_get_key(key: integer) return integer;
	attribute foreign of de1_get_key: function is "VHPIDIRECT get_key";	

	function de1_set_display_segment(display: integer; segment: integer; on_off: string) return integer;
	attribute foreign of de1_set_display_segment: function is "VHPIDIRECT set_display_segment"; 

	function de1_set_video_ram(x: integer; y: integer; p: integer) return integer;
	attribute foreign of de1_set_video_ram: function is "VHPIDIRECT set_video_ram";

	function de1_set_vretrace(val: integer) return integer;
	attribute foreign of de1_set_vretrace: function is "VHPIDIRECT set_vretrace";			
end de1simulator; 

package body de1simulator is

	function de1_init (dummy: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_init;

	function de1_shutdown (dummy: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_shutdown;

	function de1_ui_has_exited (dummy: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_ui_has_exited;

	function de1_set_led (red_green: integer; led: integer; on_off: string) 
			return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_set_led;

	function de1_get_slide_switch(switch: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_get_slide_switch;

	function de1_get_key(key: integer) return integer is 
	begin
			assert false report "VHPI" severity failure;
	end de1_get_key;

	function de1_set_display_segment(display: integer; segment: integer; on_off: string) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_set_display_segment;

	function de1_set_video_ram(x: integer; y: integer; p: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_set_video_ram;

	function de1_set_vretrace(val: integer) return integer is
	begin
			assert false report "VHPI" severity failure;
	end de1_set_vretrace;

end de1simulator; 

--
--	DE1 board top level
--

library ieee;
use ieee.std_logic_1164.all;
library work;
use work.de1simulator.all;

entity de1 is
	port (
		LED_RED : in std_logic_vector (9 downto 0);
		LED_GREEN : in std_logic_vector (7 downto 0);
		KEY : out std_logic_vector (3 downto 0);
		SWITCH : out std_logic_vector (9 downto 0);
		HEX : in std_logic_vector(31 downto 0);
	
		-- VGA interface	
		VGA_R: in std_logic_vector (3 downto 0);
		VGA_G: in std_logic_vector (3 downto 0);
		VGA_B: in std_logic_vector (3 downto 0);
		VGA_HSYNC: in std_logic;
		VGA_VSYNC: in std_logic;

		simulator_exit : out std_logic
	);
end de1;

architecture rtl of de1
is
	signal switches : std_logic_vector (9 downto 0);
	signal keys : std_logic_vector (3 downto 0);
	signal exit_sig : std_logic := '0';

	component vga_interface
		port (RED : in std_logic_vector (3 downto 0);
			  GREEN : in std_logic_vector (3 downto 0);
			  BLUE : in std_logic_vector (3 downto 0);
			  HSYNC : in std_logic;
			  VSYNC : in std_logic
			);
	end component;

begin

vga0: vga_interface port map (RED => VGA_R, GREEN => VGA_G, BLUE => VGA_B,
							  HSYNC => VGA_HSYNC, VSYNC => VGA_VSYNC);
	
pr0: process
	variable dummy : integer;
	begin

	dummy := de1_init(0);

	while exit_sig = '0' loop
		
		-- get slide switches

		for i in switches'range loop
			if de1_get_slide_switch(i) = 1 then
				switches(i) <= '1';
			else
				switches(i) <= '0';
			end if;
		end loop;

		-- get keys

		for i in keys'range loop
			if de1_get_key(i) = 1 then
				keys(i) <= '1';
			else
				keys(i) <= '0';
			end if;
		end loop;

		-- set LEDs

		for i in LED_RED'range loop
			dummy := de1_set_led(0, i, std_logic'image (LED_RED(i))); 
		end loop;

		for i in LED_GREEN'range loop
			dummy := de1_set_led(1, i, std_logic'image (LED_GREEN(i))); 
		end loop;

		-- set hex displays

		for i in 0 to 3 loop
			for j in 0 to 7 loop

				dummy := de1_set_display_segment(i, j, std_logic'image (HEX(i*8 + j)));
			end loop;
		end loop;		

		dummy := de1_ui_has_exited(0);
		if dummy = 1 then
			exit_sig <= '1';
		end if;
		wait for 1 ms;
	end loop;

	dummy := de1_shutdown(0);
	wait;
	end process;

SWITCH <= switches;
KEY    <= keys;
simulator_exit <= exit_sig;

end rtl;

