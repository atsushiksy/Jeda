library verilog;
use verilog.vl_types.all;
entity utopia1_atm_tx is
    port(
        clk             : out    vl_logic;
        data            : out    vl_logic_vector(7 downto 0);
        soc             : out    vl_logic;
        en              : out    vl_logic;
        clav            : in     vl_logic
    );
end utopia1_atm_tx;
