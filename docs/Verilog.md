1. `ifdef`/`else`
   如果定义了宏定义`STRING`，则综合工具会编译`code_0`段，若未定义`STRING`，则编译`code_1`段。

```verilog
`ifdef STRING
	code_0;
`else
	code_1;
`endif
```

2. `ifndef`/`else`
   如果定义了宏定义`STRING`，则综合工具会编译`code_1`段，若未定义`STRING`，则编译`code_0`段。

```verilog
`ifndef STRING
	code_0;
`else
	code_1;
`endif
```

