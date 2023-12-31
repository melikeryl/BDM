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

#ifndef CORE_OPERATION_LOAD_BALANCING_OP_H_
#define CORE_OPERATION_LOAD_BALANCING_OP_H_

#include "core/operation/operation.h"
#include "core/resource_manager.h"
#include "core/simulation.h"

namespace bdm {

/// A operation that balances the agents among the available NUMA
/// domains in order to minimize crosstalk. This operation invalidates the
/// AgentHandles in the ResourceManager
struct LoadBalancingOp : public StandaloneOperationImpl {
  BDM_OP_HEADER(LoadBalancingOp);

  void operator()() override {
    auto* rm = Simulation::GetActive()->GetResourceManager();
    rm->LoadBalance();
  }
};

}  // namespace bdm

#endif  // CORE_OPERATION_LOAD_BALANCING_OP_H_
