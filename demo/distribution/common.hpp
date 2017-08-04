#ifndef __MDP_COMMON_H__
#define __MDP_COMMON_H__

#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <zmqpp/zmqpp.hpp>

namespace mdp {

typedef std::chrono::time_point<std::chrono::system_clock> time_point_t;
typedef std::chrono::milliseconds duration_ms_t;

std::ostream& operator<< (std::ostream& out, const zmqpp::message& msg);
std::string toHex(const std::string& in);

// Heartbeat
const size_t HEARTBEAT_LIVENESS = 3;            //  3-5 is reasonable
const duration_ms_t HEARTBEAT_INTERVAL (2500);  //  msecs
const duration_ms_t HEARTBEAT_EXPIRY = HEARTBEAT_INTERVAL * HEARTBEAT_LIVENESS;

// Socket identifiers, as integers
const std::uint8_t SOCKET_BROKER = 1;
const std::uint8_t SOCKET_APPLICATION = 2;
const std::uint8_t SOCKET_L_NEIGHBOUR = 3;
const std::uint8_t SOCKET_R_NEIGHBOUR = 4;

//  This is the version of MDP/Client we implement
const std::string MDPC_CLIENT = "MDPC0X";

//  MDP/Client commands, as strings
const std::string MDPC_REQUEST = "\001";
const std::string MDPC_REPORT = "\002";
const std::string MDPC_NAK = "\003";

static std::string mdpc_commands [] = {
    "INVALID_CMD", "REQUEST", "REPORT", "NAK",
};

//  This is the version of MDP/Worker we implement
const std::string MDPW_WORKER = "MDPW0X";

//  MDP/Worker commands, as strings
const std::string MDPW_READY = "\001";
const std::string MDPW_REQUEST = "\002";
const std::string MDPW_REPORT = "\003";
const std::string MDPW_HEARTBEAT = "\004";
const std::string MDPW_DISCONNECT = "\005";

static std::string mdpw_commands [] = {
    "INVALID_CMD", "READY", "REQUEST", "REPORT", "HEARTBEAT", "DISCONNECT"
};

struct DistSharedInfo {
    zmqpp::reactor *reactor_;            //  Polling handler
    zmqpp::context *ctx_;
    zmqpp::socket *app_socket_;
    std::string app_endpoint_;
    std::string identity_;
    bool verbose_;
    bool zctx_interrupted_ = false;  // ZMQ interrupted by signal
};

}

#endif // __MDP_COMMON_H__