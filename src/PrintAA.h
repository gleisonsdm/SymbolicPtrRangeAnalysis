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
#include "llvm/Analysis/DependenceAnalysis.h"

namespace llvm {
class ScalarEvolution;
class AliasAnalysis;
class DominatorTree;

class DominanceFrontier;
struct PostDominatorTree;
class Value;
class Region;
class Instruction;

class PrintAA : public FunctionPass {

  private:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  void insertMetadataToInst(Instruction *I, std::string label, std::string info);

  void dumpDBGINFO(Function *F, AliasAnalysis *aa, DependenceAnalysis *da);

  void dumpEachFuncionInsts(Function *F);

  public:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  static char ID;

  PrintAA() : FunctionPass(ID) {};
 
  // We need to insert the Instructions for each source file.
  virtual bool runOnFunction(Function &F) override;

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<RegionInfoPass>();
      AU.addRequired<AliasAnalysis>();
      AU.addRequired<DominatorTreeWrapperPass>();
      AU.addRequiredTransitive<DependenceAnalysis>();
      AU.setPreservesAll();
  }

  RegionInfoPass *rp;
  AliasAnalysis *aa;
  DominatorTree *dt;
  DependenceAnalysis *da;
};

}

//===---------------------- recoverExpressions.h --------------------------===//
