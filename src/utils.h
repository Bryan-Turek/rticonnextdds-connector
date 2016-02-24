#ifndef RTIDDS_UTILS_H
#define RTIDDS_UTILS_H

#include <nan.h>
#include <node_object_wrap.h>
#include "binding.h"

#define CHECK_ARG_LENGTH(info, count) if (info.Length() != count) { \
  Nan::ThrowTypeError("Incorrect number of arguments"); \
  return; \
}

#define CHECK_ARG_TYPES(logic) if(logic) { \
  Nan::ThrowTypeError("Incorrect arguments"); \
  return; \
}

namespace dds {

  /**
   * A function to use as a JS function that does nothing and returns this
   */
   static inline NAN_METHOD(object_new);

   class ConnectorObject : public Nan::ObjectWrap {
     public:
      static void Init();
      static v8::Local<v8::Object> NewInstance(v8::Local<v8::Value> configName, v8::Local<v8::Value> fileName);
      rti_connector* Pointer() const { return pointer_; }

     private:
      ConnectorObject();
      ~ConnectorObject();

      static Nan::Persistent<v8::Function> constructor;
      static void New(const Nan::FunctionCallbackInfo<v8::Value>& info);
      rti_connector* pointer_;
    };


  /**
  * This utility class allows to keep track of a C pointer that we attached
  * to a JS object. It differs from node's ObjectWrap in the fact that it
  * does not need a constructor and both attributes are public.
  * Node's ObjectWrap is useful to wrap C++ classes whereas this class is useful
  * to wrap C structs. THIS CLASS DOES NOT MANAGE C MEMORY ALLOCATION
  */
  template <typename T>
   class ObjectHandle : public Nan::ObjectWrap {
     public:
       /**
        * @constructor
        * Create a new ObjectHandle object with the given name
        * the name can be used later to identify the wrapped objects
        */
       ObjectHandle(const char* name);

       /**
        * Utility function to retrieve an ObjectHandle from a JS object
        * @param obj, the JS Object in which the ObjectHandle was wrapped
        */
       static ObjectHandle<T>* Unwrap(v8::Handle<v8::Value> obj);

       /**
        * A pointer to the C struct (most often) that we want to wrap
        * We do not allocate this
        */
       T* pointer;

       /**
        *  The JS Object that we set our pointer in
        *  We do create this one
        */
       Nan::Persistent<v8::Function> constructor;

       /**
        * Get the name of the ObjectHandle that we gave it during instanciation
        */
       char* GetName() {
         return *(v8::String::Utf8Value(name_));
       }

     protected:
     private:
       Nan::Persistent<v8::String> name_;

       /**
        * Empty function to set as constructor for an FunctionTemplate
        * @deprecated
        */
       NAN_METHOD(NewObject) {
         // do nothing;
         v8::Local<v8::Function> cons = Nan::New<v8::Function>(constructor);
         info.GetReturnValue().Set(cons->NewInstance());
       }
   };

  template <typename T>
   ObjectHandle<T>::ObjectHandle(const char* name) : pointer(NULL) {
     v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(object_new);
     v8::Local<v8::String> tempName = Nan::New(name == NULL ? "CObject" : name).ToLocalChecked()->ToString();
     name_.Reset(tempName);


     tpl->SetClassName(tempName);
     tpl->InstanceTemplate()->SetInternalFieldCount(1);

     constructor.Reset(tpl->GetFunction());
   }

  template <typename T>
   ObjectHandle<T>* ObjectHandle<T>::Unwrap(v8::Local<v8::Value> obj) {
     assert(obj->IsObject());
     v8::Local<v8::Object> object = obj->ToObject();
     ObjectHandle<T>* ptr = Nan::ObjectWrap::Unwrap<ObjectHandle<T> >(object);
     return ptr;
   }


}

#endif
