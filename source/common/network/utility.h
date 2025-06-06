#pragma once

#include <cstdint>
#include <list>
#include <string>

#include "envoy/common/platform.h"
#include "envoy/config/core/v3/address.pb.h"
#include "envoy/network/connection.h"
#include "envoy/network/listener.h"

#include "source/common/api/os_sys_calls_impl.h"
#include "source/common/common/cleanup.h"
#include "source/common/common/statusor.h"

#if defined(__linux__)
#include "source/common/api/os_sys_calls_impl_linux.h"
#endif

#include "absl/strings/string_view.h"

namespace Envoy {
namespace Network {

/**
 * A callback interface used by readFromSocket() to pass packets read from
 * socket.
 */
class UdpPacketProcessor {
public:
  virtual ~UdpPacketProcessor() = default;

  /**
   * Consume the packet read out of the socket with the information from UDP
   * header.
   * @param local_address is the destination address in the UDP header.
   * @param peer_address is the source address in the UDP header.
   * @param buffer contains the packet read.
   * @param receive_time is the time when the packet is read.
   */
  virtual void processPacket(Address::InstanceConstSharedPtr local_address,
                             Address::InstanceConstSharedPtr peer_address,
                             Buffer::InstancePtr buffer, MonotonicTime receive_time, uint8_t tos,
                             Buffer::OwnedImpl saved_cmsg) PURE;

  /**
   * Called whenever datagrams are dropped due to overflow or truncation.
   * @param dropped supplies the number of dropped datagrams.
   */
  virtual void onDatagramsDropped(uint32_t dropped) PURE;

  /**
   * The expected max size of the datagram to be read. If it's smaller than
   * the size of datagrams received, they will be dropped.
   */
  virtual uint64_t maxDatagramSize() const PURE;

  /**
   * An estimated number of packets to read in each read event.
   */
  virtual size_t numPacketsExpectedPerEventLoop() const PURE;

  virtual const IoHandle::UdpSaveCmsgConfig& saveCmsgConfig() const PURE;
};

static const uint64_t DEFAULT_UDP_MAX_DATAGRAM_SIZE = 1500;
static const uint64_t NUM_DATAGRAMS_PER_RECEIVE = 16;
static const uint64_t MAX_NUM_PACKETS_PER_EVENT_LOOP = 6000;

/**
 * Wrapper which resolves UDP socket proto config with defaults.
 */
struct ResolvedUdpSocketConfig {
  ResolvedUdpSocketConfig(const envoy::config::core::v3::UdpSocketConfig& config,
                          bool prefer_gro_default);

  uint64_t max_rx_datagram_size_;
  bool prefer_gro_;
};

// The different options for receiving UDP packet(s) from system calls.
enum class UdpRecvMsgMethod {
  // The `recvmsg` system call.
  RecvMsg,
  // The `recvmsg` system call using GRO (generic receive offload). This is the preferred method,
  // if the platform supports it.
  RecvMsgWithGro,
  // The `recvmmsg` system call.
  RecvMmsg,
};

/**
 * Common network utility routines.
 */
class Utility {
public:
  static constexpr absl::string_view TCP_SCHEME{"tcp://"};
  static constexpr absl::string_view UDP_SCHEME{"udp://"};
  static constexpr absl::string_view UNIX_SCHEME{"unix://"};

  /**
   * Make a URL from a datagram Address::Instance; will be udp:// prefix for
   * an IP address, and unix:// prefix otherwise. Giving a tcp address to this
   * function will result in incorrect behavior (addresses don't know if they
   * are datagram or stream).
   * @param addr supplies the address to convert to string.
   * @return The appropriate url string compatible with resolveUrl.
   */
  static std::string urlFromDatagramAddress(const Address::Instance& addr);

  /**
   * Resolve a URL.
   * @param url supplies the url to resolve.
   * @return Address::InstanceConstSharedPtr the resolved address or an error status
   */
  static absl::StatusOr<Address::InstanceConstSharedPtr> resolveUrl(const std::string& url);

  /**
   * Determine the socket type for a URL.
   *
   * @param url supplies the url to resolve.
   * @return StatusOr<Socket::Type> of the socket type, or an error status if url is invalid.
   */
  static StatusOr<Socket::Type> socketTypeFromUrl(const std::string& url);

  /**
   * Match a URL to the TCP scheme
   * @param url supplies the URL to match.
   * @return bool true if the URL matches the TCP scheme, false otherwise.
   */
  static bool urlIsTcpScheme(absl::string_view url);

