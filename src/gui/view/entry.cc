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

#include "gui/view/entry.h"
#include "gui/view/inspector.h"

namespace gui {

Entry::Entry(TGCompositeFrame* fMain, Inspector* parentInspector, const char* entryName, const char* entryType) :  TGHorizontalFrame(fMain, 200, 30){
  Log::Debug("Created Entry:`", entryName, "` with type:`", entryType, "`");
  fEntryName.assign(entryName);
  fParentInspector = parentInspector;
  std::string fEntryTypeStr(entryType);

  /// Type: double
  if(fEntryTypeStr.find("double") != std::string::npos) { 
    fEntryType = EntryType::M_DOUBLE;
  } 
  /// Type: bool
  else if(fEntryTypeStr.find("bool") != std::string::npos) {
    fEntryType = EntryType::M_BOOLEAN;
  } 
  /// Type: unint32_t
  else if(fEntryTypeStr.find("uint32_t") != std::string::npos) {
    fEntryType = EntryType::M_UINT;
  } 
  /// Type: Double3
  else if(fEntryTypeStr.find("Double3") != std::string::npos) {
    fEntryType = EntryType::M_DOUBLE3;
  } 
  /// other types
  else {
    fEntryType = EntryType::M_OTHER;
  }
}

Bool_t Entry::CheckIfValid() {
  return kTRUE;
}

std::string Entry::GetEntryName() {
  return fEntryName;
}

void Entry::UpdateValue() {
  bdm::Cell* cellPtr = fModelElement->GetElement();
  TClass *cl = cellPtr->IsA();
  std::string methodName(fEntryName);
  methodName.insert(0, "Get");
  TMethodCall call(cl, methodName.c_str(), "");

  switch(fEntryType) {
    case EntryType::M_DOUBLE: {
      Double_t retValDouble = -1;
      call.Execute((void*)cellPtr, nullptr, 0, &retValDouble);
      fNumberEntries[0]->SetNumber(retValDouble);
      fCurrentValues[0] = retValDouble;
      break;
    }
    case EntryType::M_BOOLEAN: {
      // Currently not supported
    }
    case EntryType::M_UINT: {
      uint32_t retValUInt = 0;
      call.Execute((void*)cellPtr, nullptr, 0, &retValUInt);
      fNumberEntries[0]->SetNumber(retValUInt);
      fCurrentValues[0] = retValUInt;
      break;
    }
    case EntryType::M_DOUBLE3: {
      bdm::Double3 retValDouble3;
      call.Execute((void*)cellPtr, nullptr, 0, &retValDouble3);
      std::string xyz("xyz");
      for(int i = 0; i < 3; i++) {
        fCurrentValues[i] = retValDouble3[i];
        fNumberEntries[i]->SetNumber(retValDouble3[i]);
      }
      break;
    }
    case EntryType::M_OTHER:
    default:
      ;
  }
}

// Creates the necessary gui elements to match attribute types
void Entry::Init(ModelElement* modelElement) {
  fModelElement = modelElement;

  TGLayoutHints *fL2 = new TGLayoutHints(kLHintsCenterY | kLHintsLeft, 2, 2, 2, 2);

  switch(fEntryType) {
    case EntryType::M_DOUBLE: {
      TGNumberEntry* fNumberEntry = new TGNumberEntry(this, 1.0);
      AddFrame(fNumberEntry, fL2);
      fNumberEntries.push_back(fNumberEntry);
      fCurrentValues.push_back(0);
      UpdateValue();
      fNumberEntry->Associate(this);
      break;
    }
    case EntryType::M_BOOLEAN: {
      /* Currently not supported
      bool retValBool = false;
      call.Execute((void*)cellPtr, nullptr, 0, &retValBool);
      fButtonGroup = new TGButtonGroup(this, "Boolean Values", kHorizontalFrame);
      fRadioButtonTrue = new TGRadioButton(fButtonGroup, new TGHotString("True"));
      fRadioButtonFalse = new TGRadioButton(fButtonGroup, new TGHotString("False"));
      if(retValBool) {
        fRadioButtonTrue->SetState(kButtonDown);
        Log::Debug("\t\tbool: TRUE");
      } else {
        fRadioButtonFalse->SetState(kButtonDown);
        Log::Debug("\t\tbool: FALSE");
      }
      fCurrentValues.push_back(retValBool);
      AddFrame(fButtonGroup, fL2);
      */
      break;
    }
    case EntryType::M_UINT: {
      TGNumberEntry* fNumberEntry = new TGNumberEntry(this, 1.0);
      AddFrame(fNumberEntry, fL2);
      fNumberEntries.push_back(fNumberEntry);
      fNumberEntry->SetNumStyle(TGNumberFormat::kNESInteger);
      fNumberEntry->SetNumAttr(TGNumberFormat::kNEANonNegative);
      fCurrentValues.push_back(0);
      UpdateValue();
      fNumberEntry->Associate(this);
      break;
    } 
    case EntryType::M_DOUBLE3: {
      std::string xyz("xyz");
      for(int i = 0; i < 3; i++) {
        TGNumberEntry* fNumberEntry = new TGNumberEntry(this, 1.0);
        AddFrame(fNumberEntry, fL2);
        fNumberEntry->SetNumber(i);
        fNumberEntries.push_back(fNumberEntry);
        fCurrentValues.push_back(i);
        fNumberEntry->Associate(this);
        std::string tmpLabelName = std::string() + xyz.at(i);
        Log::Debug("tmpLabelName on iteration #", i, "=", tmpLabelName);
        TGLabel *tmpLabel = new TGLabel(this, tmpLabelName.c_str());
        AddFrame(tmpLabel, fL2);
      }
      UpdateValue();
      break;
    } 
    case EntryType::M_OTHER:
    default:
      Log::Info("Arg Type is not yet supported");
  }

  TGLabel *label = new TGLabel(this, fEntryName.c_str());
  AddFrame(label, fL2);
}

Bool_t Entry::ProcessMessage(Long_t msg, Long_t param1, Long_t param2) {
  Log::Debug("(From ", fEntryName, ") - Processing msg:", msg, ", parm1:", param1, ", parm2:", param2);

  if(CheckIfValueChanged()) {
    Log::Debug("Value has changed!");
    /// Will now call setter for the current attribute
    bdm::Cell* cellPtr = fModelElement->GetElement();
    TClass *cl = cellPtr->IsA();
    std::string methodName(fEntryName);
    methodName.insert(0, "Set");
    
    switch(fEntryType) {
      case EntryType::M_DOUBLE3: {
        bdm::Double3 setVal({fCurrentValues[0], fCurrentValues[1], fCurrentValues[2]});
        TMethod* method = cl->GetClassMethodWithPrototype(methodName.c_str(), "const Double3&");
        if(method == 0) {
          Log::Warning("Could not find method:`", methodName, "`, defaulting to direct calls..");
          if(fEntryName.compare("Position") == 0) {
            cellPtr->SetPosition(setVal);
          } else if (fEntryName.compare("TractorForce") == 0) {
            cellPtr->SetTractorForce(setVal);
          } else {
            Log::Error("Could not set `", fEntryName, "`");
          }
        } else {
          TMethodCall call(method);
          call.SetParamPtrs((void*)&setVal, 1);
          call.Execute(cellPtr);
        }
        break;
      }
      case EntryType::M_UINT: {
        TMethod* method = cl->GetClassMethodWithPrototype(methodName.c_str(), "uint32_t");
        if(method != 0) {
          std::cout << "Found method!!!\n";
        }
        TMethodCall call(method);
        std::string setVal(std::to_string(fCurrentValues[0]));
        call.Execute((void*)cellPtr, setVal.c_str());
        break;
      } 
      case EntryType::M_DOUBLE: {
        std::string setVal(std::to_string(fCurrentValues[0]));
        Log::Debug("Method Name:", methodName);
        TMethod* method = cl->GetClassMethodWithPrototype(methodName.c_str(), "double");
        if(method != 0) {
          std::cout << "Found method!!!\n";
        }
        TMethodCall call(method);
        const char* signature = method->GetSignature();
        std::cout << "Signature recieved: " << signature << "\n";
        call.Execute((void*)cellPtr, setVal.c_str());
        break;
      }
      case EntryType::M_BOOLEAN:
      case EntryType::M_OTHER:
      default:
        ;
    }
    fParentInspector->UpdateAllEntries();
  }
  return kTRUE;
}

Bool_t Entry::CheckIfValueChanged() {
  Bool_t isModified = kFALSE;
  if(fCurrentValues.size() != 0) {
    Size_t valuesCount = fNumberEntries.size();
    Double_t entryValue;
    uint32_t i; 
    switch(fEntryType) {
      case EntryType::M_DOUBLE:
      case EntryType::M_DOUBLE3:
      case EntryType::M_UINT:
        for(i = 0; i < valuesCount; i++) {
          entryValue = fNumberEntries[i]->GetNumber();
          if(fCurrentValues[i] != entryValue) {
            isModified = kTRUE;
            fCurrentValues[i] = entryValue;
          }
        }
        break;
      case EntryType::M_BOOLEAN:
      case EntryType::M_OTHER:
      default:
        ;
    }
  }
  return isModified;
}

} // namespace gui