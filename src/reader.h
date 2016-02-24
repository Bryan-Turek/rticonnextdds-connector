#ifndef RTIDDS_READER_H
#define RTIDDS_READER_H

#include <nan.h>
#include "binding.h"

using namespace v8;
using Nan::AsyncWorker;
using Nan::Null;
using Nan::New;

namespace dds {

  NAN_METHOD(reader_take);
  NAN_METHOD(reader_read);
  NAN_METHOD(reader_take_async);
  NAN_METHOD(reader_read_async);

  NAN_MODULE_INIT(InitReader);

  // Asynchronous Worker class for reader_take_async
  class TakeWorker : public AsyncWorker {
    public:
      TakeWorker(Nan::Callback *callback, rti_connector* pointer, char* readerName)
        : AsyncWorker(callback), pointer(pointer), name(readerName), found(0) {}
      ~TakeWorker() {}

      void Execute() {
        RTIDDSConnector_take(pointer, name);
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

  // Asynchronous Worker class for reader_read_async
  class ReadWorker : public AsyncWorker {
    public:
      ReadWorker(Nan::Callback *callback, rti_connector* pointer, char* readerName)
        : AsyncWorker(callback), pointer(pointer), name(readerName), found(0) {}
      ~ReadWorker() {}

      void Execute() {
        RTIDDSConnector_read(pointer, name);
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
