library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity rom is
	port( clk : in std_logic;
		  endereco : in unsigned(6 downto 0);
		  dado : out unsigned(16 downto 0)
 );
end entity;

architecture a_rom of rom is
	type mem is array (0 to 127) of unsigned(16 downto 0);
	constant conteudo_rom : mem := (
		-- caso endereco => conteudo
		0 => "00000000001000000",
		1 => "10100000001100100",
		2 => "00000000000000010",
		3 => "00000000000000011",
		4 => "10000000000011111",
		5 => "00000000001000101",
		6 => "11110000001110010",
		7 => "10100000000000100",
		8 => "00000000001010010",
		9 => "00000000000001100",
		10 => "00000000000000001",
		11 => "10100000000000100",
		85 => "10100000000001010",
		101 => "10101010101010101",
		-- abaixo: casos omissos => (zero em todos os bits)
		others => (others=>'0')
		);
begin
	process(clk)
	begin
		if(rising_edge(clk)) then
			dado <= conteudo_rom(to_integer(endereco));
		end if;
	end process;
end architecture;

-- Teste do Jump: Do 1 ele pula pro 100, daí do 101 ele pula pro 85, do 85 pula pro 10, do 11 pro 4 e do 7 pro 4. Após isso, fica em loop do 4 até o 7.