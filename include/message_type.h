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
    JOIN_ROOM          = 0x07,  // Client Tham gia phiên đấu giá
    EXIT_ROOM          = 0x08,  // Client Tham gia phiên đấu giá
    FETCH_ALL_ROOMS    = 0x09,  // Client yêu cầu lấy danh sách tất cả các phòng đấu giá hiện có
    FETCH_OWN_ROOMS    = 0x0A,  // Client yêu cầu lấy danh sách phòng đấu giá đang sở hữu
    FETCH_ITEMS        = 0x0B,  // Client yêu cầu lấy danh sách vật phẩm trong phòng đấu giá
    START_AUCTION      = 0x0C,  // Client (Nếu là chủ phòng) gửi yêu cầu bắt đầu phiên đấu giá
    BID                = 0x0D,  // Client Gửi giá đấu
    UPDATE_AUCTION     = 0x0E,  // Server Cập nhật trạng thái phiên đấu giá
    RESULT             = 0x0F,  // Server gửi kết quả phiên đấu giá
    NOTIFICATION       = 0x10,  // Server gửi thông báo phiên đấu giá sắp kết thúc
    BUY_NOW            = 0x11,  // Client yêu cầu mua vật phẩm với giá bán ngay
    LOGOUT             = 0x12,  // Client yêu cầu đăng xuất
    ERROR              = 0x00,  // Server thông báo lỗi
    REFRESH            = -1,  // Server yêu cầu reload
} MessageType;

void logSystem(const char *agent, const char *logType, const char *format, ...);

#endif
