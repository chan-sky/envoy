load(
    "//bazel:envoy_build_system.bzl",
    "envoy_benchmark_test",
    "envoy_cc_benchmark_binary",
    "envoy_cc_fuzz_test",
    "envoy_cc_test",
    "envoy_package",
    "envoy_select_boringssl",
)

licenses(["notice"])  # Apache 2

envoy_package()

envoy_cc_test(
    name = "backoff_strategy_test",
    srcs = ["backoff_strategy_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:backoff_lib",
        "//test/mocks/runtime:runtime_mocks",
    ],
)

envoy_cc_test(
    name = "assert_test",
    srcs = ["assert_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:assert_lib",
        "//test/test_common:logging_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "cancel_wrapper_test",
    srcs = ["cancel_wrapper_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:cancel_wrapper_lib",
    ],
)

envoy_cc_test(
    name = "containers_test",
    srcs = ["containers_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:containers_lib",
        "//test/test_common:logging_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    # Same as assert_test above, but with ENVOY_LOG_DEBUG_ASSERT_IN_RELEASE
    name = "assert_enabled_in_release_test",
    srcs = ["assert_test.cc"],
    copts = ["-DENVOY_LOG_DEBUG_ASSERT_IN_RELEASE"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:assert_lib",
        "//test/test_common:logging_lib",
        "//test/test_common:utility_lib",
        "@com_google_absl//absl/base",
    ],
)

envoy_cc_test(
    name = "assert2_test",
    srcs = ["assert2_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:assert_lib",
        "//test/test_common:logging_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "base64_test",
    srcs = ["base64_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/buffer:buffer_lib",
        "//source/common/common:base64_lib",
    ],
)

envoy_cc_fuzz_test(
    name = "base64_fuzz_test",
    srcs = ["base64_fuzz_test.cc"],
    corpus = "base64_corpus",
    rbe_pool = "6gig",
    # Fuzzer is stable, no bugs, simple test target; avoid emitting CO2.
    tags = ["no_fuzz"],
    deps = ["//source/common/common:base64_lib"],
)

envoy_cc_fuzz_test(
    name = "utility_fuzz_test",
    srcs = ["utility_fuzz_test.cc"],
    corpus = "utility_corpus",
    rbe_pool = "6gig",
    # Fuzzer is stable, no bugs, simple test target; avoid emitting CO2.
    tags = ["no_fuzz"],
    deps = ["//source/common/common:utility_lib"],
)

envoy_cc_fuzz_test(
    name = "hash_fuzz_test",
    srcs = ["hash_fuzz_test.cc"],
    corpus = "hash_corpus",
    rbe_pool = "6gig",
    deps = ["//source/common/common:hash_lib"],
)

envoy_cc_test(
    name = "cleanup_test",
    srcs = ["cleanup_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:cleanup_lib"],
)

envoy_cc_test(
    name = "dns_utils_test",
    srcs = ["dns_utils_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:dns_utils_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "mem_block_builder_test",
    srcs = ["mem_block_builder_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:mem_block_builder_lib"],
)

envoy_cc_test(
    name = "safe_memcpy_test",
    srcs = ["safe_memcpy_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:safe_memcpy_lib"],
)

envoy_cc_test(
    name = "phantom_test",
    srcs = ["phantom_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:phantom"],
)

envoy_cc_test(
    name = "fmt_test",
    srcs = ["fmt_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:fmt_lib",
        "//source/common/common:minimal_logger_lib",
    ],
)

envoy_cc_test(
    name = "hash_test",
    srcs = ["hash_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:hash_lib"],
)

envoy_cc_test(
    name = "hex_test",
    srcs = ["hex_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:hex_lib"],
)

envoy_cc_test(
    name = "linked_object_test",
    srcs = ["linked_object_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:linked_object",
    ],
)

envoy_cc_test(
    name = "log_macros_test",
    srcs = ["log_macros_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:minimal_logger_lib",
        "//test/mocks/http:http_mocks",
        "//test/mocks/network:network_mocks",
        "//test/mocks/upstream:upstream_mocks",
        "//test/test_common:logging_lib",
    ],
)

envoy_cc_test(
    name = "log_verbosity_update_test",
    srcs = ["log_verbosity_update_test.cc"],
    args = ["--enable-fine-grain-logging"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:minimal_logger_lib",
        "//test/test_common:logging_lib",
    ],
)

envoy_cc_test(
    name = "fine_grain_log_macros_test",
    srcs = ["log_macros_test.cc"],
    args = ["--enable-fine-grain-logging"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:minimal_logger_lib",
        "//test/mocks/http:http_mocks",
        "//test/mocks/network:network_mocks",
        "//test/mocks/upstream:upstream_mocks",
        "//test/test_common:logging_lib",
    ],
)

envoy_cc_benchmark_binary(
    name = "logger_speed_test",
    srcs = ["logger_speed_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:minimal_logger_lib",
        "@com_github_google_benchmark//:benchmark",
    ],
)

envoy_benchmark_test(
    name = "logger_speed_test_benchmark_test",
    benchmark_binary = "logger_speed_test",
)

envoy_cc_test(
    name = "logger_test",
    srcs = ["logger_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:minimal_logger_lib",
        "//test/mocks/http:http_mocks",
        "//test/mocks/network:network_mocks",
    ],
)

envoy_cc_fuzz_test(
    name = "logger_fuzz_test",
    srcs = ["logger_fuzz_test.cc"],
    corpus = "logger_corpus",
    rbe_pool = "6gig",
    # TODO(github.com/envoyproxy/envoy#8893): Re-enable once more fuzz tests are added
    tags = ["no_fuzz"],
    deps = ["//source/common/common:minimal_logger_lib"],
)

envoy_cc_test(
    name = "matchers_test",
    srcs = ["matchers_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:matchers_lib",
        "//source/common/config:metadata_lib",
        "//source/common/protobuf:utility_lib",
        "//source/common/stream_info:filter_state_lib",
        "//test/mocks/server:server_factory_context_mocks",
        "//test/test_common:utility_lib",
        "@envoy_api//envoy/config/core/v3:pkg_cc_proto",
        "@envoy_api//envoy/type/matcher/v3:pkg_cc_proto",
    ],
)

envoy_cc_test(
    name = "mutex_tracer_test",
    srcs = ["mutex_tracer_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:mutex_tracer_lib",
        "//test/test_common:contention_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "optref_test",
    srcs = ["optref_test.cc"],
    rbe_pool = "6gig",
)

envoy_cc_test(
    name = "random_generator_test",
    srcs = ["random_generator_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:interval_value",
        "//source/common/common:random_generator_lib",
        "//test/mocks/runtime:runtime_mocks",
        "//test/test_common:environment_lib",
    ],
)

envoy_cc_test(
    name = "trie_lookup_table_test",
    srcs = ["trie_lookup_table_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:trie_lookup_table_lib"],
)

envoy_cc_test(
    name = "radix_tree_test",
    srcs = ["radix_tree_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:radix_tree_lib"],
)

envoy_cc_test(
    name = "utility_test",
    srcs = ["utility_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:utility_lib",
        "//test/common/memory:memory_test_utility_lib",
        "//test/test_common:simulated_time_system_lib",
        "//test/test_common:test_time_lib",
        "//test/test_common:utility_lib",
        "@com_google_absl//absl/strings",
    ],
)

envoy_cc_test(
    name = "compiled_string_map_test",
    srcs = ["compiled_string_map_test.cc"],
    rbe_pool = "6gig",
)

envoy_cc_test(
    name = "packed_struct_test",
    srcs = ["packed_struct_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:packed_struct_lib",
        "//test/common/memory:memory_test_utility_lib",
        "//test/test_common:utility_lib",
        "@envoy_api//envoy/config/route/v3:pkg_cc_proto",
    ],
)

envoy_cc_test(
    name = "regex_test",
    srcs = ["regex_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:regex_lib",
        "//test/test_common:logging_lib",
        "//test/test_common:test_runtime_lib",
        "//test/test_common:utility_lib",
        "@envoy_api//envoy/type/matcher/v3:pkg_cc_proto",
    ],
)

envoy_cc_test(
    name = "perf_annotation_test",
    srcs = ["perf_annotation_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:perf_annotation_lib",
    ],
)

envoy_cc_test(
    name = "perf_annotation_disabled_test",
    srcs = ["perf_annotation_disabled_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:perf_annotation_lib",
    ],
)

envoy_cc_test(
    name = "basic_resource_impl_test",
    srcs = ["basic_resource_impl_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:basic_resource_lib",
        "//test/mocks/runtime:runtime_mocks",
    ],
)

envoy_cc_test(
    name = "token_bucket_impl_test",
    srcs = ["token_bucket_impl_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:token_bucket_impl_lib",
        "//test/test_common:simulated_time_system_lib",
        "//test/test_common:test_time_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "shared_token_bucket_impl_test",
    srcs = ["shared_token_bucket_impl_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:shared_token_bucket_impl_lib",
        "//test/test_common:simulated_time_system_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "callback_impl_test",
    srcs = ["callback_impl_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:callback_impl_lib",
        "//test/mocks/event:event_mocks",
    ],
)

envoy_cc_benchmark_binary(
    name = "re_speed_test",
    srcs = ["re_speed_test.cc"],
    deps = [
        "//source/common/common:assert_lib",
        "//source/common/common:utility_lib",
        "@com_github_google_benchmark//:benchmark",
        "@com_googlesource_code_re2//:re2",
    ],
)

envoy_cc_benchmark_binary(
    name = "utility_speed_test",
    srcs = ["utility_speed_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:assert_lib",
        "//source/common/common:utility_lib",
        "@com_github_google_benchmark//:benchmark",
        "@com_google_absl//absl/strings",
    ],
)

envoy_benchmark_test(
    name = "utility_speed_test_benchmark_test",
    benchmark_binary = "utility_speed_test",
)

envoy_cc_benchmark_binary(
    name = "trie_lookup_table_speed_test",
    srcs = ["trie_lookup_table_speed_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:trie_lookup_table_lib",
        "@com_github_google_benchmark//:benchmark",
        "@com_google_absl//absl/strings",
    ],
)

envoy_benchmark_test(
    name = "trie_lookup_table_speed_test_benchmark_test",
    benchmark_binary = "trie_lookup_table_speed_test",
)

envoy_cc_benchmark_binary(
    name = "radix_tree_speed_test",
    srcs = ["radix_tree_speed_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:radix_tree_lib",
        "@com_github_google_benchmark//:benchmark",
        "@com_google_absl//absl/strings",
    ],
)

envoy_benchmark_test(
    name = "radix_tree_speed_test_benchmark_test",
    benchmark_binary = "radix_tree_speed_test",
)

envoy_cc_benchmark_binary(
    name = "prefix_matching_benchmark",
    srcs = ["prefix_matching_benchmark.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:radix_tree_lib",
        "//source/common/common:trie_lookup_table_lib",
        "//source/common/http:headers_lib",
        "@com_github_google_benchmark//:benchmark",
        "@com_google_absl//absl/strings",
    ],
)

envoy_benchmark_test(
    name = "prefix_matching_benchmark_test",
    benchmark_binary = "prefix_matching_benchmark",
)

envoy_cc_test(
    name = "lock_guard_test",
    srcs = ["lock_guard_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:lock_guard_lib",
        "//source/common/common:thread_lib",
    ],
)

envoy_cc_test(
    name = "thread_id_test",
    srcs = ["thread_id_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:thread_lib",
        "//test/test_common:thread_factory_for_test_lib",
        "@com_google_absl//absl/hash:hash_testing",
    ],
)

envoy_cc_test(
    name = "thread_test",
    srcs = ["thread_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:thread_lib",
        "//source/common/common:thread_synchronizer_lib",
        "//test/test_common:thread_factory_for_test_lib",
    ],
)

envoy_cc_test(
    name = "stl_helpers_test",
    srcs = ["stl_helpers_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:stl_helpers",
    ],
)

envoy_cc_test(
    name = "version_test",
    srcs = ["version_test.cc"],
    copts = envoy_select_boringssl(["-DENVOY_SSL_FIPS"]),
    rbe_pool = "6gig",
    deps = [
        "//source/common/version:version_lib",
        "@com_google_absl//absl/strings",
    ],
)

envoy_cc_test(
    name = "api_version_test",
    srcs = ["api_version_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/version:api_version_lib"],
)

envoy_cc_test(
    name = "statusor_test",
    srcs = ["statusor_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:statusor_lib",
        "//source/common/http:status_lib",
    ],
)

envoy_cc_test(
    name = "interval_value_test",
    srcs = ["interval_value_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:interval_value"],
)

envoy_cc_test(
    name = "scope_tracker_test",
    srcs = ["scope_tracker_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/api:api_lib",
        "//source/common/common:scope_tracker",
        "//source/common/event:dispatcher_lib",
        "//test/mocks:common_lib",
        "//test/test_common:utility_lib",
    ],
)

envoy_cc_test(
    name = "scope_tracked_object_stack_test",
    srcs = ["scope_tracked_object_stack_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:scope_tracked_object_stack",
        "//source/common/common:utility_lib",
        "//test/mocks/stream_info:stream_info_mocks",
    ],
)

envoy_cc_test(
    name = "union_string_test",
    srcs = ["union_string_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//envoy/common:union_string",
    ],
)

envoy_cc_test(
    name = "bit_array_test",
    srcs = ["bit_array_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:bit_array_lib",
        "//source/common/common:random_generator_lib",
    ],
)

envoy_cc_test(
    name = "inline_map_test",
    srcs = ["inline_map_test.cc"],
    rbe_pool = "6gig",
    deps = ["//source/common/common:inline_map"],
)

envoy_cc_benchmark_binary(
    name = "inline_map_speed_test",
    srcs = ["inline_map_speed_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:inline_map",
        "@com_github_google_benchmark//:benchmark",
    ],
)

envoy_benchmark_test(
    name = "inline_map_speed_test_benchmark_test",
    benchmark_binary = "inline_map_speed_test",
)

envoy_cc_test(
    name = "execution_context_test",
    srcs = ["execution_context_test.cc"],
    copts = [
        "-DENVOY_ENABLE_EXECUTION_CONTEXT",
    ],
    rbe_pool = "6gig",
    deps = [
        "//envoy/common:execution_context",
        "//source/common/api:api_lib",
        "//source/common/http:conn_manager_lib",
        "//source/common/http:filter_manager_lib",
        "//test/mocks:common_lib",
        "//test/mocks/stream_info:stream_info_mocks",
    ],
)

envoy_cc_test(
    name = "notification_test",
    srcs = ["notification_test.cc"],
    rbe_pool = "6gig",
    deps = [
        "//source/common/common:notification_lib",
        "//test/test_common:logging_lib",
    ],
)
