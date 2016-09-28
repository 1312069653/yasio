//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2013-2014 xseekerj
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <memory>
#include <typeinfo>
#include "boost/asio.hpp"
#include "server.hpp"
#include "initd.h"
//#include "utils/mathext.h"

#include "utils/xxsocket.h"
#include "http_client.hpp"
//#include <thelib/process_manager.h>
#include "utils/crypto_wrapper.h"
#include "server_logger.hpp"
#include <unordered_map>
#include <map>
#include "tls.hpp"
#include "utils/crypto_wrapper.h"

//#define ngx_align(d, a)     (((d) + (a - 1)) & ~(a - 1))

// tcp::server::server* this_server;

#define SHARED_IVEC  { 0x9c, 0x95, 0x7a, 0x5a, 0x3e, 0xdd, 0xa3, 0xff, 0x57, 0x6a, 0x19, 0x1c, 0xb6, 0xa3, 0xc6, 0x62 }

int main(int argc, char* argv[])
{
#ifdef _WIN32
    enable_leak_check();
#else
    sinitd();
#endif

    LOG_WARN_ALL("program bit detect...");
    if(sizeof(void*) == 8) {
        LOG_WARN_ALL("this is a x86_64 program.");
    }
    else {
        LOG_WARN_ALL("this is a x86_32 program.");
    }

    // ���ü���IVEC, ��Ҫ����
    const unsigned char sharedIvec[] = SHARED_IVEC;
    crypto::aes::detail::set_ivec(sharedIvec);

  //  gls_ctx_init();

    try
    {
        // Check command line arguments.

        const char* address = "0.0.0.0";
        const char* port = "8080";

        if(argc >= 2) {
            address = argv[1];
        }

        if(argc >= 3) {
            port = argv[2];
        }

        // CR_SERVER_LOST(2013) CR_SERVER_GONE_AWAY(2006) CR_PRIMARY_CONFLICIT(1062)

        // Initialise the server.

        LOG_WARN_ALL("listen at port:[%s]", port);
        tcp::server::server s(address, port, 32);

        // the_server = &s;

        // Run the server until stopped.
        s.start();
    }
    catch (std::exception& e)
    {
        LOG_ERROR_ALL("exception:", e.what());
        // std::cerr << "exception: " << e.what() << "\n";
    }

  //  gls_ctx_end();

    return 0;
}

