#include <nan.h>
#include "samples.h"
#include "utils.h"

using v8::FunctionTemplate;
using Nan::ThrowReferenceError;
using Nan::New;
using Nan::GetFunction;
using namespace dds;

// Get sample attributes:
NAN_METHOD(dds::get_samples_length) {
  assert(info.Length() == 2);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());

  double length = RTIDDSConnector_getSamplesLength(conn->Pointer(), *readerName);
  info.GetReturnValue().Set(Nan::New(length));
}

NAN_METHOD(dds::get_number_from_samples) {
  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());
  int index = (int)info[2]->NumberValue();
  v8::String::Utf8Value fieldName(info[3]->ToString());

  double number = RTIDDSConnector_getNumberFromSamples(conn->Pointer(),*readerName,index,*fieldName);
  info.GetReturnValue().Set(Nan::New(number));
}

NAN_METHOD(dds::get_boolean_from_samples) {
  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());
  int index = (int)info[2]->NumberValue();
  v8::String::Utf8Value fieldName(info[3]->ToString());

  bool boolean = RTIDDSConnector_getBooleanFromSamples(conn->Pointer(),*readerName,index,*fieldName);
  info.GetReturnValue().Set(Nan::New(boolean));
}

NAN_METHOD(dds::get_string_from_samples) {

  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());
  int index = (int)info[2]->NumberValue();
  v8::String::Utf8Value fieldName(info[3]->ToString());

  const char* string = RTIDDSConnector_getStringFromSamples(conn->Pointer(),*readerName,index,*fieldName);
  info.GetReturnValue().Set(Nan::New(string).ToLocalChecked());
}

NAN_METHOD(dds::get_json_sample) {
  assert(info.Length() == 3);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());
  int index = (int)info[2]->NumberValue();

  char* json = RTIDDSConnector_getJSONSample(conn->Pointer(),*readerName,index);
  info.GetReturnValue().Set(Nan::New(json).ToLocalChecked());
}

// Set sample attributes:
NAN_METHOD(dds::set_number_into_samples) {
  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value writerName(info[1]->ToString());
  v8::String::Utf8Value fieldName(info[2]->ToString());
  double value = info[3]->NumberValue();

  RTIDDSConnector_setNumberIntoSamples(conn->Pointer(),*writerName,*fieldName,value);
}

NAN_METHOD(dds::set_string_into_samples) {
  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value writerName(info[1]->ToString());
  v8::String::Utf8Value fieldName(info[2]->ToString());
  v8::String::Utf8Value value(info[3]->ToString());

  RTIDDSConnector_setStringIntoSamples(conn->Pointer(),*writerName,*fieldName,*value);
}

NAN_METHOD(dds::set_boolean_into_samples) {
  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value writerName(info[1]->ToString());
  v8::String::Utf8Value fieldName(info[2]->ToString());
  int value = (int)info[3]->BooleanValue();

  RTIDDSConnector_setBooleanIntoSamples(conn->Pointer(),*writerName,*fieldName,value);
}

// Get info attributes:
NAN_METHOD(dds::get_infos_length) {
  assert(info.Length() == 2);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());

  double length = RTIDDSConnector_getSamplesLength(conn->Pointer(), *readerName);
  info.GetReturnValue().Set(Nan::New(length));
}

NAN_METHOD(dds::get_boolean_from_infos) {
  assert(info.Length() == 4);
  ConnectorObject* conn = Nan::ObjectWrap::Unwrap<ConnectorObject>(info[0]->ToObject());
  assert(conn->Pointer() != NULL);

  v8::String::Utf8Value readerName(info[1]->ToString());
  int index = (int)info[2]->NumberValue();
  v8::String::Utf8Value value(info[3]->ToString());

  bool boolean = RTIDDSConnector_getBooleanFromInfos(conn->Pointer(),*readerName,index,*value);
  info.GetReturnValue().Set(Nan::New(boolean));
}

