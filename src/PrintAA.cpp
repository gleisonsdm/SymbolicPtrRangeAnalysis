//===---------------------- recoverExpressions.cpp ------------------------===//
//
// This file is distributed under the Universidade Federal de Minas Gerais - 
// UFMG Open Source License. See LICENSE.TXT for details.
//
// Copyright (C) 2015   Gleison Souza Diniz Mendon?a
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include <fstream>
#include <queue>

#include "llvm/Analysis/RegionInfo.h"  
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/DIBuilder.h" 
#include "llvm/IR/Module.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/ValueTracking.h"
#include "llvm/ADT/Statistic.h"
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/Metadata.h>

#include "PrintAA.h"

using namespace llvm;
using namespace std;

void PrintAA::insertMetadataToInst(Instruction *I, std::string label,
		                   std::string info) {
  info = " {" + info + "} ";
  LLVMContext& C = I->getContext();
  MDNode *N = MDNode::get(C, MDString::get(C, info));
  MDNode *MD = I->getMetadata(label);
  MDNode *MDR = N->concatenate(N, MD);
  // Concatenate the current info:
  I->setMetadata(label, MDR);
}

void PrintAA::dumpDBGINFO(Function *F, AliasAnalysis *aa,
		          DependenceAnalysis *da) {
//  errs() << "----------------------------------------------------\n";  
//  errs() << "--------------- Alias Analysis Result --------------\n"; 
//  errs() << "----------------------------------------------------\n";  
  for (auto Src = inst_begin(F), SrcE = inst_end(F); Src != SrcE; ++Src) {
    for (auto Dst = Src, DstE = inst_end(F); Dst != DstE; ++Dst) {
      if (aa->isNoAlias(&(*Src), &(*Dst))) {
//        errs() << "No Alias:\n";
//        Src->dump();
//	Dst->dump(); 
      }
      else if (aa->isMustAlias(&(*Src), &(*Dst))) {
//        errs() << "Must Alias:\n";
//        Src->dump();
//	Dst->dump(); 
      }
      else {
//        errs() << "May Alias:\n";
//        Src->dump();
//	Dst->dump();  
      }
    }
  }
  errs() << "----------------------------------------------------\n";  
  errs() << "------------ Dependence Analysis Result ------------\n"; 
  errs() << "----------------------------------------------------\n";  
  std::string dep1 = std::string();
  std::string dep2 = std::string();
  int id1 = 0, id2 = 0;
  // Check for memory dependecies among every pair of instructions in this function.
  for (auto Src = inst_begin(F), SrcE = inst_end(F); Src != SrcE; ++Src) {
    if (Src->mayWriteToMemory() || Src->mayReadFromMemory()) {
      id1++;
      for (auto Dst = Src, DstE = inst_end(F); Dst != DstE; ++Dst) {
        id2++;
      	if (Dst->mayWriteToMemory() || Dst->mayReadFromMemory()) {
          if (auto D = da->depends(&*Src, &*Dst, true)) {
	    dep1 = std::to_string(id1) + " depends of " + std::to_string(id2);
	    dep2 = std::to_string(id2) + " depends of " + std::to_string(id1);
	    insertMetadataToInst(&*Src, "Memory Dependence", dep1);
	    insertMetadataToInst(&*Dst, "Memory Dependence", dep2);
	  }
	  else {
	    dep1 = std::to_string(id1) + " do not depend of " + std::to_string(id2);
	    dep2 = std::to_string(id2) + " do not depend of " + std::to_string(id1);
	    insertMetadataToInst(&*Src, "Memory Dependence", dep1);
	    insertMetadataToInst(&*Dst, "Memory Dependence", dep2);
	  }
	}
      }
    }
  }
}

void PrintAA::dumpEachFuncionInsts(Function *F) {
  int countInst = 0;
  for (auto Src = inst_begin(F), SrcE = inst_end(F); Src != SrcE; ++Src) {
    countInst++;
  }
  std::string nInst = " NINST = " +  std::to_string(countInst);
  LLVMContext& C = F->begin()->getTerminator()->getContext();
  MDNode* N = MDNode::get(C, MDString::get(C, "Number of Instructions"));
  F->begin()->getTerminator()->setMetadata(nInst, N);
}

bool PrintAA::runOnFunction(Function &F) {
  this->rp = &getAnalysis<RegionInfoPass>();
  this->aa = &getAnalysis<AliasAnalysis>();
  this->dt = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  this->da = &getAnalysis<DependenceAnalysis>(); 

  if (F.isDeclaration() || F.isIntrinsic())
    return true;

  dumpDBGINFO(&F, aa, da);

  dumpEachFuncionInsts(&F);

  return true;
}

char PrintAA::ID = 0;
static RegisterPass<PrintAA> Z("print-aa",
"Recover Expressions to the source File.");

//===------------------------ recoverExpressions.cpp ------------------------===//
