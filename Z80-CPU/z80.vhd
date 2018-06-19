library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity z80 is 
	port(	clk 			:	in std_logic;
			rst 			:	in std_logic;
			wr_enable 		:	in std_logic;
			debug_muares_2 	: 	out unsigned (16 downto 0) -- Pino para debugar a saida da rom.
		);
end entity;

architecture a_z80 of z80 is
	component uc 
		port (	data_in  : in unsigned(15 downto 0);
				estado   : in unsigned(1 downto 0);
				instrucao: in unsigned(16 downto 0); -- Vai receber uma instrução de 17 bits vinda da rom.
				data_out : out unsigned(15 downto 0)
		);	
	end component;
	component reg16bits
		port ( 	clk			: in std_logic;
				rst			: in std_logic;
				wr_enable	: in std_logic;
				data_in		: in unsigned(15 downto 0);
				data_out	: out unsigned(15 downto 0)
		);
	end component;
	component rom
		port (	clk : in std_logic;
				endereco : in unsigned(6 downto 0);
				dado : out unsigned(16 downto 0)
		);
	end component;
	component maq_estados
		port(	clk			: in std_logic;
				rst			: in std_logic;
				estado_o    : out unsigned(1 downto 0)
		);
	end component;
	
	component ula
		port(	data_in1: in unsigned (15 downto 0);
				data_in2: in unsigned (15 downto 0);
				data_out: buffer unsigned(15 downto 0);
				aluop: in unsigned(1 downto 0);
				zero: out std_logic;
				overflow: out std_logic;
				greater_equal: out std_logic
		);
	end component;
	
	component regbank
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
	end component;
	
	signal pc_in, pc_out 	: unsigned(15 downto 0);
	signal rom_data			: unsigned(16 downto 0); -- recebe a saida da rom.
	signal butcher			: unsigned(6 downto 0); -- corta a saida do pc
	signal estado_s			: unsigned(1 downto 0);
	
	begin 
		pc: reg16bits port map (clk=>clk, rst=>rst, wr_enable=>wr_enable, data_in=>pc_in, data_out=>pc_out);
		maki_estados: maq_estados port map(clk=>clk, rst=>rst, estado_o=>estado_s);
		butcher <= pc_out(6 downto 0);
		romer : rom port map (clk=>clk, endereco=>butcher, dado=>rom_data);
		unicontrol: uc port map (data_in=>pc_out, estado=>estado_s, data_out=>pc_in, instrucao=>rom_data);
		debug_muares_2 <= rom_data when estado_s = "01" else
						  "00000000000000000"; -- Ler a memória quando o estado do flipflop for 0. (?)
		-- Fazer (talvez) um registrador pra guardar a instrução. Só escreve quando o estado_s = "01". Ai debug_muares_2 <= data_out desse reg
		-- Instanciar ULA e regbank
		-- Criar opcodes. Vai ter q criar pinos na UC
end architecture;