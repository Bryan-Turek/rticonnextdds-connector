#ifndef RTIDDS_CONNECTOR_H
#define RTIDDS_CONNECTOR_H

#include <nan.h>
#include "binding.h"

using namespace v8;
using Nan::AsyncWorker;
using Nan::Null;
using Nan::New;

namespace dds {

  NAN_METHOD(connector_new);
  NAN_METHOD(connector_delete);
  NAN_METHOD(connector_wait_async);

  NAN_MODULE_INIT(InitConnector);

  class WaitWorker : public AsyncWorker {
    public:
      WaitWorker(Nan::Callback *callback, rti_connector* conn, int val)
        : AsyncWorker(callback), conn(conn), val(val), found(0) {}
      ~WaitWorker() {}

      void Execute() {
        found = RTIDDSConnector_wait(conn, val);
      }

      void HandleOkCallback() {
        Nan::HandleScope scope;

        v8::Local<v8::Value> argv[] = {
          Null(),
          New<Boolean>(found)
        };

        callback->Call(2, argv);
      }

    private:
      rti_connector* conn;
      int val;
      bool found;
  };

} // end namespace dds

#endif
