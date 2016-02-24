#include <nan.h>
#include "connector.h"
#include "utils.h"

using v8::FunctionTemplate;
using Nan::ThrowReferenceError;
using Nan::New;
using Nan::GetFunction;
using namespace dds;

ConnectorObject::ConnectorObject() {};
ConnectorObject::~ConnectorObject() {};

Nan::Persistent<v8::Function> ConnectorObject::constructor;

void ConnectorObject::Init() {
  Nan::HandleScope scope;

  // Prepare constructor template
  v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
  tpl->SetClassName(Nan::New("ConnectorObject").ToLocalChecked());
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  constructor.Reset(Nan::GetFunction(tpl).ToLocalChecked());
}


void ConnectorObject::New(const Nan::FunctionCallbackInfo<v8::Value>& info) {
  CHECK_ARG_LENGTH(info, 2);
  CHECK_ARG_TYPES(!info[0]->IsString() || !info[1]->IsString());

  ConnectorObject* obj = new ConnectorObject();

  v8::String::Utf8Value configName(info[0]->ToString());
  v8::String::Utf8Value fileName(info[1]->ToString());

  obj->pointer_ = RTIDDSConnector_new(*configName, *fileName);

  if( obj->pointer_ == NULL ) {
    ThrowReferenceError("Unable to create RTIDDSConnector object");
    return;
  }

  obj->Wrap(info.This());

  info.GetReturnValue().Set(info.This());
}

v8::Local<v8::Object> ConnectorObject::NewInstance(v8::Local<v8::Value> configName, v8::Local<v8::Value> fileName) {
  Nan::EscapableHandleScope scope;

  const unsigned argc = 2;
  v8::Local<v8::Value> argv[argc] = { configName, fileName };
  v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
  v8::Local<v8::Object> instance = cons->NewInstance(argc, argv);

  return scope.Escape(instance);
}

NAN_METHOD(dds::object_new) {
  info.GetReturnValue().Set(info.This());
}

NAN_METHOD(dds::connector_new) {
  // CHECK_ARG_LENGTH(info, 2);
  // CHECK_ARG_TYPES(!info[0]->IsString() || !info[1]->IsString());
  //
  // //create object handler for rti_connector struct.
  // ObjectHandle<rti_connector>* conn = new ObjectHandle<rti_connector>("rti_connector");
  //
  // v8::String::Utf8Value configName(info[0]->ToString());
  // v8::String::Utf8Value fileName(info[1]->ToString());
  //
  // conn->pointer = RTIDDSConnector_new(*configName, *fileName);
  //
  // if( conn->pointer == NULL ) {
  //   ThrowReferenceError("Unable to create RTIDDSConnector object");
  //   return;
  // }
  //
  // v8::Local<v8::Function> cons = Nan::New<v8::Function>(conn->constructor);
  // info.GetReturnValue().Set(cons->NewInstance());
  info.GetReturnValue().Set(ConnectorObject::NewInstance(info[0], info[1]));
}

NAN_METHOD(dds::connector_delete) {

  CHECK_ARG_LENGTH(info, 1);

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("Unable to find RTIDDSConnector object to delete");
  }

  RTIDDSConnector_delete(conn->Pointer());

  // now we can delete the pointer;
  // delete conn->pointer;
  // conn->pointer = NULL;
}

NAN_METHOD(dds::connector_wait_async) {
  CHECK_ARG_LENGTH(info, 3);

  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());

  if( conn->Pointer() == NULL ) {
    ThrowReferenceError("Unable to find RTIDDSConnector object to delete");
  }

  int value = (int)info[1]->NumberValue();
  Nan::Callback *callback = new Nan::Callback(info[2].As<Function>());

  Nan::AsyncQueueWorker(new WaitWorker(callback, conn->Pointer(), value));
}

NAN_MODULE_INIT(dds::InitConnector) {

  ConnectorObject::Init();

  // Initialize Connector methods:
  Nan::Set(target,
    New("connector_new").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(connector_new)).ToLocalChecked()
  );

  Nan::Set(target,
    New("connector_delete").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(connector_delete)).ToLocalChecked()
  );

  Nan::Set(target,
    New("connector_wait_async").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(connector_wait_async)).ToLocalChecked()
  );

}
