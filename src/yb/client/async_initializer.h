// Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.

#ifndef YB_CLIENT_ASYNC_INITIALIZER_H_
#define YB_CLIENT_ASYNC_INITIALIZER_H_

#include <future>

#include "yb/client/client.h"

#include "yb/server/server_base_options.h"

namespace yb {
namespace client {

class AsyncClientInitialiser {
 public:
  AsyncClientInitialiser(const std::string& client_name, const uint32_t num_reactors,
                   const uint32_t timeout_seconds, const std::string& tserver_uuid,
                   const server::ServerBaseOptions* opts,
                   scoped_refptr<MetricEntity> metric_entity);

  ~AsyncClientInitialiser();

  void Shutdown() { stopping_ = true; }

  const std::shared_ptr<client::YBClient>& client() const;
  const std::shared_future<client::YBClientPtr>& get_client_future() const {
    return client_future_;
  }

 private:
  void InitClient();

  YBClientBuilder client_builder_;
  std::promise<client::YBClientPtr> client_promise_;
  mutable std::shared_future<client::YBClientPtr> client_future_;

  std::thread init_client_thread_;
  std::atomic<bool> stopping_ = {false};
};

}  // namespace client
}  // namespace yb

#endif // YB_CLIENT_ASYNC_INITIALIZER_H_
