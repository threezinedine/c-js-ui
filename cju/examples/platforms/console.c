#include "cju/cju.h"

int main(void)
{
	cuConsoleSetColor(CU_CONSOLE_COLOR_BLUE);
	cuPrintLn("Hello, Console With blue!");

	cuConsoleSetColor(CU_CONSOLE_COLOR_GREEN);
	cuPrintLn("Hello, Console With green!");

	cuConsoleSetColor(CU_CONSOLE_COLOR_RED);
	cuPrintLn("Hello, Console With red!");

	CuConsoleConfig config;
	config.textColor = CU_CONSOLE_COLOR_YELLOW;
	cuConsoleConfig(config);
	cuPrintLn("Hello, Console With yellow via config!");

	config.textColor = CU_CONSOLE_COLOR_RESET;
	cuConsoleConfig(config);
	cuPrintLn("Back to default color.");

	return 0;
}