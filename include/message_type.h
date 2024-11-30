#ifndef MESSAGE_TYPE_H
#define MESSAGE_TYPE_H

typedef enum
{
    REGISTER           = 0x01,  // Client Thông điệp đăng ký
    LOGIN              = 0x02,  // Client Thông điệp đăng nhập
    CREATE_ROOM        = 0x03,  // Client Tạo phòng đấu giá mới
    DELETE_ROOM        = 0x04,  // Client Tạo phòng đấu giá mới
    CREATE_ITEM        = 0x05,  // Client Tạo vật phẩm đấu giá
    DELETE_ITEM        = 0x06,  // Client Xóa vật phẩm đấu giá
    JOIN_AUCTION       = 0x07,  // Client Tham gia phiên đấu giá
    BID                = 0x08,  // Client Gửi giá đấu
    UPDATE_AUCTION     = 0x09,  // Server Cập nhật trạng thái phiên đấu giá
    RESULT             = 0x0A,  // Server gửi kết quả phiên đấu giá
    NOTIFICATION       = 0x0B,  // Server gửi thông báo phiên đấu giá sắp kết thúc
    BUY_NOW            = 0x0C,  // Client yêu cầu mua vật phẩm với giá bán ngay
    LOGOUT             = 0x0D,  // Client yêu cầu đăng xuất
    ERROR              = 0x0E,  // Server thông báo lỗi
} MessageType;

#endif
