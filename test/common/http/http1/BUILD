load(
    "//bazel:envoy_build_system.bzl",
    "envoy_cc_fuzz_test",
    "envoy_cc_test",
    "envoy_package",
)

licenses(["notice"])  # Apache 2

envoy_package()

envoy_cc_test(
    name = "header_formatter_test",
    srcs = ["header_formatter_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/http/http1:header_formatter_lib",
    ],
)

envoy_cc_test(
    name = "codec_impl_test",
    srcs = ["codec_impl_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//envoy/buffer:buffer_interface",
        "//envoy/event:dispatcher_interface",
        "//source/common/buffer:buffer_lib",
        "//source/common/event:dispatcher_lib",
        "//source/common/http:exception_lib",
        "//source/common/http:header_map_lib",
        "//source/common/http/http1:codec_lib",
        "//source/extensions/http/header_validators/envoy_default:http1_header_validator",
        "//test/common/memory:memory_test_utility_lib",
        "//test/common/stats:stat_test_utility_lib",
        "//test/mocks/buffer:buffer_mocks",
        "//test/mocks/http:http_mocks",
        "//test/mocks/init:init_mocks",
        "//test/mocks/local_info:local_info_mocks",
        "//test/mocks/network:network_mocks",
        "//test/mocks/protobuf:protobuf_mocks",
        "//test/mocks/server:factory_context_mocks",
        "//test/mocks/server:overload_manager_mocks",
        "//test/mocks/stream_info:stream_info_mocks",
        "//test/mocks/thread_local:thread_local_mocks",
        "//test/mocks/upstream:upstream_mocks",
        "//test/test_common:logging_lib",
        "//test/test_common:test_runtime_lib",
        "@envoy_api//envoy/extensions/http/header_validators/envoy_default/v3:pkg_cc_proto",
    ],
)

envoy_cc_test(
    name = "conn_pool_test",
    srcs = ["conn_pool_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/buffer:buffer_lib",
        "//source/common/event:dispatcher_lib",
        "//source/common/http:codec_client_lib",
        "//source/common/http/http1:conn_pool_lib",
        "//source/common/network:utility_lib",
        "//source/common/upstream:upstream_includes",
        "//source/common/upstream:upstream_lib",
        "//test/common/http:common_lib",
        "//test/common/upstream:utility_lib",
        "//test/mocks/buffer:buffer_mocks",
        "//test/mocks/event:event_mocks",
        "//test/mocks/http:http_mocks",
        "//test/mocks/network:network_mocks",
        "//test/mocks/runtime:runtime_mocks",
        "//test/mocks/server:overload_manager_mocks",
        "//test/mocks/upstream:cluster_info_mocks",
        "//test/mocks/upstream:transport_socket_match_mocks",
        "//test/test_common:simulated_time_system_lib",
        "//test/test_common:test_runtime_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "http1_connection_fuzz_test",
    srcs = ["http1_connection_fuzz_test.cc"],
    corpus = "http1_connection_corpus",
    rbe_pool = "6gig",
    deps = [
        "//source/common/http:conn_manager_lib",
        "//source/common/http:header_map_lib",
        "//source/common/http/http1:codec_lib",
        "//test/fuzz:utility_lib",
        "//test/mocks/http:http_mocks",
        "//test/mocks/network:network_mocks",
        "//test/mocks/server:overload_manager_mocks",
        "//test/test_common:test_runtime_lib",
    ],
)
