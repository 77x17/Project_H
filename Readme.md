# Tiến độ
- 08/06/2025: Start
- 09/06/2025: [Enemies] 1 |                                  | [Items]  0 | [Regions] 1
- 10/06/2025: [Enemies] 2 |                                  | [Items]  0 | [Regions] 2
- 12/06/2025: [Enemies] 2 | [Quests] 1 | [Npcs] 2            | [Items]  0 | [Regions] 2
- 18/06/2025: [Enemies] 2 | [Quests] 1 | [Npcs] 3            | [Items]  4 | [Regions] 2 | [Particle] 2
- 22/06/2025: [Enemies] 2 | [Quests] 2 | [Npcs] 4 | [Boss] 1 | [Items] 10 | [Regions] 4 | [Particle] 2
- 26/06/2025: [Enemies] 4 | [Quests] 4 | [Npcs] 6 | [Boss] 2 | [Items] 19 | [Regions] 7 | [Particle] 2

# Những kinh nghiệm rút ra khi làm dự án này
- Đôi khi lỗi không đến từ những hàm mới thêm vào mà từ những hàm cũ.
- modulo 0, float modulo (Error)
- I hate collision
- Jitter: Màn hình bị giật hở khi thay đổi view -> Vì view có tọa độ dạng số thực -> static_cast<unsigned int>
- Screen Tearing: Màn hình bị xọc -> Bất đồng bộ với tần số quét của màn hình
- Attack cooldown: Tránh trường hợp quái tấn công ngay lập tức 
- Invincible: Bất tử khi mới respawn
- Nên dùng switch case thay vì if quá nhiều
- Minimap nên dùng ảnh vẽ một lần rồi tính toán kéo thả chứ đừng vẽ lại.
- Nên vẽ màn hình vào một texture rồi vẽ lên màn hình thay vì vẽ thẳng.
- Khi làm shader bóng tối, view bị thay đổi leap đi theo player nên phải cần một instant view.
- Vị trí trong OpenGL tăng từ dưới lên
- float length != 0 suy ra chia ? => BUG BUG BUG
- Bug 2 ngày vì float / 10.0f quá nhỏ tiệm cận 0 => 0
- Lưu std::vector< std::pair<> > sẽ nhanh hơn dùng std::unordered_map<> thường nếu không cần .at()
- Nên dynamic_cast<> thay vì thêm một attribute vào Base class một cách không cần thiết
- Khi đa hình thì phải dùng hàm virtual clone() = 0 cho Base class để copy không bị Slicing data
- Quy tắc đặt tên, quản lý Entity sẽ dùng tên số ít + Manager, quản lý tạo sẽ là Database hoặc Factory
- Nên dùng Forward declaration khi chỉ cần truyền tham chiếu hoặc con trỏ vào method (function)