# Hệ Thống Ví Điểm (Wallet System)

[![GitHub](https://img.shields.io/github/license/sssSonsss/exercise_c_plus)](https://github.com/sssSonsss/exercise_c_plus/blob/main/LICENSE)
[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/std/the-standard)
[![CMake](https://img.shields.io/badge/CMake-3.10+-green.svg)](https://cmake.org/)

Hệ thống quản lý ví điểm với các tính năng bảo mật và xác thực người dùng. Dự án được phát triển bằng C++17, sử dụng OpenSSL cho mã hóa và xác thực.

## Tính Năng Chính

### Quản Lý Người Dùng
- Đăng ký tài khoản với email
- Đăng nhập với xác thực mật khẩu
- Đổi mật khẩu
- Khóa tài khoản sau 5 lần đăng nhập sai
- Tự động mở khóa sau 30 phút

### Quản Lý Ví
- Tạo ví tự động khi đăng ký
- Xem số dư
- Chuyển điểm với xác thực OTP
- Giới hạn số lần chuyển điểm trong ngày (tối đa 10 lần)
- Giới hạn số điểm tối đa (10,000,000 điểm)

### Bảo Mật
- Mật khẩu được mã hóa bằng SHA-256
- Xác thực OTP qua email
- Giới hạn số lần chuyển điểm
- Khóa tài khoản tạm thời khi có dấu hiệu bất thường
- Kiểm tra và xác thực đầu vào

### Quản Trị
- Tạo tài khoản người dùng mới
- Xem danh sách người dùng
- Sao lưu và khôi phục dữ liệu

## Yêu Cầu Hệ Thống

- C++17 trở lên
- CMake 3.10 trở lên
- OpenSSL 3.0 trở lên
- macOS (đã test trên macOS)

## Cài Đặt

1. Clone repository:
```bash
git clone https://github.com/sssSonsss/exercise_c_plus.git
cd exercise_c_plus
```

2. Cài đặt OpenSSL (nếu chưa có):
```bash
brew install openssl@3
```

3. Build dự án:
```bash
mkdir build && cd build
cmake ..
make
```

## Sử Dụng

1. Chạy chương trình:
```bash
./wallet_system
```

2. Làm theo hướng dẫn trên màn hình:
   - Đăng ký tài khoản mới
   - Đăng nhập
   - Sử dụng các tính năng của hệ thống

## Cấu Trúc Dự Án

```
exercise_c_plus/
├── include/
│   ├── database.h    # Quản lý cơ sở dữ liệu
│   ├── user.h        # Quản lý người dùng
│   ├── wallet.h      # Quản lý ví
│   ├── transaction.h # Quản lý giao dịch
│   └── otp.h         # Xác thực OTP
├── src/
│   ├── main.cpp      # Điểm vào chương trình
│   ├── database.cpp  # Triển khai database
│   ├── user.cpp      # Triển khai user
│   ├── wallet.cpp    # Triển khai wallet
│   ├── transaction.cpp # Triển khai transaction
│   └── otp.cpp       # Triển khai OTP
├── CMakeLists.txt    # Cấu hình build
└── README.md         # Tài liệu dự án
```

## Bảo Mật

- Mật khẩu được mã hóa bằng SHA-256
- Xác thực OTP qua email cho các giao dịch
- Giới hạn số lần chuyển điểm trong ngày
- Khóa tài khoản tạm thời khi có dấu hiệu bất thường
- Kiểm tra và xác thực đầu vào

## Đóng Góp

Mọi đóng góp đều được hoan nghênh. Vui lòng:
1. Fork dự án
2. Tạo branch mới (`git checkout -b feature/AmazingFeature`)
3. Commit thay đổi (`git commit -m 'Add some AmazingFeature'`)
4. Push lên branch (`git push origin feature/AmazingFeature`)
5. Tạo Pull Request

## Giấy Phép

Dự án này được phân phối dưới giấy phép MIT. Xem file `LICENSE` để biết thêm chi tiết.

## Liên Hệ

Nguyễn Sơn - [GitHub](https://github.com/sssSonsss)

Project Link: [https://github.com/sssSonsss/exercise_c_plus](https://github.com/sssSonsss/exercise_c_plus) 