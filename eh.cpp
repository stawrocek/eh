#include <cstdio>
#include <cstdlib>
#include <sys/ptrace.h>
#include <unistd.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <stdarg.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include <vector>
#include <string>
#include <algorithm>
#include <initializer_list>

extern char* strsignal(int);

void procmsg(const char* format, ...){
	va_list ap;
	fprintf(stdout, "[%d] ", getpid());
	va_start(ap, format);
	vfprintf(stdout, format, ap);
	va_end(ap);
}

uint64_t parseLL(char* str){
	if(strlen(str) > 2 && str[0] == '0' && str[1] == 'x')
        return strtoll(str, nullptr, 16);
	return atoll(str);
}

void run_target(const char* programname, const char* password){
	if(ptrace(PTRACE_TRACEME, 0, 0, 0) < 0){
		perror("ptrace error (you should probably patch ptrace checking from binary)\n");
		return;
	}
	execl(programname, programname, password, 0);
}

user_regs_struct getRegs(pid_t child_pid){
	struct user_regs_struct regs;
	ptrace(PTRACE_GETREGS, child_pid, 0, &regs);
	return regs;
}

void registerBreakpoint(pid_t child_pid, uint64_t addr, long orig_data){
	long data_with_trap = (orig_data & 0xffffff00) | 0xcc;
	ptrace(PTRACE_POKETEXT, child_pid, (void*)addr, (void*)data_with_trap);
	long readback_data = ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addr, 0);
}

void removeBreakpoint(pid_t child_pid, uint64_t addr, long orig_data){
	auto regs = getRegs(child_pid);
	ptrace(PTRACE_POKETEXT, child_pid, (void*)addr, (void*)orig_data);
	regs.rip -= 1;
	ptrace(PTRACE_SETREGS, child_pid, 0, &regs);
}

bool checkBooleanFunctionFailure(pid_t child_pid, uint64_t avoid_value, char* reg){
	auto regs = getRegs(child_pid);

	std::vector<std::pair<unsigned long long int*, const char *> >vec_regs = {
		{&regs.rax, "rax"},
		{&regs.rbx, "rbx"},
		{&regs.rcx, "rcx"},
		{&regs.rdx, "rdx"},
		{&regs.rsi, "rsi"},
		{&regs.rdi, "rdi"},
		{&regs.rbp, "rbp"},
		{&regs.rsp, "rsp"},
		{&regs.r8, "r8"},
		{&regs.r9, "r9"},
		{&regs.r10, "r10"},
		{&regs.r11, "r11"},
		{&regs.r12, "r12"},
		{&regs.r13, "r13"},
		{&regs.r14, "r14"},
		{&regs.r15, "r15"}
	};
	for(auto p: vec_regs){
		uint64_t value = *(p.first);
		if(reg[0] == 'e'){
			reg[0] = 'r';
			value &= 0xffffffff;
	        }
		if(strcmp(p.second, reg) == 0)
		return value == avoid_value;
	}
}

int run_debugger(pid_t child_pid, uint64_t addrResult,
	uint64_t addrLoop, uint64_t avoid_val, char* reg){

	int wait_status;
	wait(&wait_status);
	long orig_data=ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addrResult, 0);
	long orig_data_loop=ptrace(PTRACE_PEEKTEXT, child_pid, (void*)addrLoop, 0);
	registerBreakpoint(child_pid, addrLoop, orig_data_loop);
	int result = 0;
	while (WIFSTOPPED(wait_status)){
		ptrace(PTRACE_CONT, child_pid, 0, 0);
		wait(&wait_status);
		removeBreakpoint(child_pid, addrLoop, orig_data_loop);
		registerBreakpoint(child_pid, addrResult, orig_data);
		ptrace(PTRACE_CONT, child_pid, 0, 0);
		wait(&wait_status);
		if(checkBooleanFunctionFailure(child_pid, avoid_val, reg))
			return result;
		result++;
		removeBreakpoint(child_pid, addrResult, orig_data);
		registerBreakpoint(child_pid, addrLoop, orig_data_loop);
	}
	return result;
}

