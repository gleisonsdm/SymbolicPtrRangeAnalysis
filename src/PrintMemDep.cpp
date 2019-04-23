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

#include "PrintMemDep.h"

using namespace llvm;
using namespace std;

int PrintMemDep::getLineNum(Value *V) {
  if(!V)
    return -1;
  if (Instruction *I = dyn_cast<Instruction>(V))
      if (MDNode *N = I->getMetadata("dbg"))
        if (DILocation *DL = dyn_cast<DILocation>(N))
          return DL->getLine();
  return -1;
}

int PrintMemDep::getColumnNum(Value *V) {
  if(!V)
    return -1;
  if (Instruction *I = dyn_cast<Instruction>(V))
      if (MDNode *N = I->getMetadata("dbg"))
        if (DILocation *DL = dyn_cast<DILocation>(N))
          return DL->getColumn();
  return -1;
}

void PrintMemDep::insertMetadataToInst(Instruction *I, std::string label,
		                   std::string info) {
  info = " {" + info + "} ";
  LLVMContext& C = I->getContext();
  MDNode *N = MDNode::get(C, MDString::get(C, info));
  MDNode *MD = I->getMetadata(label);
  MDNode *MDR = N->concatenate(N, MD);
  // Concatenate the current info:
  I->setMetadata(label, MDR);
}

void PrintMemDep::dumpDBGINFO(Function *F, DependenceAnalysis *da) {
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

void PrintMemDep::dumpEachFuncionInsts(Function *F) {
  int countInst = 0;
  for (auto I = inst_begin(F), IE = inst_end(F); I != IE; ++I) {
    countInst++;
    std::string info = std::string();
    std::string fName = F->getName();
    info += "< FUNCTION " + fName + " >";
    info += "< ID " + std::to_string(countInst) + " >";
    info += "< LINE " + std::to_string(getLineNum(&*I)) + " >";
    info += "< COLUMN " + std::to_string(getColumnNum(&*I)) + " >";
    insertMetadataToInst(&*I, "InstID", info);
  }
  std::string nInst = " NINST = " +  std::to_string(countInst);
  insertMetadataToInst(F->begin()->getTerminator(), "Number of Instructions", nInst);
}

bool PrintMemDep::runOnFunction(Function &F) {
  this->rp = &getAnalysis<RegionInfoPass>();
  this->dt = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  this->da = &getAnalysis<DependenceAnalysis>(); 

  if (F.isDeclaration() || F.isIntrinsic())
    return true;

  dumpDBGINFO(&F, da);

  dumpEachFuncionInsts(&F);

  return true;
}

char PrintMemDep::ID = 0;
static RegisterPass<PrintMemDep> Z("print-mem-dep",
"Recover Expressions to the source File.");

//===------------------------ recoverExpressions.cpp ------------------------===//
