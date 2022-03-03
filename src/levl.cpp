#include<iostream>
#include<chrono>
#include <vector>
enum instructions
{
	STORE = 0x1,
	PUSH = 0x2,
	ADD = 0x3,
	SUB = 0x4,
	MUL = 0x5,
	DIV = 0x6,
	JMP = 0x7,
	CMP = 0x8,
	PRINT = 0x9,
	REG = 16512,
	PUSH_UD = 0x10,
	MOD = 0x11,
	PUTS = 0x12

};

struct thread
{
	int program_ptr = 0;
	int* mem;
	int* ins;
	int mem_size = 0;
	int ins_size = 0;
	int reg = 0;
	bool ini = false;
};

void set_mem(long long int index, long long value, thread* t);
void set_ins(long long int index, long long value, thread* t);

thread init()
{
	thread T;
	T.mem = new int[1000];
	T.ins = new int[1000];
	int i = 0;
	while(i != 1000)
	{
		T.mem[i] = 0;
		T.ins[i] = 0;
		++i;
	}
	T.mem_size = 1000;
	T.ins_size = 1000;
	set_mem(REG, 0, &T);
	return T;
}

void set_mem(long long int index, long long value, thread* t)
{

	if(index > t->mem_size)
	{
		int* temp = t->mem;
		t->mem = new int[t->mem_size*10];
		int i = 0;
		while(i != t->mem_size + 1)
		{
			t->mem[i] = temp[i];
			++i;
		}
		t->mem_size *= 10;
		set_mem(index, value, t);
	}
	else
	{
		t->mem[index] = value;		
	}
}
void set_ins(long long int index, long long value, thread* t)
{

	if(index > t->ins_size)
	{

		int* temp = t->ins;
		t->ins = new int[t->ins_size*10];
		int i = 0;
		while(i != t->ins_size + 1)
		{
			t->ins[i] = temp[i];
			++i;
		}
		t->ins_size *= 10;
		set_mem(index, value, t);
	}
	else
	{
		t->ins[index] = value;		
	}
}

void set_ins_by_str(const char* str, thread* T)
{
	std::vector<std::string> Symbols;
	std::string temp = "";
	while(*str != 0)
	{
		if(*str == ' ' || *str == ',' || *str == '|')
		{
			if(temp != "")
			{
				Symbols.push_back(temp.c_str());
			}
			temp = "";
		}
		else
		{
			if(*str != ' ')
			{
				temp += *str;
			}
		}
		++str;
	}
	Symbols.push_back(temp.c_str());
	int i = 0;
	for(std::string chr : Symbols)
	{
		int val = std::stoi(chr);	
		set_ins(i, val, &*T);
		++i;
	}
}

void execute_ins(thread* T)
{
	int ptr = T->program_ptr;
	switch(T->ins[ptr]){
		case STORE:
		{
			int val = T->ins[ptr + 1];
			set_mem(val, T->reg, &*T);
			T->program_ptr += 2;
			break;
		}
		case PUSH:
		{
			int ptr_push = T->ins[ptr + 1];
			T->reg = T->mem[ptr_push];
			T->program_ptr += 2;
			break;
		}
		case PUSH_UD:
		{
			int num = T->ins[ptr + 1];
			T->reg = num;
			T->program_ptr += 2;
			break;
		}
		case ADD:
		{
			int ptr_add = T->ins[ptr + 1];
			T->reg += T->mem[ptr_add];
			T->program_ptr += 2;
			break;
		}
		case SUB:
		{
			int ptr_sub = T->ins[ptr + 1];
			T->reg -= T->mem[ptr_sub];
			T->program_ptr += 2;
			break;
		}
		case MUL:
		{
			int ptr_mul = T->ins[ptr + 1];
			T->reg *= T->mem[ptr_mul];
			T->program_ptr += 2;
			break;
		}
		case DIV:
		{	
			int ptr_div = T->ins[ptr + 1];
			T->reg /= T->mem[ptr_div];
			T->program_ptr += 2;
			break;
		}
		case JMP:
		{
			int jmp_point = T->ins[ptr + 1];
			T->program_ptr = jmp_point;
			break;
		}
		case CMP:
		{
			int ptr1 = T->ins[T->program_ptr + 1];
			int val = T->mem[ptr1];
			int cmpval = T->reg;
			int ins = T->ins[T->program_ptr + 2];
			int insc = T->ins[T->program_ptr + 3];
			if(cmpval == val)
			{
				T->program_ptr = ins;
			}
			else
			{
				T->program_ptr = insc;
			}
			break;
		}
		case PRINT:
		{
			std::cout << T->reg << "\n";
			T->program_ptr += 1;
			break;
		}
		case MOD:
		{
			int ptr_mod = T->ins[ptr + 1];
			int reg_temp = T->reg;
			T->reg = T->mem[ptr_mod]%reg_temp;
			T->program_ptr += 2;
			break;
		}
		case PUTS:
		{
			int start = T->ins[ptr + 1];
			int end = T->ins[ptr + 2];
			char temp;
			std::string str = "";
			int i = start;
			while(i != end + 1)
			{
				temp = (char)T->mem[i];
				std::cout << temp;
				++i;
			}
			std::cout << str << "\n";
			T->program_ptr += 3;
			break;
		}
	}
	set_mem(16512, T->reg, &*T);
}
#include "interpreter.h"
int main(int argc, char** argv)
{
	thread T = init();
	const char* src = "";
	std::vector<char> v;
	if(argc == 2)
	{
		if (FILE *fp = fopen(argv[1], "r"))
		{
			char buf[1024];
			while (size_t len = fread(buf, 1, sizeof(buf), fp))
				v.insert(v.end(), buf, buf + len);
			fclose(fp);
			src = buf; 
		}
	}
	InterpretLevl(src, &T);
	while(true)
	{
		execute_ins(&T);
	}
}