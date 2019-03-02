
#include <vector>
#include "stack.h"
#include "executor.h"

#include "load0Command.h"
#include "load1Command.h"
#include "loadNCommand.h"
#include "addCommand.h"
#include "subCommand.h"
#include "mulCommand.h"
#include "divCommand.h"
#include "dupCommand.h" 
#include "outCommand.h"
#include "inCommand.h"
#include "jnzCommand.h"
#include "popCommand.h"

void EmptyAndPrintStack(Istack& _stack)
{
    while(!_stack.Empty())
    {
        std::cout << "|" << _stack.Top();
        _stack.Pop();
    }
    std::cout << "|" << std::endl;
}

int main()
{
	std::vector<Icommand*> vec;
	
	Load1Command load1;
	AddCommand add;
	DupCommand dup;
    Load0Command load0;
    LoadNCommand loadN(5);
    SubCommand sub;
    MulCommand mul;
    DivCommand div;
    OutCommand out;
    InCommand in;
    JnzCommand jnz(2);
    PopCommand pop;	
	
    // test #print 6 
    vec.push_back(&load1);
    vec.push_back(&dup);
	vec.push_back(&add);
	vec.push_back(&load1);
	vec.push_back(&add);
	vec.push_back(&dup);
	vec.push_back(&dup);
	vec.push_back(&mul);
    vec.push_back(&sub);	
    vec.push_back(&jnz);	
    vec.push_back(&out);
	
	std::vector<Icommand*>::iterator vecBegin = vec.begin();
	std::vector<Icommand*>::iterator vecEnd = vec.end();
	
	InstructionIterator<std::vector<Icommand*>::iterator> itrBegin(vecBegin);
	InstructionIterator<std::vector<Icommand*>::iterator> itrEnd(vecEnd);
	
	Istack* stack = new Stack;
	Executor executer(stack, vec.size());
	
	executer.Run<InstructionIterator<std::vector<Icommand*>::iterator> > (itrBegin, itrEnd);

    EmptyAndPrintStack(*stack);



    return 0;
}


