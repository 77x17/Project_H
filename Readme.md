# Một số ảnh demo minh họa:

Main Screen
![MainScreen](Assets/MainScreen.png)

Menu Screen
![MenuScreen](Assets/MenuScreen.png)
![MapScreen](Assets/MapScreen.png)
![TalkAndQuestsScreen](Assets/TalkAndQuestsScreen.png)
![MonsterAndItemScreen](Assets/MonsterAndItemScreen.png)
![InventoryScreen](Assets/InventoryScreen.png)
![BossesScreen](Assets/BossesScreen.png)
![Biome1Screen](Assets/Biome1Screen.png)
![Biome2Screen](Assets/Biome2Screen.png)
![Biome3Screen](Assets/Biome3Screen.png)
![Biome4Screen](Assets/Biome4Screen.png)
![AnimationTilesScreen](Assets/AnimationTilesScreen.png)


# Tiến độ phát triển
| Ngày       | Enemies | Quests | NPCs | Bosses | Items  | Regions | Particles |
|------------|---------|--------|------|--------|--------|---------|-----------|
| 08/06/2025 | 0       | 0      | 0    | 0      | 0      | 0       | 0         |
| 09/06/2025 | 1       | 0      | 0    | 0      | 0      | 1       | 0         |
| 10/06/2025 | 2       | 0      | 0    | 0      | 0      | 2       | 0         |
| 12/06/2025 | 2       | 1      | 2    | 0      | 0      | 2       | 0         |
| 18/06/2025 | 2       | 1      | 3    | 0      | 4      | 2       | 2         |
| 22/06/2025 | 2       | 2      | 4    | 1      | 10     | 4       | 2         |
| 26/06/2025 | 4       | 4      | 6    | 2      | 19     | 7       | 2         |

# Mục tiêu cần thực hiện
- 26/06/2025: [✔] Refactor lại hệ thống Quest để có thể lưu và dễ dàng quản lý (nâng cấp thành MMO-RPG về sau), tách Quest thành QuestProgress, QuestManager, QuestDatabase ...
- 27/06/2025: [✔] Enemies, Quests, Npcs, Bosses, Items, Regions, Particles đã ổn và có thể quản lý thành hệ thống client - server.
- 27/06/2025: [✔] Thêm [Esc] UI để Save, Load và Quit.
- 28/06/2025: [✔] Hoàn thành lưu game, load game cho quests, inventory, player.information.
- 02/07/2025: [ ] Thêm quests và làm thêm tương tác với maps để giải đố, mở cửa, mở các khu vực mới vv...

# Những kinh nghiệm rút ra khi làm dự án này
- Đôi khi lỗi không đến từ những hàm mới thêm vào mà từ những hàm cũ.
- modulo 0, float modulo (Error)
- I hate collision
- Jitter: Màn hình bị giật hở khi thay đổi view -> Vì view có tọa độ dạng số thực -> static_cast< unsigned int >
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