std::string str = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~ ";

int main(int argc, char** argv){
	if(argc != 7){
		fprintf(stderr, "+------------------------eh------------------------+\n");
		fprintf(stderr, "|                                                  |\n");
		fprintf(stderr, "| Simple tool for cracking x64 linux binaries      |\n");
		fprintf(stderr, "| with pseudocode following pseudocode:            n\n");
		fprintf(stderr, "|                                                  e\n");
		fprintf(stderr, "| bool testChar(char* c, int i, ...){              v\n");
		fprintf(stderr, "|      //lots of shenanigans && obfuscation here   e\n");
		fprintf(stderr, "|      //but in general:                           r\n");
		fprintf(stderr, "|      testChar(...) -> c at index i is proper     |\n");
		fprintf(stderr, "| }                                                g\n");
		fprintf(stderr, "|                                                  o\n");
		fprintf(stderr, "| //somewhere in the code                          n\n");
		fprintf(stderr, "| for(int i = 0; i < strlen(password); i++){       n\n");
		fprintf(stderr, "|     if(testChar(i,password[i])==avoid_val){      a\n");
		fprintf(stderr, "|         exit();                                  |\n");
		fprintf(stderr, "|     }                                            g\n");
		fprintf(stderr, "| }                                                i\n");
		fprintf(stderr, "| //do something with proper password              v\n");
		fprintf(stderr, "| //simply authenticate or use it as checksum      e\n");
		fprintf(stderr, "|                                                  |\n");
		fprintf(stderr, "+--------------------how-to-use--------------------+\n");
		fprintf(stderr, "|                                                  |\n");
		fprintf(stderr, "| ./eh progname a1 a2 avoid_val register len       y\n");
		fprintf(stderr, "| progname -> path to binary to crack              o\n");
		fprintf(stderr, "| a1 -> address of instruction after function call u\n");
		fprintf(stderr, "| a2 -> any address from loop != addr1             |\n");
		fprintf(stderr, "| avoid_val -> boolean func result to avoid        u\n");
		fprintf(stderr, "| register -> register containing result           p\n");
		fprintf(stderr, "|    {rax,rbx,rcx,rdx,rsi,rdi,rbp,rsp,r8,...,r15,  !\n");
		fprintf(stderr, "|     eax,ebx,ecx,edx,esi,edi,ebp,esp}             |\n");
		fprintf(stderr, "| len -> password length                           |\n");
		fprintf(stderr, "|                                                  |\n");
		fprintf(stderr, "+--------------------------------------------------+\n");
		return -1;
	}
	int result = 0;
	std::string crackedPassword(parseLL(argv[6]), '?');

	for(int i = 0; i < parseLL(argv[6]); i++){
		if(result < i){
			if(crackedPassword.size() < parseLL(argv[6]))
				procmsg("result %d at position %d is too small!\nFailure...\n", result, i);
			else
				procmsg("Pasword: %s\nGG!\n", crackedPassword.c_str());
			break;
		}
		for(int j = 0; j < str.size(); j++){
			crackedPassword[i] = str[j];
			pid_t child_pid;
			child_pid = fork();
			if(child_pid == 0){
				const char* pass = crackedPassword.c_str();
				run_target(argv[1], pass);
			}
			else if(child_pid > 0){
				/*0x401c82, 0x401c9f*/
				int res = run_debugger(child_pid, parseLL(argv[2]),
					parseLL(argv[3]), parseLL(argv[4]), argv[5]);
				if(res == i+1){
					result = res;
					std::string tmpPass = crackedPassword.substr(0, i);
					procmsg("Password(%d) = %s\n", i, tmpPass.c_str());
					break;
				}
			}
			//usleep(20);
		}
	}
    return 0;
}
