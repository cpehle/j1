cc_test(
    name = "j1_test",
    srcs = ["J1Test.cpp"],
    copts = ["-std=c++14"],
    deps = [
    	":dut_j1",
        ":jit",
        "@gtest//:main",
    ],
)

cc_test(
    name = "binary_jit_test",
    srcs = ["BinaryJitTTest.cpp"],
    deps = [
        ":jit",
        "@gtest//:main",
    ],
)

cc_library(
    name = "jit",
    hdrs = [
        "BinaryJitT.hpp",
        "JitBuilder.hpp",
    ],
    visibility = ["//visibility:public"],
)

cc_library(
    name = "dut_j1",
    srcs = [
        "J1.cpp",
        "obj_dir/Vj1__ALL.a",
    ],
    hdrs = [
        "J1.hpp",
        "obj_dir/Vj1.h",
    ],
    includes = [
        "obj_dir/",
    ],
    deps = [
        "@verilator//:verilated",
        "@verilator//:verilated_vcd_c",
    ],
)
