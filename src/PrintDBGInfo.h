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
class Value;
class Region;
class Instruction;

class PrintDBGInfo : public ModulePass {

  private:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  void dumpInstructions();

  void dumpDBGINFO(MDNode *Mtdt, std::map<MDNode*, bool> & printed);

  void dumpDBGINFO(Module *M);

  public:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
  //===---------------------------------------------------------------------===

  static char ID;

  PrintDBGInfo() : ModulePass(ID) {};
  
  // We need to insert the Instructions for each source file.
  virtual bool runOnModule(Module &M) override;

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
  }

};

}

//===---------------------- recoverExpressions.h --------------------------===//
