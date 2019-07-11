// Author: Lukasz Stempniewicz 25/05/19

// -----------------------------------------------------------------------------
//
// Copyright (C) The BioDynaMo Project.
// All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------

#ifndef GUI_MODEL_ELEMENT_H_
#define GUI_MODEL_ELEMENT_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <map>

#include <RQ_OBJECT.h>
#include <TApplication.h>
#include <TClass.h>
#include <TEnv.h>
#include <TFile.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TVirtualX.h>
#include <TMethod.h>
#include <TMethodArg.h>
#include <TMethodCall.h>

#include "TObject.h"
#include "TString.h"
#include "gui/view/log.h"
#include "gui/model/simulation_entity.h"
#include "gui/model/module.h"
#include "gui/constants.h"

namespace gui {

class ModelElement {
 public:
  ModelElement() {
    if(fEntityAttributeMap.empty()) {
      PopulateCellMembers();
    }
    if(fModuleAttributeMap.empty()) {
      //PopulateModuleMembers();
    }
  }
  ~ModelElement() = default;
  std::string GenerateCode() {return "";};
  void        Save() {};
  Bool_t      SetElementType(int type) {
    if(fType > 0) {
      Log::Error("Element is already set for this model element!");
      return kTRUE;
    }
    if(type != M_ENTITY_CELL && type != M_MODULE_GROWTH) {
      Log::Warning("Type:", type, " not yet supported!");
      return kFALSE;
    }
    fType = type;
    return kTRUE;
  }

  void SetName(const char* name) {
    fName.assign(name);
  }

  bdm::Cell* GetElement() {
    return fEntity.GetElement();
  }

  void PrintData() {
    Log::Debug("Type:");
    switch(fType) {
      case M_ENTITY_CELL:
        Log::Debug("Cell");
        break;
      case M_MODULE_GROWTH:
        Log::Debug("Module");
        break;
      default:
        Log::Debug("UNKNOWN");
    }
  }

  std::string GetName() {
    return fName;
  }

  int GetType() {
    return fType;
  }

  std::map<std::string, std::string> GetAttributeMap() {
    std::map<std::string, std::string> tmpMap;
    switch(fType) {
      case M_ENTITY_CELL:
        tmpMap.insert(fEntityAttributeMap.begin(), fEntityAttributeMap.end());
        break;
      case M_MODULE_GROWTH:
        tmpMap.insert(fModuleAttributeMap.begin(), fModuleAttributeMap.end());
        break;
      default:
        Log::Warning("UNKNOWN TYPE");
    }
    return tmpMap;
  }

 private:

  void PopulateCellMembers() {
    Log::Info("Populating Cell attribute map");
    std::unique_ptr<bdm::Cell> cellPtr = std::make_unique<bdm::Cell>(1.0);
    TClass *cl = cellPtr->IsA();
  
    Log::Debug("Public Methods:");
    auto t = cl->GetListOfAllPublicMethods();

    /// Declaring loop variables    
    TMethod* method = nullptr;
    TObjLink* lnk = nullptr;
    TMethodArg* arg = nullptr;
    std::string clName, methodName, methodSignature, memberName, fullType;

    TIterator* it = t->MakeIterator();
    TObject* obj = it->Next();
    while(obj) {
      clName.assign(obj->ClassName());
      if(clName.compare("TMethod") == 0) {
        method = (TMethod*)obj;
        methodName.assign(method->GetName());
        if(methodName.find("Set") != std::string::npos) {
          methodSignature.assign(method->GetSignature());
          Log::Debug("Setter found:", methodName, methodSignature);
          memberName.assign(methodName.substr(3));
          fullType.assign("");
          TList* args = method->GetListOfMethodArgs();
          if(args->GetEntries() > 1) {
            Log::Error("Currently only supports 1 argument!!!");
            return;
          }
          lnk = args->FirstLink();
            while (lnk) {
              arg = (TMethodArg*)lnk->GetObject();
              Log::Debug("Method arg full type: ", arg->GetFullTypeName());
              fullType.assign(arg->GetFullTypeName());
              Log::Debug("Method arg type: ", arg->GetTypeName());
              Log::Debug("Method arg name: ", arg->GetName());
              lnk = lnk->Next();
            }
          
          methodName.replace(0, 3, "Get");
          Log::Debug("Getter should be:", methodName);
          Log::Debug("Inserting attribute into map -> member:`", memberName, "`, type:`", fullType, "`");
          fEntityAttributeMap.insert(std::pair<std::string, std::string>(memberName, fullType));
        }
      }
      obj = it->Next();
    }
  
    /// Testing
    Double_t retVal = -1.0;
    TMethodCall call(cl, "GetDiameter", "");
    call.Execute((void*)cellPtr.get(), retVal);
    Log::Debug("Called GetDiameter returns: ", retVal);
  }

  void PopulateModuleMembers() {}

  std::string        fPathName;
  //ModelElement*      fParent;

  SimulationEntity fEntity;
  Module           fModule;
  static std::map<std::string, std::string> fEntityAttributeMap;
  static std::map<std::string, std::string> fModuleAttributeMap;
  /// TODO: add other element types

  int              fType = 0;
  std::string      fName;
  
  ClassDefNV(ModelElement,1)
};

} // namespace gui

#endif  // GUI_MODEL_ELEMENT_H_