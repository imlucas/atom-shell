// Copyright (c) 2013 GitHub, Inc. All rights reserved.
// Use of this source code is governed by the MIT license that can be
// found in the LICENSE file.

#ifndef ATOM_BROWSER_NATIVE_WINDOW_WIN_H_
#define ATOM_BROWSER_NATIVE_WINDOW_WIN_H_

#include <map>
#include <string>
#include <vector>

#include "base/memory/scoped_ptr.h"
#include "base/strings/string16.h"
#include "atom/browser/native_window.h"
#include "atom/browser/ui/accelerator_util.h"
#include "ui/gfx/size.h"
#include "ui/views/focus/widget_focus_manager.h"
#include "ui/views/widget/widget_delegate.h"
#include "vendor/brightray/browser/win/inspectable_web_contents_view_win.h"

namespace ui {
class MenuModel;
}

namespace views {
class Widget;
}

namespace atom {

class Menu2;

class NativeWindowWin : public NativeWindow,
                        public views::WidgetFocusChangeListener,
                        public views::WidgetDelegateView {
 public:
  explicit NativeWindowWin(content::WebContents* web_contents,
                           const mate::Dictionary& options);
  virtual ~NativeWindowWin();

  // NativeWindow implementation.
  virtual void Close() OVERRIDE;
  virtual void CloseImmediately() OVERRIDE;
  virtual void Move(const gfx::Rect& pos) OVERRIDE;
  virtual void Focus(bool focus) OVERRIDE;
  virtual bool IsFocused() OVERRIDE;
  virtual void Show() OVERRIDE;
  virtual void Hide() OVERRIDE;
  virtual bool IsVisible() OVERRIDE;
  virtual void Maximize() OVERRIDE;
  virtual void Unmaximize() OVERRIDE;
  virtual void Minimize() OVERRIDE;
  virtual void Restore() OVERRIDE;
  virtual void SetFullscreen(bool fullscreen) OVERRIDE;
  virtual bool IsFullscreen() OVERRIDE;
  virtual void SetSize(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetSize() OVERRIDE;
  virtual void SetContentSize(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetContentSize() OVERRIDE;
  virtual void SetMinimumSize(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetMinimumSize() OVERRIDE;
  virtual void SetMaximumSize(const gfx::Size& size) OVERRIDE;
  virtual gfx::Size GetMaximumSize() OVERRIDE;
  virtual void SetResizable(bool resizable) OVERRIDE;
  virtual bool IsResizable() OVERRIDE;
  virtual void SetAlwaysOnTop(bool top) OVERRIDE;
  virtual bool IsAlwaysOnTop() OVERRIDE;
  virtual void Center() OVERRIDE;
  virtual void SetPosition(const gfx::Point& position) OVERRIDE;
  virtual gfx::Point GetPosition() OVERRIDE;
  virtual void SetTitle(const std::string& title) OVERRIDE;
  virtual std::string GetTitle() OVERRIDE;
  virtual void FlashFrame(bool flash) OVERRIDE;
  virtual void SetSkipTaskbar(bool skip) OVERRIDE;
  virtual void SetKiosk(bool kiosk) OVERRIDE;
  virtual bool IsKiosk() OVERRIDE;
  virtual gfx::NativeWindow GetNativeWindow() OVERRIDE;

  void OnMenuCommand(int position, HMENU menu);

  // Set the native window menu.
  void SetMenu(ui::MenuModel* menu_model);

  brightray::InspectableWebContentsViewWin* inspectable_web_contents_view()
      const {
    return static_cast<brightray::InspectableWebContentsViewWin*>(
        inspectable_web_contents()->GetView());
  }

  views::Widget* window() const { return window_.get(); }
  atom::Menu2* menu() const { return menu_.get(); }
  SkRegion* draggable_region() { return draggable_region_.get(); }

 protected:
  virtual void UpdateDraggableRegions(
      const std::vector<DraggableRegion>& regions) OVERRIDE;

  // Overridden from content::WebContentsDelegate:
  virtual void HandleKeyboardEvent(
      content::WebContents*,
      const content::NativeWebKeyboardEvent&) OVERRIDE;

  // Overridden from views::View:
  virtual void Layout() OVERRIDE;
  virtual void ViewHierarchyChanged(
      const ViewHierarchyChangedDetails& details) OVERRIDE;
  virtual bool AcceleratorPressed(const ui::Accelerator& accelerator) OVERRIDE;

  // Overridden from views::WidgetDelegate:
  virtual void DeleteDelegate() OVERRIDE;
  virtual views::View* GetInitiallyFocusedView() OVERRIDE;
  virtual bool CanResize() const OVERRIDE;
  virtual bool CanMaximize() const OVERRIDE;
  virtual string16 GetWindowTitle() const OVERRIDE;
  virtual bool ShouldHandleSystemCommands() const OVERRIDE;
  virtual gfx::ImageSkia GetWindowAppIcon() OVERRIDE;
  virtual gfx::ImageSkia GetWindowIcon() OVERRIDE;
  virtual views::Widget* GetWidget() OVERRIDE;
  virtual const views::Widget* GetWidget() const OVERRIDE;
  virtual views::ClientView* CreateClientView(views::Widget* widget) OVERRIDE;
  virtual views::NonClientFrameView* CreateNonClientFrameView(
      views::Widget* widget) OVERRIDE;

  // Overridden from views::WidgetFocusChangeListener:
  virtual void OnNativeFocusChange(gfx::NativeView focused_before,
                                   gfx::NativeView focused_now) OVERRIDE;

 private:
  typedef struct { int position; ui::MenuModel* model; } MenuItem;
  typedef std::map<ui::Accelerator, MenuItem> AcceleratorTable;

  void ClientAreaSizeToWindowSize(gfx::Size* size);

  void OnViewWasResized();

  // Register accelerators supported by the menu model.
  void RegisterAccelerators();

  scoped_ptr<views::Widget> window_;
  views::View* web_view_;  // managed by window_.

  // The window menu.
  scoped_ptr<atom::Menu2> menu_;

  // Map from accelerator to menu item's command id.
  accelerator_util::AcceleratorTable accelerator_table_;

  scoped_ptr<SkRegion> draggable_region_;

  bool use_content_size_;

  bool resizable_;
  string16 title_;
  gfx::Size minimum_size_;
  gfx::Size maximum_size_;

  DISALLOW_COPY_AND_ASSIGN(NativeWindowWin);
};

}  // namespace atom

#endif  // ATOM_BROWSER_NATIVE_WINDOW_WIN_H_
