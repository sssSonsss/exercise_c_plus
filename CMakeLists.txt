cmake_minimum_required(VERSION 3.10)
project(wallet_system)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenSSL paths for macOS
set(OPENSSL_ROOT_DIR "/opt/homebrew/opt/openssl@3")
set(OPENSSL_INCLUDE_DIR "/opt/homebrew/opt/openssl@3/include")
set(OPENSSL_LIBRARIES "/opt/homebrew/opt/openssl@3/lib")

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(wallet_system 
    src/main.cpp
    src/user.cpp
    src/wallet.cpp
    src/transaction.cpp
    src/otp.cpp
    src/database.cpp
)

target_link_libraries(wallet_system ${OPENSSL_LIBRARIES})