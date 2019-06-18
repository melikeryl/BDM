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

#ifndef GUI_MODEL_H_
#define GUI_MODEL_H_

#include <string>
#include <vector>

#include <TROOT.h>
#include <TClass.h>
#include <TFile.h>
#include <TSystem.h>
#include <TSystemDirectory.h>
#include <TEnv.h>
#include <TApplication.h>
#include <TVirtualX.h>
#include <RQ_OBJECT.h>
#include "TObject.h"
#include "TString.h"

enum SimulationState { kIDLE ,kSIMULATING, kDONE };

class ModelElement;

class Model : public TObject {
 public:

  Model(const char* name) : TObject() { fModelName = name; }
 ~Model() = default;

  const char*   fModelName;
  Bool_t        fModified;

  void          CreateModel();
  void          SaveModel();
  void          SimulateModel();
  void          UpdateModel(std::string elementName, ModelElement& element);
  void          IsElementNameAvailable();
  Bool_t        CreateElement(const char* parent, const char* name, int type);
 
 private:
  TList         *fModelElements;
  
  std::string   GenerateCode();
  void          InitializeElement(ModelElement* parent, const char* name, int type);
  ModelElement* FindElement(const char* elementName);

  ClassDef(Model,1)
};

#endif // GUI_MODEL_H_