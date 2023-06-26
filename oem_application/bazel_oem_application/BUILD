load("@rules_python//python:defs.bzl", "py_binary")

py_binary(
  name = "main",

  srcs = ["main.py",
          "function_implementation.py", 
          "combine_bins.py", 
          "config_parser.py", 
          "firebase_connection.py",
    ],

    data = [
      "serviceAccountKey.json", 
      "ota_config",
    ],
)
