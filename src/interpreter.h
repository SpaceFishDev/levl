void InterpretLevl(const char* source, thread* T)
{
	std::vector<std::string> symbols;
	std::string temp = "";

	while(*source != '\0')
	{
		char c = *source;
		if(c == ' ' || c == '\t' || c == '\n')
		{
			symbols.push_back(temp);
			temp = "";
		}
		else
		{
			temp += c;
		}
		++source;
	}
	symbols.push_back(temp);
	int i = 0;
	std::vector<int> tokens;
	bool store = false;
	bool push = false;
	bool add = false;
	bool jmp = false;
	int indx = 0;
	int cmpind = 0;
	int puts_ind = 0;
	bool cmp = false;
	bool puts = false;
	int prptr = 0;
	for(std::string symbol : symbols)
	{
		if(symbol == "store")
		{
			tokens.push_back(0x1);
			store = true;
		}
		else if(symbol == "push")
		{
			tokens.push_back(0x2);
			push = true;
		}else if(symbol == "print")
		{
			tokens.push_back(0x9);
		}
		else if(symbol == "set")
		{
			tokens.push_back(PUSH_UD);
			tokens.push_back(std::stoi(symbols[indx + 2]));
			tokens.push_back(STORE);
			tokens.push_back(std::stoi(symbols[indx + 1]));
		}
		else if(symbol == "add")
		{
			tokens.push_back(ADD);
			add = true;
		}
		else if(symbol == "jmp")
		{
			tokens.push_back(JMP);
			jmp = true;
		}else if(symbol == "cmp")
		{
			tokens.push_back(CMP);
			cmp = true;
		}else if(symbol == "puts")
		{
			tokens.push_back(PUTS);
			puts = true;
		}
		else
		{
			if(store || push || add)
			{
				int index = std::stoi(symbol);
				tokens.push_back(index);
				push = false;
				store = false;	
				add = false;
			}else if(jmp)
			{
				int index = std::stoi(symbol);
				tokens.push_back(index);
				jmp = false;
			}else if(cmp)
			{
				if(cmpind < 3)
				{
					int index = std::stoi(symbol);
					tokens.push_back(index);
					++cmpind;
				}else
				{
					cmp = false;
					cmpind = 0;
				}
			}else if(puts)
			{
				if(puts_ind < 3)
				{
					int index = std::stoi(symbol);
					tokens.push_back(index);
					++puts_ind;
				}else
				{
					puts = false;
					puts_ind = 0;
				}
			}
		}
		++indx;
	}
	for(int token : tokens)
	{
		set_ins(i, token, T);
		++i;
	}
}