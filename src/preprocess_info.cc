#include "preprocess_info.h"
#include "utils.h"

#include <napi.h>
#include <uv.h>

using namespace Napi;

namespace ie = InferenceEngine;

namespace ienodejs {

Napi::FunctionReference PreProcessInfo::constructor;

void PreProcessInfo::Init(const Napi::Env& env) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(
      env, "PreProcessInfo",
      {InstanceMethod("getColorFormat", &PreProcessInfo::GetColorFormat),
       InstanceMethod("setColorFormat", &PreProcessInfo::SetColorFormat),
       InstanceMethod("getResizeAlgorithm", &PreProcessInfo::GetResizeAlgorithm),
       InstanceMethod("setResizeAlgorithm", &PreProcessInfo::SetResizeAlgorithm),
       InstanceMethod("getMeanVariant", &PreProcessInfo::GetMeanVariant),
       InstanceMethod("setVariant", &PreProcessInfo::SetVariant),
       InstanceMethod("getNumberOfChannels", &PreProcessInfo::GetNumberOfChannels)
       });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();
}

PreProcessInfo::PreProcessInfo(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<PreProcessInfo>(info) {}

Napi::Object PreProcessInfo::NewInstance(const Napi::Env& env,
                                         ie::PreProcessInfo& actual) {
  Napi::EscapableHandleScope scope(env);
  Napi::Object obj = constructor.New({});
  PreProcessInfo* preInfo = Napi::ObjectWrap<PreProcessInfo>::Unwrap(obj);

  preInfo->actual_ = &actual;
  return scope.Escape(napi_value(obj)).ToObject();
}

void PreProcessInfo::SetColorFormat(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string colorFormatName = info[0].ToString().Utf8Value();

  if (!utils::IsColorFormatName(colorFormatName)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  actual_->setColorFormat(utils::GetColorFormatByName(colorFormatName));
}

Napi::Value PreProcessInfo::GetColorFormat(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfColorFormat(actual_->getColorFormat()));
}

Napi::Value PreProcessInfo::GetNumberOfChannels(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::Number::New(
      env, actual_->getNumberOfChannels());
}

void PreProcessInfo::SetResizeAlgorithm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string resizeAlgorithm = info[0].ToString().Utf8Value();

  if (!utils::IsResizeAlgorithmName(resizeAlgorithm)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  actual_->setResizeAlgorithm(utils::GetResizeAlgorithmByName(resizeAlgorithm));
}

Napi::Value PreProcessInfo::GetResizeAlgorithm(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfResizeAlgorithm(actual_->getResizeAlgorithm()));
}

void PreProcessInfo::SetVariant(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (info.Length() != 1) {
    Napi::TypeError::New(env, "Wrong number of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Wrong type of arguments")
        .ThrowAsJavaScriptException();
    return;
  }

  std::string meanvariant = info[0].ToString().Utf8Value();

  if (!utils::IsMeanVariantName(meanvariant)) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  actual_->setVariant(utils::GetMeanVariantByName(meanvariant));
}

Napi::Value PreProcessInfo::GetMeanVariant(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  if (info.Length() > 0) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return Napi::Object::New(env);
  }

  return Napi::String::New(
      env, utils::GetNameOfMeanVariant(actual_->getMeanVariant()));
}
}  // namespace ienodejs