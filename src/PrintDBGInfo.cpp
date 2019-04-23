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

#include "PrintDBGInfo.h"

using namespace llvm;
using namespace std;
using namespace lge;

void PrintDBGInfo::dumpInstructions() {
}

void PrintDBGInfo::dumpDBGINFO(MDNode *Mtdt, std::map<MDNode*, bool> & printed) {
  if (printed.count(Mtdt) != 0)
     return;
  Mtdt->dump();
  printed[Mtdt] = true;
  for (int i = 0, ie = Mtdt->getNumOperands(); i < ie; i++) {
    if (!Mtdt->getOperand(i))
      continue;
    if (MDNode *Mtdta = dyn_cast<MDNode>(Mtdt->getOperand(i)))
      dumpDBGINFO(Mtdta, printed);
  }
}

void PrintDBGInfo::dumpDBGINFO(Module *M) {
    std::map<MDNode*, bool> printed;
    for (auto MI = M->named_metadata_begin(), ME = M->named_metadata_end();
		 MI != ME; MI++) {
    for (int i = 0, ie = MI->getNumOperands(); i < ie; i++) {
      dumpDBGINFO(MI->getOperand(i), printed);
    }
  }
}


bool PrintDBGInfo::runOnModule(Module &M) {

  dumpDBGINFO(&M);

  return true;
}

char PrintDBGInfo::ID = 0;
static RegisterPass<PrintDBGInfo> Z("print-dbg-info",
"Recover Expressions to the source File.");

//===------------------------ recoverExpressions.cpp ------------------------===//
