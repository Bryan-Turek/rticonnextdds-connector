#include <nan.h>
#include "reader.h"
#include "utils.h"

using v8::FunctionTemplate;
using Nan::ThrowReferenceError;
using Nan::New;
using Nan::GetFunction;
using namespace dds;

NAN_METHOD(dds::reader_take) {
  CHECK_ARG_LENGTH(info, 2);
  CHECK_ARG_TYPES(!info[0]->IsObject() || !info[1]->IsString());

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  v8::String::Utf8Value readerName(info[1]->ToString());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("reader_take: Unable to reference RTIDDSConnector object");
    return;
  }

  RTIDDSConnector_take(conn->Pointer(), *readerName);
}

NAN_METHOD(dds::reader_read) {
  CHECK_ARG_LENGTH(info, 2);
  CHECK_ARG_TYPES(!info[0]->IsObject() || !info[1]->IsString());

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  v8::String::Utf8Value readerName(info[1]->ToString());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("reader_take: Unable to reference RTIDDSConnector object");
    return;
  }

  RTIDDSConnector_read(conn->Pointer(), *readerName);
}

NAN_METHOD(dds::reader_take_async) {
  CHECK_ARG_LENGTH( info, 3 );
  CHECK_ARG_TYPES( !info[0]->IsObject() || !info[1]->IsString() || !info[2]->IsFunction());

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  v8::String::Utf8Value readerName(info[1]->ToString());

  Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("reader_take: Unable to reference RTIDDSConnector object");
    return;
  }

  Nan::AsyncQueueWorker(new TakeWorker(callback, conn->Pointer(), *readerName));
}

NAN_METHOD(dds::reader_read_async) {
  CHECK_ARG_LENGTH( info, 3 );
  CHECK_ARG_TYPES( !info[0]->IsObject() || !info[1]->IsString() || !info[2]->IsFunction());

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  v8::String::Utf8Value readerName(info[1]->ToString());

  Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("reader_take: Unable to reference RTIDDSConnector object");
    return;
  }

  Nan::AsyncQueueWorker(new ReadWorker(callback, conn->Pointer(), *readerName));
}

// NAN_METHOD(reader_take) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 2, "reader_take expects 2 arguments");
//   // RDC_THROW_EXCEPTION(args[0]->IsObject(), "arg[1] must be an object");
//   // RDC_THROW_EXCEPTION(args[1]->IsString(), "arg[2] must be a string");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//
//   //Please don't let it be:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   RTIDDSConnector_take(conn->pointer, *readerName);
//
//   NanReturnUndefined();
// }
//
// NAN_METHOD(reader_read) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 2, "reader_take expects 2 arguments");
//   // RDC_THROW_EXCEPTION(args[0]->IsObject(), "arg[1] must be an object");
//   // RDC_THROW_EXCEPTION(args[1]->IsString(), "arg[2] must be a string");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//
//   // That wouldn't be good:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   RTIDDSConnector_read(conn->pointer, *readerName);
//
//   NanReturnUndefined();
// }

NAN_MODULE_INIT(dds::InitReader) {

  // Initialize Reader methods

  Nan::Set(target,
    New("reader_take").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(reader_take)).ToLocalChecked()
  );

  Nan::Set(target,
    New("reader_read").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(reader_read)).ToLocalChecked()
  );

  Nan::Set(target,
    New("reader_take_async").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(reader_take_async)).ToLocalChecked()
  );

  Nan::Set(target,
    New("reader_read_async").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(reader_read_async)).ToLocalChecked()
  );

}
