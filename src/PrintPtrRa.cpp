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

#include "PrintPtrRa.h"

using namespace llvm;
using namespace std;
using namespace lge;

void PrintPtrRa::dumpInstructions(Region *R,
	      std::map<Value*, std::pair<Value*, Value*> > & instSet) {
}

void PrintPtrRa::dumpDBGINFO(Function *F, AliasAnalysis *a) {
  std::vector<Instruction*> memoryInst;
  for (auto B = F->begin(), BE = F->end(); B != BE; B++) {
    for (auto I = B->begin(), IE = B->end(); I != IE; I++) {
      if (isa<LoadInst>(I))
	memoryInst.push_back(I);
      if (isa<StoreInst>(I))
	memoryInst.push_back(I);
    }
  }
  for (int i = 0, ie = memoryInst.size(); i != ie; i++) {
    for (int j = i + 1, je = memoryInst.size(); j != je; j++) {
      if (aa->isNoAlias(memoryInst[i], memoryInst[j])) {
        errs() << "No Alias:\n";
	memoryInst[i]->dump();
	memoryInst[j]->dump(); 
      }
      else if (aa->isMustAlias(memoryInst[i], memoryInst[j])) {
        errs() << "Must Alias:\n";
	memoryInst[i]->dump();
	memoryInst[j]->dump(); 
      }
      else {
        errs() << "May Alias:\n";
	memoryInst[i]->dump();
	memoryInst[j]->dump(); 
      }
    }
  }
}

bool PrintPtrRa::pointerDclInsideRegion(Region *R, Value *V) {
  if (isa<GlobalValue>(V) || isa<Argument>(V))
    return false;
  
  if (!isa<Instruction>(V))
    return false;

  Instruction *Inst = cast<Instruction>(V);
  for (auto BB = R->block_begin(), BE = R->block_end(); BB != BE; BB++)
    for (auto I = BB->begin(), IE = BB->end(); I != IE; I++) {
      if (Inst == I)
        return true;
    }
  return false;
}

void PrintPtrRa::runPtrRa(Region *r, PtrRangeAnalysis *ptrRA, 
		                    RegionInfoPass *rp,
                                    AliasAnalysis *aa, ScalarEvolution *se,
                                    LoopInfo *li, DominatorTree *dt) {
  // Initilize The Analisys with Default Values.
  Module *M = r->block_begin()->getParent()->getParent();
  const DataLayout DT = DataLayout(M);
  std::map<Value*, std::pair<Value*, Value*> > pointerBounds;

  if (ptrRA->RegionsRangeData[r].HasFullSideEffectInfo == false)
    return;

  // If the region has not an Entering Block, it is not a hammoc region and 
  // the region is invalid.
  Instruction *insertPt = nullptr;
  
  if (r->isTopLevelRegion()) {
    BasicBlock *BB = *(r->block_begin());
    time_t t = time(0);
    std::string name = "AI_BLOCK" + std::to_string(t);
    BasicBlock *newBB = BB->splitBasicBlock(BB->begin(), name);
    insertPt = BB->getTerminator();
  }
  else if (r->getEnteringBlock())
    insertPt = r->getEnteringBlock()->getTerminator(); 

  if (!insertPt)
    return;

  SCEVRangeBuilder rangeBuilder(se, DT, aa, li, dt, r, insertPt);
  // Generate and store both bounds for each base pointer in the region.
  for (auto& pair : ptrRA->RegionsRangeData[r].BasePtrsData) {
    if (pointerDclInsideRegion(r,pair.first)) {
      continue;
    }
    // Adds "sizeof(element)" to the upper bound of a pointer, so it gives us
    // the address of the first byte after the memory region.
    Value *low = rangeBuilder.getULowerBound(pair.second.AccessFunctions);
    Value *up = rangeBuilder.getUUpperBound(pair.second.AccessFunctions);
    up = rangeBuilder.stretchPtrUpperBound(pair.first, up);
    pointerBounds.insert(std::make_pair(pair.first, std::make_pair(low, up)));
  }
}

bool PrintPtrRa::runOnFunction(Function &F) {
  this->li = &getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  this->rp = &getAnalysis<RegionInfoPass>();
  this->aa = &getAnalysis<AliasAnalysis>();
  this->se = &getAnalysis<ScalarEvolution>();
  this->dt = &getAnalysis<DominatorTreeWrapperPass>().getDomTree();
  this->rr = &getAnalysis<RegionReconstructor>();

  if (F.isDeclaration() || F.isIntrinsic())
    return true;

  dumpDBGINFO(&F, aa);

  return true;
}

char PrintPtrRa::ID = 0;
static RegisterPass<PrintPtrRa> Z("print-ptr-ra",
"Recover Expressions to the source File.");

//===------------------------ recoverExpressions.cpp ------------------------===//
