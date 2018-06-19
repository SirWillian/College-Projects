library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity ula is
	port(	data_in1: in unsigned (15 downto 0);
			data_in2: in unsigned (15 downto 0);
			data_out: buffer unsigned(15 downto 0);
			aluop: in unsigned(1 downto 0);
			zero: out std_logic;
			overflow: out std_logic;
			greater_equal: out std_logic
	);
end entity;

architecture a_ula of ula is
begin
	data_out <= data_in1 + data_in2 when aluop = "00" else
				data_in1 - data_in2 when aluop = "01" else
				data_in1 and data_in2 when aluop = "10" else
				"0000000000000000";
				
	zero <= '1' when (data_in1 - data_in2) = "0000000000000000" else
			'0';
			
	greater_equal <='1' when data_in1(15) = '0' and data_in2(15) = '1' and aluop = "11" else
					'0' when data_in1(15) = '1' and data_in2(15) = '0' and aluop = "11" else
					'1' when data_in1 >= data_in2 and aluop = "11" else
					'0';
					
	overflow <= '1' when 	data_in1(15) = '0' and data_in2(15) = '0' and data_out(15) = '1' and aluop = "00" else
				'1' when	data_in1(15) = '1' and data_in2(15) = '1' and data_out(15) = '0' and aluop = "00" else
				'1' when	data_in1(15) = '0' and data_in2(15) = '1' and data_out(15) = '1' and aluop = "01" else
				'1' when	data_in1(15) = '1' and data_in2(15) = '0' and data_out(15) = '0' and aluop = "01" else
				'0';
end architecture;