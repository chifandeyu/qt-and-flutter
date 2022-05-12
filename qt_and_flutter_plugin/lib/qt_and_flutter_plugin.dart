
import 'dart:async';

import 'package:flutter/services.dart';

class QtAndFlutterPlugin {
  static const MethodChannel _channel = MethodChannel('qt_and_flutter_plugin');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
