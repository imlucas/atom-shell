# app

The `app` module is responsible for controlling the application's life time.

The example of quitting the whole application when the last window is closed:

```javascript
var app = require('app');
app.on('window-all-closed', function() {
  app.quit();
});
```

## Event: will-finish-launching

Emitted when application has done basic startup. On Windows and Linux it is the
same with `ready` event, on OS X this event represents the
`applicationWillFinishLaunching` message of `NSApplication`, usually you would
setup listeners to `open-file` and `open-url` events here, and start the crash
reporter and auto updater.

Under most cases you should just do everything in `ready` event.

## Event: ready

Emitted when atom-shell has done everything initialization.

## Event: window-all-closed

Emitted when all windows have been closed.

This event is only emitted when the application is not going to quit. If a
user pressed `Cmd + Q`, or the developer called `app.quit()`, atom-shell would
first try to close all windows and then emit the `will-quit` event, and in
this case the `window-all-closed` would not be emitted.

## Event: will-quit

* `event` Event

Emitted when all windows have been closed and the application will quit.
Calling `event.preventDefault()` will prevent the default behaviour, which is
terminating the application.

See description of `window-all-closed` for the differences between `will-quit`
and it.

## Event: open-file

* `event` Event
* `path` String

Emitted when user wants to open a file with the application, it usually
happens when the application is already opened and then OS wants to reuse the
application to open file.

You should call `event.preventDefault()` if you want to handle this event.

## Event: open-url

* `event` Event
* `url` String

Emitted when user wants to open a URL with the application, this URL scheme
must be registered to be opened by your application.

You should call `event.preventDefault()` if you want to handle this event.

## Event: activate-with-no-open-windows

Emitted when the application is activated while there is no opened windows. It
usually happens when user has closed all of application's windows and then
click on the application's dock icon.

## app.quit()

Try to close all windows. If all windows are successfully closed, the
`will-quit` event will be emitted and by default the application would be
terminated.

This method guarantees all `beforeunload` and `unload` handlers are correctly
executed. It is possible that a window cancels the quitting by returning
`false` in `beforeunload` handler.

## app.terminate()

Quit the application directly, it will not try to close all windows so cleanup
code will not run.

## app.getVersion()

Returns the version of loaded application, if no version is found in
application's `package.json`, the version of current bundle or executable would
be returned.

## app.getName()

Returns current application's name, the name in `package.json` would be
used.

Usually the `name` field of `package.json` is a short lowercased name, according
to the spec of npm modules. So usually you should also specify a `productName`
field, which is your application's full capitalized name, and it will be
preferred over `name` by atom-shell.

## app.commandLine.appendSwitch(switch, [value])

Append a switch [with optional value] to Chromium's command line.

**Note:** This will not affect `process.argv`, and is mainly used by developers
to control some low-level Chromium behaviors.

## app.commandLine.appendArgument(value)

Append an argument to Chromium's command line. The argument will quoted properly.

**Note:** This will not affect `process.argv`.

## app.dock.bounce([type])

* `type` String - Can be `critical` or `informational`, the default is
* `informational`

When `critical` is passed, the dock icon will bounce until either the
application becomes active or the request is canceled.

When `informational` is passed, the dock icon will bounce for one second. The
request, though, remains active until either the application becomes active or
the request is canceled.

An ID representing the request would be returned.

**Note:** This API is only available on Mac.

## app.dock.cancelBounce(id)

* `id` Integer

Cancel the bounce of `id`.

**Note:** This API is only available on Mac.

## app.dock.setBadge(text)

* `text` String

Sets the string to be displayed in the dock’s badging area.

**Note:** This API is only available on Mac.

## app.dock.getBadge()

Returns the badge string of the dock.

**Note:** This API is only available on Mac.

## app.dock.hide()

Hides the dock icon.

**Note:** This API is only available on Mac.

## app.dock.show()

Shows the dock icon.

**Note:** This API is only available on Mac.
