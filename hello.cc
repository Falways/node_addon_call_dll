// hello.cc
#include <node.h>
#include <windows.h>
#include <tchar.h>
#include "stdio.h"

namespace sdp {

using v8::Context;
using v8::Function;
using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::NewStringType;
using v8::Null;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;

void Method(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(
      isolate, "world", NewStringType::kNormal).ToLocalChecked());
}

void Add(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() < 2) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"invalid param counts",NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
    isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"参数错误",NewStringType::kNormal).ToLocalChecked()));
    return;
  }

  double value = args[0].As<Number>()->Value() + args[1].As<Number>()->Value();
  Local<Number> num = Number::New(isolate, value);

  args.GetReturnValue().Set(num);
}

void RunCallback(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();
  Local<Function> cb = Local<Function>::Cast(args[0]);
  const unsigned argc = 1;
  Local<Value> argv[argc] = { String::NewFromUtf8(isolate, "C callback value", NewStringType::kNormal).ToLocalChecked() };
  cb->Call(context, Null(isolate), argc, argv).ToLocalChecked();
}

// obj factory
void CreateObject(const FunctionCallbackInfo<Value>& args) {

  Isolate* isolate = args.GetIsolate();
  Local<Context> context = isolate->GetCurrentContext();

  Local<Object> obj = Object::New(isolate);
  obj->Set(context, String::NewFromUtf8(isolate, "msg", NewStringType::kNormal).ToLocalChecked(), args[0]->ToString(context).ToLocalChecked()).FromJust();

  args.GetReturnValue().Set(obj);
}

// call dll
void LoadDLL(const FunctionCallbackInfo<Value>& args) {
    DWORD dwError = 0;
    LPSTR lpStr = _T("C:\\Users\\xh\\Desktop\\work5\\node_call_dll\\food.dll");
    HINSTANCE hDll = LoadLibrary(lpStr);
    dwError = GetLastError();
    printf("dwError = ");
    printf("%d\n", dwError);

    if(hDll != NULL) {
        printf("%s\n", "load ok!");
        typedef int(*GetSum)(int,int);
        GetSum getSum = (GetSum)GetProcAddress(hDll,"main");
        if (getSum==NULL) {
            FreeLibrary(hDll); // 释放内存
        }
        printf("%s\n","get two nums sum (5,6)");
        int finalResult = getSum(5,6);
        printf("%d\n", finalResult);
        FreeLibrary(hDll); // 释放内存
    }else {
        printf("%s\n", "load failure!");
    }
}

void Initialize(Local<Object> exports) {
  NODE_SET_METHOD(exports, "hello", Method);
  NODE_SET_METHOD(exports, "add", Add);
  NODE_SET_METHOD(exports, "callFuc", RunCallback);
  NODE_SET_METHOD(exports, "getObj", CreateObject);
  NODE_SET_METHOD(exports, "loadDll", LoadDLL);
}

NODE_MODULE(NODE_GYP_MODULE_NAME, Initialize)

}