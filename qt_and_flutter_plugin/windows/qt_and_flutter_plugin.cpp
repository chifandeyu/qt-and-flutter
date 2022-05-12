#include "include/qt_and_flutter_plugin/qt_and_flutter_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include "qmfcapp.h"
#include <map>
#include <memory>
#include <sstream>

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpvReserved*/)
{
    static bool ownApplication = FALSE;

    if (dwReason == DLL_PROCESS_ATTACH)
        ownApplication = QMfcApp::pluginInstance(hInstance);
    if (dwReason == DLL_PROCESS_DETACH && ownApplication)
        delete qApp;

    return TRUE;
}

namespace {

class QtAndFlutterPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  QtAndFlutterPlugin();

  virtual ~QtAndFlutterPlugin();

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void QtAndFlutterPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "qt_and_flutter_plugin",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<QtAndFlutterPlugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

QtAndFlutterPlugin::QtAndFlutterPlugin() {}

QtAndFlutterPlugin::~QtAndFlutterPlugin() {}

void QtAndFlutterPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else if(method_call.method_name().compare("createWidget") == 0) {
      QWidget* widget = new QWidget;
      QLabel* lable = new QLabel( "Im a Qt label.", widget);
      QGridLayout* layout = new QGridLayout();
      lable->setAlignment(Qt::AlignCenter);
      layout->addWidget(lable);
      widget->setLayout(layout);
      widget->resize(400, 300);
      widget->show();
      widget->activateWindow();

      result->Success(flutter::EncodableValue(true));
  }
  else {
    result->NotImplemented();
  }
}

}  // namespace

void QtAndFlutterPluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  QtAndFlutterPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
