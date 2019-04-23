//===------------------------- PrintMemDep.h -----------------------------===//
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

  // Find the line for a value in the source code.
  // Return -1 case not found it.
  int getLineNum(Value *V);

  // Find the column for a value in the source code.
  // Return -1 case not found it. 
  int getColumnNum(Value *V);

  // Interface that pprovides a easy way to associate Metadata to a Instruction.
  // 
  // - I -> Instruction Reference
  // - label -> ID to identify the metadata
  // - info -> Information to insert
  //  
  void insertMetadataToInst(Instruction *I, std::string label, std::string info);

  // Write the information in DependenceAnalysis in DWARF format.
  void printDBGINFO(Function *F, DependenceAnalysis *da);

  // Write information of each Function in DWARF format.
  // 
  // Including:
  // - ID for each instruction
  // - Function name
  // - Line of the instruction in the source code
  // - Column of the instruction in the source code
  //
  void printEachFuncionInsts(Function *F);

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

//===------------------------- PrintMemDep.h -----------------------------===//