  /**
   * Match a URL to the UDP scheme
   * @param url supplies the URL to match.
   * @return bool true if the URL matches the UDP scheme, false otherwise.
   */
  static bool urlIsUdpScheme(absl::string_view url);

  /**
   * Match a URL to the Unix scheme
   * @param url supplies the Unix to match.
   * @return bool true if the URL matches the Unix scheme, false otherwise.
   */
  static bool urlIsUnixScheme(absl::string_view url);

  /**
   * Parse an internet host address (IPv4 or IPv6) and create an Instance from it. The address must
   * not include a port number.
   * @param ip_address string to be parsed as an internet address.
   * @param port optional port to include in Instance created from ip_address, 0 by default.
   * @param v6only disable IPv4-IPv6 mapping for IPv6 addresses?
   * @param network_namespace network namespace containing the address.
   * @return pointer to the Instance, or nullptr if unable to parse the address.
   */
  static Address::InstanceConstSharedPtr
  parseInternetAddressNoThrow(const std::string& ip_address, uint16_t port = 0, bool v6only = true,
                              absl::optional<std::string> network_namespace = absl::nullopt);

  /**
   * Parse an internet host address (IPv4 or IPv6) AND port, and create an Instance from it. Throws
   * EnvoyException if unable to parse the address. This is needed when a shared pointer is needed
   * but only a raw instance is available.
   * @param Address::Ip& to be copied to the new instance.
   * @return pointer to the Instance.
   */
  static Address::InstanceConstSharedPtr copyInternetAddressAndPort(const Address::Ip& ip);

  /**
   * Create a new Instance from an internet host address (IPv4 or IPv6) and port.
   * @param ip_addr string to be parsed as an internet address and port. Examples:
   *        - "1.2.3.4:80"
   *        - "[1234:5678::9]:443"
   * @param v6only disable IPv4-IPv6 mapping for IPv6 addresses?
   * @param network_namespace network namespace containing the address.
   * @return pointer to the Instance, or a nullptr in case of a malformed IP address.
   */
  static Address::InstanceConstSharedPtr
  parseInternetAddressAndPortNoThrow(const std::string& ip_address, bool v6only = true,
                                     absl::optional<std::string> network_namespace = absl::nullopt);

  /**
   * Get the local address of the first interface address that is of type
   * version and is not a loopback address. If no matches are found, return the
   * loopback address of type version.
   * @param the local address IP version.
   * @return the local IP address of the server
   */
  static Address::InstanceConstSharedPtr getLocalAddress(const Address::IpVersion version);

  /**
   * Determine whether this is a local connection.
   * @return bool the address is a local connection.
   */
  static bool isSameIpOrLoopback(const ConnectionInfoProvider& socket);

  /**
   * Determine whether this is an internal (RFC1918) address.
   * @return bool the address is an RFC1918 address.
   */
  static bool isInternalAddress(const Address::Instance& address);

  /**
   * Check if address is loopback address.
   * @param address IP address to check.
   * @return true if so, otherwise false
   */
  static bool isLoopbackAddress(const Address::Instance& address);

  /**
   * @return Address::InstanceConstSharedPtr an address that represents the canonical IPv4 loopback
   *         address (i.e. "127.0.0.1"). Note that the range "127.0.0.0/8" is all defined as the
   *         loopback range, but the address typically used (e.g. in tests) is "127.0.0.1".
   */
  static Address::InstanceConstSharedPtr getCanonicalIpv4LoopbackAddress();

  /**
   * @return Address::InstanceConstSharedPtr an address that represents the IPv6 loopback address
   *         (i.e. "::1").
   */
  static Address::InstanceConstSharedPtr getIpv6LoopbackAddress();

  /**
   * @return Address::InstanceConstSharedPtr an address that represents the IPv4 wildcard address
   *         (i.e. "0.0.0.0"). Used during binding to indicate that incoming connections to any
   *         local IPv4 address are to be accepted.
   */
  static Address::InstanceConstSharedPtr getIpv4AnyAddress();

  /**
   * @return Address::InstanceConstSharedPtr an address that represents the IPv6 wildcard address
   *         (i.e. "::"). Used during binding to indicate that incoming connections to any local
   *         IPv6 address are to be accepted.
   */
  static Address::InstanceConstSharedPtr getIpv6AnyAddress();

  /**
   * @return the IPv4 CIDR catch-all address (0.0.0.0/0).
   */
  static const std::string& getIpv4CidrCatchAllAddress();

