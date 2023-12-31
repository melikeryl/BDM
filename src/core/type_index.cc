// -----------------------------------------------------------------------------
//
// Copyright (C) 2021 CERN & University of Surrey for the benefit of the
// BioDynaMo collaboration. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
//
// See the LICENSE file distributed with this work for details.
// See the NOTICE file distributed with this work for additional information
// regarding copyright ownership.
//
// -----------------------------------------------------------------------------

#include "core/type_index.h"

#include <TClass.h>

namespace bdm {

// -----------------------------------------------------------------------------
void TypeIndex::Add(Agent* agent) {
  auto& type_vector = data_[agent->IsA()];
  auto uid = agent->GetUid();
  if (index_.size() <= uid.GetIndex()) {
    Reserve(uid.GetIndex() + 1);
  }
  index_.Insert(uid, type_vector.size());
  type_vector.push_back(agent);
}

// -----------------------------------------------------------------------------
void TypeIndex::Update(Agent* new_agent) {
  auto idx = index_[new_agent->GetUid()];
  auto& type_vector = data_[new_agent->IsA()];
  type_vector[idx] = new_agent;
}

// -----------------------------------------------------------------------------
void TypeIndex::Remove(Agent* agent) {
  auto idx = index_[agent->GetUid()];
  auto& type_vector = data_[agent->IsA()];
  if (idx == type_vector.size() - 1) {
    type_vector.pop_back();
  } else {
    // swap
    auto* reordered = type_vector.back();
    type_vector[idx] = reordered;
    type_vector.pop_back();
    index_.Insert(reordered->GetUid(), idx);
  }
}

// -----------------------------------------------------------------------------
void TypeIndex::Clear() {
  index_.clear();
  for (auto& pair : data_) {
    pair.second.clear();
  }
}

// -----------------------------------------------------------------------------
void TypeIndex::Reserve(uint64_t capacity) {
  if (index_.size() < capacity) {
    index_.resize(capacity * 1.5);
  }
}

// -----------------------------------------------------------------------------
const std::vector<Agent*>& TypeIndex::GetType(TClass* tclass) const {
  return data_[tclass];
}

}  // namespace bdm
