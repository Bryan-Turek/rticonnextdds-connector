#ifndef RTIDDS_WRITER_H
#define RTIDDS_WRITER_H

#include <nan.h>
#include "binding.h"

using namespace v8;
using Nan::AsyncWorker;
using Nan::Null;
using Nan::New;

namespace dds {

  NAN_METHOD(writer_write);
  NAN_METHOD(writer_write_async);

  NAN_MODULE_INIT(InitWriter);

  // Asynchronous Worker class for writer_write_async
  class WriteWorker : public AsyncWorker {
    public:
      WriteWorker(Nan::Callback *callback, rti_connector* pointer, char* writerName)
        : AsyncWorker(callback), pointer(pointer), name(writerName), found(0) {}
      ~WriteWorker() {}

      void Execute() {
        RTIDDSConnector_write(pointer, name);
      }

      // void HandleOkCallback() {
      //   Nan::HandleScope scope;
      //
      //   v8::Local<v8::Value> argv[] = {
      //     Null(),
      //     New<Boolean>(found)
      //   };
      //
      //   callback->Call(2, argv);
      // }

    private:
      rti_connector* pointer;
      char* name;
      bool found;
  };

}

#endif
