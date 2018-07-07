/* Debug type for MyPass. */
#define DEBUG_TYPE "MyPass"
/* All the required header files here. */
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/User.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/InstIterator.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ADT/ValueMap.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/IR/Constants.h"

#include "llvm/IR/GlobalValue.h"

/* Using the namespace llvm here. */
using namespace llvm;


/* Anonymous namespace starts here. */
namespace {
struct MyPass :  public ModulePass {

	static char ID;                           
	MyPass() : ModulePass(ID) {}

    virtual bool runOnModule(llvm::Module &M){
        bool changed = false;
		
		/* Iterating over all the functions. */
        for(Module::iterator F = M.begin(), E = M.end(); F!=E; ++F){
			
			/* Storing the address of Function. */
			Function &addr = *F;
			/* Procedd only if function is actually in module. */
			if(!addr.isDeclaration()){
				
				/*Iterating over basic blocks in the function. */
				for(Function::iterator bb = F->begin(), bbe = F->end(); bb != bbe; ++bb){
					
					/* Iteraating over all the instructions one-by-one. */
					for(BasicBlock::iterator i = bb->begin(), e = bb->end(); i != e; ++i){
						
						/* Checking if instruction is a call to a function using dynamic casting. */
						if (CallInst* callInst = dyn_cast<CallInst>(&*i)){
							
							/* Proceeding only if the callee function is in the module. */
							if(!(callInst->getCalledFunction()->isDeclaration())){
								
								/* Number of opereands here. */
								unsigned arg_num = callInst->getNumArgOperands();
								/* Variable flag for indication. */
								int flag = 0;
								/* For all the arguments of the callee function. */
								for(int k = 0;k < arg_num;++k){
									Value *arg_hold = callInst->getArgOperand(k);
									/* Checking if its a constant. */
									if(isa<Constant>(arg_hold))
										continue;
									else{
										/* If not a constant, raise the flag and break. */
										flag = 1;
										break;
									}
								}
								/* Check if the flag is raised or number of arguments = 0. */
								if(flag != 0 || arg_num == 0)
									continue;
								else{
									/* Else, all arguments must be constant. Safe to proceed.*/
									
									/* Iterating over the instructions of the callee function to count them. */
									int counter = 0;
									for (Function::iterator basic = callInst->getCalledFunction()->begin(), basice = callInst->getCalledFunction()->end(); basic != basice; ++basic){
										for(BasicBlock::iterator foo = basic->begin(), fooe = basic->end();foo != fooe;++foo){
											counter++;	
										}
									}
									/* If number of instructions < 10, only then proceed. */
									if(counter < 10){
										/* For all arguments of callee function. */
										for(int k = 0;k < arg_num;++k){
											/* Accessing the arguments by indexing (k is the index). */
											Value *arg_hold = callInst->getArgOperand(k);
											Type *Ty = arg_hold->getType();
											/* Creating a new constant with the type and value of the argument. */
											Constant *c = (Constant *) arg_hold;
											const APInt api = c->getUniqueInteger();
											Constant *newC = ConstantInt::get(Ty, api);
											
											/* Going through all the arguments again. */
											for(Function::arg_iterator constArg = callInst->getCalledFunction()->arg_begin(),
												constArgE = callInst->getCalledFunction()->arg_end(); constArg != constArgE; ++constArg){
												/* If it's the argument in question, replace it with constant. */
												//if(constArg == arg_hold)
												constArg->replaceAllUsesWith(newC);
											}
										}
										/* Copy the instructions from callee function. */
										llvm::ValueToValueMapTy vmap;
										for(Function::iterator BB = callInst->getCalledFunction()->begin(); BB != callInst->getCalledFunction()->end(); ++BB){
											for(BasicBlock::iterator in = BB->begin(); in != BB->end(); ++in){
												/* Proceed only if it is not a return instruction. */
												if(!isa<ReturnInst>(in)){
													Instruction *new_inst = in->clone();
													new_inst->insertBefore(i);
													vmap[in] = new_inst;
													/* Remapping the instructions. */
													llvm::RemapInstruction(new_inst, vmap, RF_NoModuleLevelChanges | static_cast<RemapFlags>(2));
												}
											}
										}
										//remove here.
										
										for(Function::iterator bb1 = callInst->getCalledFunction()->begin(); bb1 != callInst->getCalledFunction()->end(); bb1++){
											for(BasicBlock::iterator fun_instr = bb1->begin(); fun_instr != bb1->end(); fun_instr++){
	                                            
												if (ReturnInst* ret_inst = dyn_cast<ReturnInst>(&*fun_instr)){
	                                                Instruction* inst_after_call = ++i;
	                                                if(ret_inst->getNumOperands() != 0){
	                                                    Value *ret_value = ret_inst->getReturnValue();
														callInst->replaceAllUsesWith(vmap[ret_value]);
													}
	                                                
													Instruction* inst_to_rem = --i;
	                                                i--;
	                                                inst_to_rem->eraseFromParent();
												}
											}		
										}
									}
								}
							}
						}
					}
				}
			}
        }	
        return changed;
    }
};
}

char MyPass::ID = 0;
/* Registration here */
static RegisterPass<MyPass> X("mypass", "My Custom Pass", false, false);
