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
#include "llvm/Analysis/DependenceAnalysis.h"

namespace llvm {
class ScalarEvolution;
class DominatorTree;

class DominanceFrontier;
struct PostDominatorTree;
class Value;
class Region;
class Instruction;

class PrintMemDep : public FunctionPass {

  private:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  int getLineNum(Value *V);

  int getColumnNum(Value *V);

  void insertMetadataToInst(Instruction *I, std::string label, std::string info);

  void dumpDBGINFO(Function *F, DependenceAnalysis *da);

  void dumpEachFuncionInsts(Function *F);

  public:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  static char ID;

  PrintMemDep() : FunctionPass(ID) {};
 
  // We need to insert the Instructions for each source file.
  virtual bool runOnFunction(Function &F) override;

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<RegionInfoPass>();
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequiredTransitive<DependenceAnalysis>();
      AU.setPreservesAll();
  }

  RegionInfoPass *rp;
  DominatorTree *dt;
  DependenceAnalysis *da;
};

}

//===---------------------- recoverExpressions.h --------------------------===//
