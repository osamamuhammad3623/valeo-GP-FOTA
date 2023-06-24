load("@rules_python//python:defs.bzl", "py_binary")

py_binary(
  name = "main",

  srcs = ["main.py",
          "function_implementation.py", 
          "combine_bins.py", 
          "package_config.py", 
          "firebase_connection.py",
    ],

    data = ["serviceAccountKey.json"],
)