  /**
   * @return the IPv6 CIDR catch-all address (::/0).
   */
  static const std::string& getIpv6CidrCatchAllAddress();

  /**
   * @param address IP address instance.
   * @param port to update.
   * @return Address::InstanceConstSharedPtr a new address instance with updated port.
   */
  static Address::InstanceConstSharedPtr getAddressWithPort(const Address::Instance& address,
                                                            uint32_t port);

  /**
   * Retrieve the original destination address from an accepted socket.
   * The address (IP and port) may be not local and the port may differ from
   * the listener port if the packets were redirected using iptables
   * @param sock is accepted socket
   * @return the original destination or nullptr if not available.
   */
  static Address::InstanceConstSharedPtr getOriginalDst(Socket& sock);

  /**
   * Converts IPv6 absl::uint128 in network byte order to host byte order.
   * @param address supplies the IPv6 address in network byte order.
   * @return the absl::uint128 IPv6 address in host byte order.
   */
  static absl::uint128 Ip6ntohl(const absl::uint128& address);

  /**
   * Converts IPv6 absl::uint128 in host byte order to network byte order.
   * @param address supplies the IPv6 address in host byte order.
   * @return the absl::uint128 IPv6 address in network byte order.
   */
  static absl::uint128 Ip6htonl(const absl::uint128& address);

  /**
   * @param proto_address supplies the proto address to convert
   * @return the InstanceConstSharedPtr for the address, or null if proto_address is invalid.
   */
  static Address::InstanceConstSharedPtr
  protobufAddressToAddressNoThrow(const envoy::config::core::v3::Address& proto_address);

  /**
   * Copies the address instance into the protobuf representation of an address.
   * @param address is the address to be copied into the protobuf representation of this address.
   * @param proto_address is the protobuf address to which the address instance is copied into.
   */
  static void addressToProtobufAddress(const Address::Instance& address,
                                       envoy::config::core::v3::Address& proto_address);

  /**
   * Returns socket type corresponding to SocketAddress.protocol value of the
   * given address, or SocketType::Stream if the address is a pipe address.
   * @param proto_address the address protobuf
   * @return socket type
   */
  static Socket::Type
  protobufAddressSocketType(const envoy::config::core::v3::Address& proto_address);

  /**
   * Send a packet via given UDP socket with specific source address.
   * @param handle is the UDP socket used to send.
   * @param slices points to the buffers containing the packet.
   * @param num_slices is the number of buffers.
   * @param local_ip is the source address to be used to send.
   * @param peer_address is the destination address to send to.
   */
  static Api::IoCallUint64Result writeToSocket(IoHandle& handle, Buffer::RawSlice* slices,
                                               uint64_t num_slices, const Address::Ip* local_ip,
                                               const Address::Instance& peer_address);
  static Api::IoCallUint64Result writeToSocket(IoHandle& handle, const Buffer::Instance& buffer,
                                               const Address::Ip* local_ip,
                                               const Address::Instance& peer_address);

  /**
   * Read a packet from a given UDP socket and pass the packet to given UdpPacketProcessor.
   * @param handle is the UDP socket to read from.
   * @param local_address is the socket's local address used to populate port.
   * @param udp_packet_processor is the callback to receive the packet.
   * @param receive_time is the timestamp passed to udp_packet_processor for the
   * receive time of the packet.
   * @param recv_msg_method the type of system call and socket options combination to use when
   * receiving packets from the kernel.
   * @param packets_dropped is the output parameter for number of packets dropped in kernel. If the
   * caller is not interested in it, nullptr can be passed in.
   * @param num_packets_read is the output parameter for the number of packets passed to the
   * udp_packet_processor in this call. If the caller is not interested in it, nullptr can be passed
   * in.
   */
  static Api::IoCallUint64Result
  readFromSocket(IoHandle& handle, const Address::Instance& local_address,
                 UdpPacketProcessor& udp_packet_processor, MonotonicTime receive_time,
                 UdpRecvMsgMethod recv_msg_method, uint32_t* packets_dropped,
                 uint32_t* num_packets_read);

