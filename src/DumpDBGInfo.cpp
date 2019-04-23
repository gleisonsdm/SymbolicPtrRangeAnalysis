//===-------------------------- DumpDBGInfo.cpp ---------------------------===//
//
// This file is distributed under the Universidade Federal de Minas Gerais - 
// UFMG Open Source License. See LICENSE.TXT for details.
//
// Copyright (C) 2019   Gleison Souza Diniz Mendon?a
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

#include "DumpDBGInfo.h"

using namespace llvm;
using namespace std;

void DumpDBGInfo::insertMetadataToInst(Instruction *I, std::string label,
		                   std::string info) {
  // Find the context to the instruction:
  LLVMContext& C = I->getContext();
  // Build a Metadata node with the information:
  MDNode *N = MDNode::get(C, MDString::get(C, info));
  // Select the current metadata with the same label:
  MDNode *MD = I->getMetadata(label);
  // Concatenate the old information with the one you would like to insert:
  MDNode *MDR = N->concatenate(MD, N);
  // Concatenate the current info:
  I->setMetadata(label, MDR);
}

std::string DumpDBGInfo::getMetadataInfo(Instruction *I, std::string label) {
  MDNode *MD = I->getMetadata(label);
  if (MD) {
    std::string strMetadata = std::string();
    errs() << "---------------------------------------------------\n";
    I->dump();
    errs() << "---------------------------------------------------\n";
    for (int i = 0, ie = MD->getNumOperands(); i != ie; i++) {
      if (MDString *MDS = dyn_cast<MDString>(MD->getOperand(i))) {
	std::string info =  MDS->getString();
        strMetadata = info + strMetadata;
        errs() << "Info : " <<  info << "\n";
      }
    }
    return strMetadata;
  }
  return std::string();
}

void DumpDBGInfo::dumpDBGINFO(Function *F) {
  for (auto Src = inst_begin(F), SrcE = inst_end(F); Src != SrcE; ++Src) {
    getMetadataInfo(&*Src, "InstID");
    getMetadataInfo(&*Src, "Memory Dependence");
  }
}

bool DumpDBGInfo::runOnFunction(Function &F) {
  this->rp = &getAnalysis<RegionInfoPass>();

  if (F.isDeclaration() || F.isIntrinsic())
    return true;

  dumpDBGINFO(&F);

  return true;
}

char DumpDBGInfo::ID = 0;
static RegisterPass<DumpDBGInfo> Z("dump-dbg-info",
"Recover Expressions to the source File.");

//===-------------------------- DumpDBGInfo.cpp --------------------------===//
