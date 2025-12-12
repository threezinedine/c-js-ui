#include "cju/cju.h"

int main(void)
{
	cuConsoleSetColor(CU_CONSOLE_COLOR_BLUE);
	cuConsolePrintLn("Hello, Console With blue!");

	cuConsoleSetColor(CU_CONSOLE_COLOR_GREEN);
	cuConsolePrintLn("Hello, Console With green!");

	cuConsoleSetColor(CU_CONSOLE_COLOR_RED);
	cuConsolePrintLn("Hello, Console With red!");

	CuConsoleConfig config;
	config.textColor = CU_CONSOLE_COLOR_YELLOW;
	cuConsoleConfig(config);
	cuConsolePrintLn("Hello, Console With yellow via config!");

	config.textColor = CU_CONSOLE_COLOR_RESET;
	cuConsoleConfig(config);
	cuConsolePrintLn("Back to default color.");

	return 0;
}