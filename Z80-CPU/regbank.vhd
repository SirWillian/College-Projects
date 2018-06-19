library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity regbank is
	port(	read_reg_a	: in unsigned(2 downto 0);
			read_reg_b	: in unsigned(2 downto 0);
			write_reg	: in unsigned(2 downto 0);
			data_in		: in unsigned(15 downto 0);
			write_enable	: in std_logic;
			clock			: in std_logic;
			reset			: in std_logic;
			reg_data_a	: out unsigned(15 downto 0);
			reg_data_b	: out unsigned(15 downto 0)
	);
end entity;

architecture a_regbank of regbank is
	component reg16bits is
		port(	clk			: in std_logic;
				rst			: in std_logic;
				wr_enable	: in std_logic;
				data_in		: in unsigned(15 downto 0);
				data_out	: out unsigned(15 downto 0)
		);
	end component;
	signal data_in0, data_in1, data_in2, data_in3, data_in4, data_in5, data_in6, data_in7: unsigned(15 downto 0);
	signal wr_enable0, wr_enable1, wr_enable2, wr_enable3, wr_enable4, wr_enable5, wr_enable6, wr_enable7, rst0 : std_logic; 
	signal data_out0, data_out1, data_out2, data_out3, data_out4, data_out5, data_out6, data_out7 : unsigned(15 downto 0);
begin
	reg0: reg16bits port map(clk=>clock, rst=>rst0, wr_enable=>wr_enable0, data_in=>data_in0, data_out=>data_out0);
	regB: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable1, data_in=>data_in1, data_out=>data_out1);
	regC: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable2, data_in=>data_in2, data_out=>data_out2);
	regD: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable3, data_in=>data_in3, data_out=>data_out3);
	regE: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable4, data_in=>data_in4, data_out=>data_out4);
	regG: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable5, data_in=>data_in5, data_out=>data_out5);
	regHL: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable6, data_in=>data_in6, data_out=>data_out6);
	regA: reg16bits port map(clk=>clock, rst=>reset, wr_enable=>wr_enable7, data_in=>data_in7, data_out=>data_out7);
	
	data_in0 <= data_in when write_reg = "000" else "0000000000000000";
	data_in1 <= data_in when write_reg = "001" else "0000000000000000";
	data_in2 <= data_in when write_reg = "010" else "0000000000000000";
	data_in3 <= data_in when write_reg = "011" else "0000000000000000";
	data_in4 <= data_in when write_reg = "100" else "0000000000000000";
	data_in5 <= data_in when write_reg = "101" else "0000000000000000";
	data_in6 <= data_in when write_reg = "110" else "0000000000000000";
	data_in7 <= data_in when write_reg = "111" else "0000000000000000";
	
	wr_enable0 <= write_enable when write_reg = "000" else '0';
	wr_enable1 <= write_enable when write_reg = "001" else '0';
	wr_enable2 <= write_enable when write_reg = "010" else '0';
	wr_enable3 <= write_enable when write_reg = "011" else '0';
	wr_enable4 <= write_enable when write_reg = "100" else '0';
	wr_enable5 <= write_enable when write_reg = "101" else '0';
	wr_enable6 <= write_enable when write_reg = "110" else '0';
	wr_enable7 <= write_enable when write_reg = "111" else '0';
	
	reg_data_a <= data_out0 when read_reg_a = "000" else 
	              data_out1 when read_reg_a = "001" else
				  data_out2 when read_reg_a = "010" else 
				  data_out3 when read_reg_a = "011" else 
				  data_out4 when read_reg_a = "100" else 
				  data_out5 when read_reg_a = "101" else 
				  data_out6 when read_reg_a = "110" else 
				  data_out7 when read_reg_a = "111" else "0000000000000000";
				  
	reg_data_b <= data_out0 when read_reg_b = "000" else 
	              data_out1 when read_reg_b = "001" else
				  data_out2 when read_reg_b = "010" else 
				  data_out3 when read_reg_b = "011" else 
				  data_out4 when read_reg_b = "100" else 
				  data_out5 when read_reg_b = "101" else 
				  data_out6 when read_reg_b = "110" else 
				  data_out7 when read_reg_b = "111" else "0000000000000000";

	rst0 <= '1';
	
end architecture;
	
	
	
	
	
	