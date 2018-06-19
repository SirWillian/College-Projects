library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity uc is
	port(	data_in : in unsigned(15 downto 0);
			estado  : in unsigned(1 downto 0);
			instrucao: in unsigned(16 downto 0);
			data_out : out unsigned(15 downto 0)
	);
end entity;

architecture a_uc of uc is
	signal data_signal : unsigned(15 downto 0);
	signal opcode: unsigned(3 downto 0);
	signal jump_en  : std_logic;
begin
	opcode <= instrucao(16 downto 13);
	jump_en <= '1' when opcode="1010" else
			   '0';
	data_signal <= data_in + "0000000000000001" when estado = "01" else
				   data_in;
	data_out <= data_signal when jump_en = '0' else
				data_in(15 downto 13)& instrucao(12 downto 0);
	
end architecture;
	