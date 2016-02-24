#include <nan.h>
#include "writer.h"
#include "utils.h"

using v8::FunctionTemplate;
using Nan::ThrowReferenceError;
using Nan::New;
using Nan::GetFunction;
using namespace dds;

NAN_METHOD(dds::writer_write) {

  CHECK_ARG_LENGTH(info, 2);
  CHECK_ARG_TYPES(!info[0]->IsObject() || !info[1]->IsString());

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  v8::String::Utf8Value writerName(info[1]->ToString());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("writer_write: Unable to reference RTIDDSConnector object");
    return;
  }

  RTIDDSConnector_write(conn->Pointer(), *writerName);

}

NAN_METHOD(dds::writer_write_async) {

  CHECK_ARG_LENGTH(info, 3);
  CHECK_ARG_TYPES(!info[0]->IsObject() || !info[1]->IsString() || !info[2]->IsFunction());

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  v8::String::Utf8Value writerName(info[1]->ToString());

  Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("writer_write_async: Unable to reference RTIDDSConnector object");
    return;
  }

  Nan::AsyncQueueWorker(new WriteWorker(callback, conn->Pointer(), *writerName));

}

// NAN_METHOD(writer_write) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 2, "writer_write expects 2 arguments");
//   // RDC_THROW_EXCEPTION(args[0]->IsObject(), "arg[1] must be an object");
//   // RDC_THROW_EXCEPTION(args[1]->IsString(), "arg[2] must be a string");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value writerName(args[1]->ToString());
//
//   RTIDDSConnector_write(conn->pointer, *writerName);
//
//   NanReturnUndefined();
// }

NAN_MODULE_INIT(dds::InitWriter) {

  Nan::Set(target,
    New("writer_write").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(writer_write)).ToLocalChecked()
  );

  Nan::Set(target,
    New("writer_write_async").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(writer_write_async)).ToLocalChecked()
  );

}