  /**
   * Read some packets from a given UDP socket and pass the packet to a given
   * UdpPacketProcessor. Read no more than MAX_NUM_PACKETS_PER_EVENT_LOOP packets.
   * @param handle is the UDP socket to read from.
   * @param local_address is the socket's local address used to populate port.
   * @param udp_packet_processor is the callback to receive the packets.
   * @param time_source is the time source used to generate the time stamp of the received packets.
   * @param allow_gro whether to use GRO, iff the platform supports it. This function will check
   * the IoHandle to ensure the platform supports GRO before using it.
   * @param allow_mmsg whether to use recvmmsg, iff the platform supports it. This function will
   * check the IoHandle to ensure the platform supports recvmmsg before using it. If `allow_gro` is
   * true and the platform supports GRO, then it will take precedence over using recvmmsg.
   * @param packets_dropped is the output parameter for number of packets dropped in kernel.
   * Return the io error encountered or nullptr if no io error but read stopped
   * because of MAX_NUM_PACKETS_PER_EVENT_LOOP.
   *
   * TODO(mattklein123): Allow the number of packets read to be limited for fairness. Currently
   *                     this function will always return an error, even if EAGAIN. In the future
   *                     we can return no error if we limited the number of packets read and have
   *                     to fake another read event.
   * TODO(mattklein123): Can we potentially share this with the TCP stack somehow? Similar code
   *                     exists there.
   */
  static Api::IoErrorPtr readPacketsFromSocket(IoHandle& handle,
                                               const Address::Instance& local_address,
                                               UdpPacketProcessor& udp_packet_processor,
                                               TimeSource& time_source, bool allow_gro,
                                               bool allow_mmsg, uint32_t& packets_dropped);

#if defined(__linux__)
  /**
   * Changes the calling thread's network namespace to the one referenced by the file at `netns`,
   * calls the function `f`, and returns its result after switching back to the original network
   * namespace.
   *
   * @param f the function to execute in the specified network namespace.
   * @param netns filepath referencing the network namespace in which `f` is executed.
   * @return the result of 'f' wrapped in absl::StatusOr to any indicate syscall failures.
   */
  template <typename Func>
  static auto execInNetworkNamespace(Func&& f, const char* netns)
      -> absl::StatusOr<typename std::invoke_result_t<Func>> {
    Api::OsSysCalls& posix = Api::OsSysCallsSingleton().get();

    // Open the original netns fd, so that we can return to it.
    constexpr auto curr_netns_file = "/proc/self/ns/net";
    auto og_netns_fd_result = posix.open(curr_netns_file, O_RDONLY);
    int og_netns_fd = og_netns_fd_result.return_value_;
    if (og_netns_fd_result.errno_ != 0) {
      return absl::InternalError(fmt::format("failed to open netns file {}: {}", curr_netns_file,
                                             errorDetails(og_netns_fd_result.errno_)));
    }
    Cleanup cleanup_og_fd([&og_netns_fd, &posix]() { posix.close(og_netns_fd); });

    // Open the fd for the network namespace we want the socket in.
    auto netns_fd_result = posix.open(netns, O_RDONLY);
    const int netns_fd = netns_fd_result.return_value_;
    if (netns_fd <= 0) {
      return absl::InternalError(fmt::format("failed to open netns file {}: {}", netns,
                                             errorDetails(netns_fd_result.errno_)));
    }
    Cleanup cleanup_netns_fd([&posix, &netns_fd]() { posix.close(netns_fd); });

    // Change the network namespace of this thread.
    auto setns_result = Api::LinuxOsSysCallsSingleton().get().setns(netns_fd, CLONE_NEWNET);
    if (setns_result.return_value_ != 0) {
      return absl::InternalError(fmt::format("failed to set netns to {} (fd={}): {}", netns,
                                             netns_fd, errorDetails(errno)));
    }

    // Calling function from the specified network namespace.
    auto result = std::forward<Func>(f)();

    // Restore the original network namespace before returning the function result.
    setns_result = Api::LinuxOsSysCallsSingleton().get().setns(og_netns_fd, CLONE_NEWNET);
    RELEASE_ASSERT(
        setns_result.return_value_ == 0,
        fmt::format("failed to restore original netns (fd={}): {}", netns_fd, errorDetails(errno)));

    return result;
  }
#endif

private:
  /**
   * Takes a number and flips the order in byte chunks. The last byte of the input will be the
   * first byte in the output. The second to last byte will be the second to first byte in the
   * output. Etc..
   * @param input supplies the input to have the bytes flipped.
   * @return the absl::uint128 of the input having the bytes flipped.
   */
  static absl::uint128 flipOrder(const absl::uint128& input);
};

/**
 * Log formatter for an address.
 */
struct AddressStrFormatter {
  void operator()(std::string* out, const Network::Address::InstanceConstSharedPtr& instance) {
    out->append(instance->asString());
  }
};

} // namespace Network
} // namespace Envoy
