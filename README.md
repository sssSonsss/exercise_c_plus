# Hệ Thống Ví Điểm (Wallet System)

Hệ thống quản lý ví điểm với các tính năng bảo mật và xác thực người dùng.

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
- Giới hạn số lần chuyển điểm trong ngày
- Giới hạn số điểm tối đa

### Bảo Mật
- Mật khẩu được mã hóa bằng SHA-256
- Xác thực OTP qua email
- Giới hạn số lần chuyển điểm
- Khóa tài khoản tạm thời khi có dấu hiệu bất thường

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

1. Cài đặt OpenSSL:
```bash
brew install openssl@3
```

2. Clone repository:
```bash
git clone [repository-url]
cd wallet_system
```

3. Tạo thư mục build và build dự án:
```bash
mkdir build
cd build
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
wallet_system/
├── include/
│   ├── database.h
│   ├── user.h
│   ├── wallet.h
│   ├── transaction.h
│   └── otp.h
├── src/
│   ├── main.cpp
│   ├── database.cpp
│   ├── user.cpp
│   ├── wallet.cpp
│   ├── transaction.cpp
│   └── otp.cpp
├── CMakeLists.txt
└── README.md
```

## Bảo Mật

- Mật khẩu được mã hóa bằng SHA-256
- Xác thực OTP qua email cho các giao dịch
- Giới hạn số lần chuyển điểm trong ngày
- Khóa tài khoản tạm thời khi có dấu hiệu bất thường
- Kiểm tra và xác thực đầu vào

## Đóng Góp

Mọi đóng góp đều được hoan nghênh. Vui lòng tạo issue hoặc pull request để đóng góp.

## Giấy Phép

[MIT License](LICENSE) 