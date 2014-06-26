// Copyright (c) 2013 GitHub, Inc. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#include "atom/browser/api/atom_api_window.h"

#include "atom/browser/api/atom_api_web_contents.h"
#include "atom/browser/native_window.h"
#include "atom/common/native_mate_converters/function_converter.h"
#include "base/bind.h"
#include "base/callback.h"
#include "content/public/browser/render_process_host.h"
#include "native_mate/constructor.h"
#include "native_mate/dictionary.h"
#include "ui/gfx/point.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/size.h"

#include "atom/common/node_includes.h"

namespace mate {

template<>
struct Converter<gfx::Rect> {
  static bool FromV8(v8::Isolate* isolate,
                     v8::Handle<v8::Value> val,
                     gfx::Rect* out) {
    if (!val->IsObject())
      return false;
    mate::Dictionary dict(isolate, val->ToObject());
    int x, y, width, height;
    if (!dict.Get("x", &x) || !dict.Get("y", &y) ||
        !dict.Get("width", &width) || !dict.Get("height", &height))
      return false;
    *out = gfx::Rect(x, y, width, height);
    return true;
  }
};

}  // namespace mate

namespace atom {

namespace api {

namespace {

void OnCapturePageDone(
    const base::Callback<void(v8::Handle<v8::Value>)>& callback,
    const std::vector<unsigned char>& data) {
  v8::Locker locker(node_isolate);
  v8::HandleScope handle_scope(node_isolate);

  v8::Local<v8::Value> buffer = node::Buffer::New(
      reinterpret_cast<const char*>(data.data()),
      data.size());
  callback.Run(buffer);
}

}  // namespace


Window::Window(const mate::Dictionary& options)
    : window_(NativeWindow::Create(options)) {
  window_->InitFromOptions(options);
  window_->AddObserver(this);
}

Window::~Window() {
  if (window_)
    Destroy();

  Emit("destroyed");
}

void Window::OnPageTitleUpdated(bool* prevent_default,
                                const std::string& title) {
  base::ListValue args;
  args.AppendString(title);
  *prevent_default = Emit("page-title-updated", args);
}

void Window::WillCloseWindow(bool* prevent_default) {
  *prevent_default = Emit("close");
}

void Window::OnWindowClosed() {
  Emit("closed");

  window_->RemoveObserver(this);
}

void Window::OnWindowBlur() {
  Emit("blur");
}

void Window::OnWindowFocus() {
  Emit("focus");
}

void Window::OnRendererUnresponsive() {
  Emit("unresponsive");
}

void Window::OnRendererResponsive() {
  Emit("responsive");
}

// static
mate::Wrappable* Window::New(const mate::Dictionary& options) {
  return new Window(options);
}

void Window::Destroy() {
  window_->DestroyWebContents();
  window_->CloseImmediately();
}

void Window::Close() {
  window_->Close();
}

void Window::Focus() {
  window_->Focus(true);
}

bool Window::IsFocused() {
  return window_->IsFocused();
}

void Window::Show() {
  window_->Show();
}

void Window::Hide() {
  window_->Hide();
}

bool Window::IsVisible() {
  return window_->IsVisible();
}

void Window::Maximize() {
  window_->Maximize();
}

void Window::Unmaximize() {
  window_->Unmaximize();
}

void Window::Minimize() {
  window_->Minimize();
}

void Window::Restore() {
  window_->Restore();
}

void Window::SetFullscreen(bool fullscreen) {
  window_->SetFullscreen(fullscreen);
}

bool Window::IsFullscreen() {
  return window_->IsFullscreen();
}

void Window::SetSize(int width, int height) {
  window_->SetSize(gfx::Size(width, height));
}

std::vector<int> Window::GetSize() {
  std::vector<int> result(2);
  gfx::Size size = window_->GetSize();
  result[0] = size.width();
  result[1] = size.height();
  return result;
}

void Window::SetContentSize(int width, int height) {
  window_->SetContentSize(gfx::Size(width, height));
}

std::vector<int> Window::GetContentSize() {
  std::vector<int> result(2);
  gfx::Size size = window_->GetContentSize();
  result[0] = size.width();
  result[1] = size.height();
  return result;
}

void Window::SetMinimumSize(int width, int height) {
  window_->SetMinimumSize(gfx::Size(width, height));
}

std::vector<int> Window::GetMinimumSize() {
  std::vector<int> result(2);
  gfx::Size size = window_->GetMinimumSize();
  result[0] = size.width();
  result[1] = size.height();
  return result;
}

void Window::SetMaximumSize(int width, int height) {
  window_->SetMaximumSize(gfx::Size(width, height));
}

std::vector<int> Window::GetMaximumSize() {
  std::vector<int> result(2);
  gfx::Size size = window_->GetMaximumSize();
  result[0] = size.width();
  result[1] = size.height();
  return result;
}

void Window::SetResizable(bool resizable) {
  window_->SetResizable(resizable);
}

bool Window::IsResizable() {
  return window_->IsResizable();
}

void Window::SetAlwaysOnTop(bool top) {
  window_->SetAlwaysOnTop(top);
}

bool Window::IsAlwaysOnTop() {
  return window_->IsAlwaysOnTop();
}

void Window::Center() {
  window_->Center();
}

void Window::SetPosition(int x, int y) {
  window_->SetPosition(gfx::Point(x, y));
}

std::vector<int> Window::GetPosition() {
  std::vector<int> result(2);
  gfx::Point pos = window_->GetPosition();
  result[0] = pos.x();
  result[1] = pos.y();
  return result;
}

void Window::SetTitle(const std::string& title) {
  window_->SetTitle(title);
}

std::string Window::GetTitle() {
  return window_->GetTitle();
}

void Window::FlashFrame(bool flash) {
  window_->FlashFrame(flash);
}

void Window::SetSkipTaskbar(bool skip) {
  window_->SetSkipTaskbar(skip);
}

void Window::SetKiosk(bool kiosk) {
  window_->SetKiosk(kiosk);
}

bool Window::IsKiosk() {
  return window_->IsKiosk();
}

void Window::OpenDevTools() {
  window_->OpenDevTools();
}

void Window::CloseDevTools() {
  window_->CloseDevTools();
}

bool Window::IsDevToolsOpened() {
  return window_->IsDevToolsOpened();
}

void Window::InspectElement(int x, int y) {
  window_->InspectElement(x, y);
}

void Window::FocusOnWebView() {
  window_->FocusOnWebView();
}

void Window::BlurWebView() {
  window_->BlurWebView();
}

bool Window::IsWebViewFocused() {
  return window_->IsWebViewFocused();
}

void Window::CapturePage(mate::Arguments* args) {
  gfx::Rect rect;
  base::Callback<void(v8::Handle<v8::Value>)> callback;

  if (!(args->Length() == 1 && args->GetNext(&callback)) &&
      !(args->Length() == 2 && args->GetNext(&rect)
                            && args->GetNext(&callback))) {
    args->ThrowError();
    return;
  }

  window_->CapturePage(rect, base::Bind(&OnCapturePageDone, callback));
}

void Window::SetRepresentedFilename(const std::string& filename) {
  window_->SetRepresentedFilename(filename);
}

void Window::SetDocumentEdited(bool edited) {
  window_->SetDocumentEdited(edited);
}

mate::Handle<WebContents> Window::GetWebContents(v8::Isolate* isolate) const {
  return WebContents::Create(isolate, window_->GetWebContents());
}

mate::Handle<WebContents> Window::GetDevToolsWebContents(
    v8::Isolate* isolate) const {
  return WebContents::Create(isolate, window_->GetDevToolsWebContents());
}

// static
void Window::BuildPrototype(v8::Isolate* isolate,
                            v8::Handle<v8::ObjectTemplate> prototype) {
  mate::ObjectTemplateBuilder(isolate, prototype)
      .SetMethod("destroy", &Window::Destroy)
      .SetMethod("close", &Window::Close)
      .SetMethod("focus", &Window::Focus)
      .SetMethod("isFocused", &Window::IsFocused)
      .SetMethod("show", &Window::Show)
      .SetMethod("hide", &Window::Hide)
      .SetMethod("isVisible", &Window::IsVisible)
      .SetMethod("maximize", &Window::Maximize)
      .SetMethod("unmaximize", &Window::Unmaximize)
      .SetMethod("minimize", &Window::Minimize)
      .SetMethod("restore", &Window::Restore)
      .SetMethod("setFullScreen", &Window::SetFullscreen)
      .SetMethod("isFullScreen", &Window::IsFullscreen)
      .SetMethod("getSize", &Window::GetSize)
      .SetMethod("setSize", &Window::SetSize)
      .SetMethod("getContentSize", &Window::GetContentSize)
      .SetMethod("setContentSize", &Window::SetContentSize)
      .SetMethod("setMinimumSize", &Window::SetMinimumSize)
      .SetMethod("getMinimumSize", &Window::GetMinimumSize)
      .SetMethod("setMaximumSize", &Window::SetMaximumSize)
      .SetMethod("getMaximumSize", &Window::GetMaximumSize)
      .SetMethod("setResizable", &Window::SetResizable)
      .SetMethod("isResizable", &Window::IsResizable)
      .SetMethod("setAlwaysOnTop", &Window::SetAlwaysOnTop)
      .SetMethod("isAlwaysOnTop", &Window::IsAlwaysOnTop)
      .SetMethod("center", &Window::Center)
      .SetMethod("setPosition", &Window::SetPosition)
      .SetMethod("getPosition", &Window::GetPosition)
      .SetMethod("setTitle", &Window::SetTitle)
      .SetMethod("getTitle", &Window::GetTitle)
      .SetMethod("flashFrame", &Window::FlashFrame)
      .SetMethod("setSkipTaskbar", &Window::SetSkipTaskbar)
      .SetMethod("setKiosk", &Window::SetKiosk)
      .SetMethod("isKiosk", &Window::IsKiosk)
      .SetMethod("setRepresentedFilename", &Window::SetRepresentedFilename)
      .SetMethod("setDocumentEdited", &Window::SetDocumentEdited)
      .SetMethod("_openDevTools", &Window::OpenDevTools)
      .SetMethod("closeDevTools", &Window::CloseDevTools)
      .SetMethod("isDevToolsOpened", &Window::IsDevToolsOpened)
      .SetMethod("inspectElement", &Window::InspectElement)
      .SetMethod("focusOnWebView", &Window::FocusOnWebView)
      .SetMethod("blurWebView", &Window::BlurWebView)
      .SetMethod("isWebViewFocused", &Window::IsWebViewFocused)
      .SetMethod("capturePage", &Window::CapturePage)
      .SetMethod("_getWebContents", &Window::GetWebContents)
      .SetMethod("_getDevToolsWebContents", &Window::GetDevToolsWebContents);
}

}  // namespace api

}  // namespace atom


namespace {

void Initialize(v8::Handle<v8::Object> exports) {
  using atom::api::Window;
  v8::Local<v8::Function> constructor = mate::CreateConstructor<Window>(
      node_isolate, "BrowserWindow", base::Bind(&Window::New));
  mate::Dictionary dict(v8::Isolate::GetCurrent(), exports);
  dict.Set("BrowserWindow", static_cast<v8::Handle<v8::Value>>(constructor));
}

}  // namespace

NODE_MODULE(atom_browser_window, Initialize)
