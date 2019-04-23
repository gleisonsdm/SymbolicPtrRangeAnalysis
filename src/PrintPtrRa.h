//===---------------------- recoverExpressions.h --------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the Universidade Federal de Minas Gerais -
// UFMG Open Source License. See LICENSE.TXT for details.
//
// Copyright (C) 2015   Gleison Souza Diniz Mendon?a
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/LoopInfo.h"

#ifndef myutils
#define myutils

#include "PtrRangeAnalysis.h"
#endif

using namespace lge;

namespace llvm {
class ScalarEvolution;
class AliasAnalysis;
class SCEV;
class DominatorTree;

class DominanceFrontier;
struct PostDominatorTree;
class Value;
class Region;
class Instruction;
class LoopInfo;
class ArrayInference;
class ScopeTree;

class PrintPtrRa : public FunctionPass {

  private:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===


  void dumpInstructions(Region *R,
	      std::map<Value*, std::pair<Value*, Value*> > & instSet);

  void dumpDBGINFO(Function *F, AliasAnalysis *aa);

  bool pointerDclInsideRegion(Region *R, Value *V);

  void runPtrRa(Region *r, PtrRangeAnalysis *ptrRA,  RegionInfoPass *rp,
		           AliasAnalysis *aa, ScalarEvolution *se,
			   LoopInfo *li, DominatorTree *dt);

  public:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  static char ID;

  PrintPtrRa() : FunctionPass(ID) {};
  
  // We need to insert the Instructions for each source file.
  virtual bool runOnFunction(Function &F) override;

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<RegionInfoPass>();
      AU.addRequired<AliasAnalysis>();
      AU.addRequired<ScalarEvolution>();
      AU.addRequiredTransitive<LoopInfoWrapperPass>();
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequired<RegionReconstructor>(); 
      AU.addRequired<PtrRangeAnalysis>();
      AU.setPreservesAll();
  }

  RegionInfoPass *rp;
  AliasAnalysis *aa;
  ScalarEvolution *se;
  LoopInfo *li;
  DominatorTree *dt;
  RegionReconstructor *rr;
  PtrRangeAnalysis *ptrRa;
};

}

//===---------------------- recoverExpressions.h --------------------------===//