// NAN_METHOD(samples_length) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 2, "samples_length expects 2 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//
//   double length = RTIDDSConnector_getSamplesLength(conn->pointer, *readerName);
//
//   NanReturnValue(NanNew<Number>(length));
// }
//
// NAN_METHOD(number_from_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "number_from_samples expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//   int index = (int)args[2]->NumberValue();
//   v8::String::Utf8Value fieldName(args[3]->ToString());
//
//   double number = RTIDDSConnector_getNumberFromSamples(conn->pointer,*readerName,index,*fieldName);
//
//   NanReturnValue(NanNew<Number>(number));
// }
//
// NAN_METHOD(boolean_from_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "boolean_from_samples expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//   int index = (int)args[2]->NumberValue();
//   v8::String::Utf8Value fieldName(args[3]->ToString());
//
//   bool boolean = (bool)RTIDDSConnector_getBooleanFromSamples(conn->pointer,*readerName,index,*fieldName);
//
//   NanReturnValue(NanNew<Boolean>(boolean));
// }
//
// NAN_METHOD(string_from_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "string_from_samples expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//   int index = (int)args[2]->NumberValue();
//   v8::String::Utf8Value fieldName(args[3]->ToString());
//
//   char* string = RTIDDSConnector_getStringFromSamples(conn->pointer,*readerName,index,*fieldName);
//
//   NanReturnValue(NanNew<String>(string));
// }
//
// NAN_METHOD(json_from_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 3, "json_from_samples expects 3 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//   int index = (int)args[2]->NumberValue();
//
//   char* string = RTIDDSConnector_getJSONSample(conn->pointer,*readerName,index);
//
//   NanReturnValue(NanNew<String>(string));
// }
//
// NAN_METHOD(infos_length) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 2, "infos_length expects 2 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//
//   double length = RTIDDSConnector_getInfosLength(conn->pointer,*readerName);
//
//   NanReturnValue(NanNew<Number>(length));
// }
//
// NAN_METHOD(boolean_from_infos) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "boolean_from_infos expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value readerName(args[1]->ToString());
//   int index = (int)args[2]->NumberValue();
//   v8::String::Utf8Value value(args[3]->ToString());
//
//   bool boolean = (bool)RTIDDSConnector_getBooleanFromInfos(conn->pointer,*readerName,index,*value);
//
//   NanReturnValue(NanNew<Boolean>(boolean));
// }

//
// NAN_METHOD(number_into_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "number_into_samples expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value writerName(args[1]->ToString());
//   v8::String::Utf8Value fieldName(args[2]->ToString());
//
//   double number = args[3]->NumberValue();
//
//   RTIDDSConnector_setNumberIntoSamples(conn->pointer,*writerName,*fieldName,number);
//
//   NanReturnUndefined();
// }
//
// NAN_METHOD(string_into_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "string_into_samples expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   //I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value writerName(args[1]->ToString());
//   v8::String::Utf8Value fieldName(args[2]->ToString());
//   v8::String::Utf8Value value(args[3]->ToString());
//
//   RTIDDSConnector_setStringIntoSamples(conn->pointer,*writerName,*fieldName,*value);
//
//   NanReturnUndefined();
// }
//
// NAN_METHOD(boolean_into_samples) {
//   NanScope();
//
//   RDC_THROW_EXCEPTION(args.Length() == 4, "boolean_into_samples expects 4 arguments");
//
//   ObjectHandle<rti_connector>* conn = ObjectHandle<rti_connector>::Unwrap(args[0]);
//
//   // I can't even:
//   NULL_POINTER_EXCEPTION(conn->pointer != NULL);
//
//   v8::String::Utf8Value writerName(args[1]->ToString());
//   v8::String::Utf8Value fieldName(args[2]->ToString());
//   int value = (int)args[3]->BooleanValue();
//
//   RTIDDSConnector_setBooleanIntoSamples(conn->pointer,*writerName,*fieldName,value);
//
//   NanReturnUndefined();
// }
//

NAN_MODULE_INIT(dds::InitSamples) {

  // Get sample attributes:
  Nan::Set(target,
    New("get_samples_length").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_samples_length)).ToLocalChecked()
  );

  Nan::Set(target,
    New("get_number_from_samples").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_number_from_samples)).ToLocalChecked()
  );

  Nan::Set(target,
    New("get_string_from_samples").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_string_from_samples)).ToLocalChecked()
  );

  Nan::Set(target,
    New("get_boolean_from_samples").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_boolean_from_samples)).ToLocalChecked()
  );

  Nan::Set(target,
    New("get_json_sample").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_json_sample)).ToLocalChecked()
  );

  // Set sample attributes:
  Nan::Set(target,
    New("set_number_into_samples").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(set_number_into_samples)).ToLocalChecked()
  );

  Nan::Set(target,
    New("set_string_into_samples").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(set_string_into_samples)).ToLocalChecked()
  );

  Nan::Set(target,
    New("set_boolean_into_samples").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(set_boolean_into_samples)).ToLocalChecked()
  );

  // Get info attributes:
  Nan::Set(target,
    New("get_infos_length").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_infos_length)).ToLocalChecked()
  );

  Nan::Set(target,
    New("get_boolean_from_infos").ToLocalChecked(),
    GetFunction(New<FunctionTemplate>(get_boolean_from_infos)).ToLocalChecked()
  );

}
