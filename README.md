# CatFeedingSysESP32BÁO CÁO BÀI TẬP LỚN
Môn học: IoT và ứng dụng

Tên đề tài
Hệ thống giám sát và chăm sóc thú cưng thông minh

Thành viên nhóm và phân công
-Nguyễn Công Nhật Minh: Lập trình ESP32 cho các modules


Nội dung thực hiện
1.Giới thiệu đề tài:
    Đề tài "Hệ Thống Chăm Sóc Thú Cưng Thông Minh" là một dự án kết hợp giữa công nghệ và yêu thú cưng. Với cuộc sống ngày càng bận rộn, việc chăm sóc và quan sát thú cưng trở nên khó khăn.
     Trong khi đó, với sự phát triển của công nghệ hiện nay, việc ứng dụng IOT là một phương án thú vị. Với hệ thống này, người dùng không cần phải ở cạnh thú cưng của mình để quan sát cũng như cho ăn mà họ có thể sử dụng một trang web để thực hiện việc đó từ xa. Thiết bị cho phép người dùng tự động cho mèo ăn từ xa. Thiết bị này sử dụng kết nối Wi-Fi để kết nối với điện thoại thông minh hoặc máy tính bảng của người dùng. Người dùng có thể sử dụng ứng dụng dành cho thiết bị di động để kiểm soát thiết bị, bao gồm cài đặt thời gian cho ăn và xem camera tích hợp để theo dõi mèo

2.Các thiết bị trong hệ thống
   Thiết bị này bao gồm các thành phần sau:

Hai bo mạch ESP32
Một camera 5MP
Một cảm biến siêu âm
Một động cơ motor
Một bộ nguồn
Một hộp đựng


3.Mục tiêu của dự án

Theo Dõi Thú Cưng từ Xa: Cho phép người dùng xem hình ảnh và video thú cưng từ xa thông qua trình duyệt web hoặc ứng dụng di động. Điều này giúp họ luôn ở gần thú cưng dù ở bất kỳ đâu.

Cho Ăn Tự Động: Cung cấp khả năng cho ăn thú cưng tự động thông qua việc đặt lịch trình hoặc kích hoạt từ xa. Người dùng có thể thực hiện việc này một cách tiện lợi và linh hoạt.

4.Cách thức hoạt động

Thu Thập Dữ Liệu Hình Ảnh: Camera ESP32-CAM được đặt trong khu vực thú cưng để thu thập hình ảnh và video của thú cưng. Camera này có khả năng chụp hình và ghi video.

Xử Lý Dữ Liệu Hình Ảnh: Dữ liệu hình ảnh thu thập từ camera được truyền đến mô hình học máy được tích hợp trong ESP32-CAM. Mô hình này đã được huấn luyện trước để nhận diện mèo trong hình ảnh.

Nhận Diện Mèo: Mô hình học máy sẽ xử lý dữ liệu hình ảnh và thực hiện nhận diện xem có mèo trong hình hay không. Nếu mèo được nhận diện, mô hình sẽ tạo một tín hiệu.

Kích Hoạt Cho Ăn: Khi mô hình nhận diện mèo, nó sẽ gửi tín hiệu đến servo motor để kích hoạt chức năng cho ăn. Servo motor sẽ mở nắp bát đựng thức ăn để cho phép thú cưng được cung cấp thức ăn.

Giao Diện App : Ngoài việc kích hoạt cho ăn tự động, Giao diện web cho phép họ xem hình ảnh và video trực tiếp, đặt lịch trình cho ăn và tương tác với hệ thống từ xa.


5.Sơ đồ khối tổng quát



Khối nguồn: cung cấp nguồn cho hệ thống hoạt động
Khối xử lý: ESP32 thu thập dữ liệu, thực hiện kết nối và truyền nhận tín hiệu.
Khối hiển thị: Giao diện app hiển thị hình ảnh và giao diện


Kết quả thực hiện


giao diện chính của app

Tổng quan hệ thống

Mã nguồn hệ thống:
