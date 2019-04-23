//===------------------------ DumpDBGInfo.h ------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the Universidade Federal de Minas Gerais -
// UFMG Open Source License. See LICENSE.TXT for details.
//
// Copyright (C) 2019   Gleison Souza Diniz Mendon?a
//
//===----------------------------------------------------------------------===//
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/RegionInfo.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include <llvm/IR/Metadata.h>

namespace llvm {
class DominanceFrontier;
struct PostDominatorTree;
class Value;
class Region;
class Instruction;

class DumpDBGInfo : public FunctionPass {

  private:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===
 
  //===---------------------------------------------------------------------===

  // Interface that pprovides a easy way to associate Metadata to a Instruction.
  // 
  // - I -> Instruction Reference
  // - label -> ID to identify the metadata
  // - info -> Information to insert
  // 
  void insertMetadataToInst(Instruction *I, std::string label, std::string info);

  // recover and Print Information in the DWARF format (return an unique string
  // with the info).
  std::string getMetadataInfo(Instruction *I, std::string label);

  // Find the information in DWARF format and dump it into standart output.
  void dumpDBGINFO(Function *F);

  public:

  //===---------------------------------------------------------------------===
  //                              Data Structs
  //===---------------------------------------------------------------------===

  //===---------------------------------------------------------------------===

  static char ID;

  DumpDBGInfo() : FunctionPass(ID) {};
 
  // We need to insert the Instructions for each source file.
  virtual bool runOnFunction(Function &F) override;

  virtual void getAnalysisUsage(AnalysisUsage &AU) const {
      AU.addRequired<RegionInfoPass>();
      AU.setPreservesAll();
  }

  RegionInfoPass *rp;
};

}

//===-------------------------- DumpDBGInfo.h ----------------------------===//
