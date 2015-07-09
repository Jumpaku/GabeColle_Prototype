
# include <Siv3D.hpp>

class Memory;

class Object
{
	friend Memory;
	bool exists_m;
private:
	bool exists()const
	{
		return exists_m;
	}
	void construct()
	{
		exists_m = true;
	}
	void destruct()
	{
		exists_m = false;
	}
};

class CircleObject : public Object
{
	double x_m;
	double y_m;
public:
	Vec2 center()const
	{
		return Vec2(x_m, y_m);
	}
	void center(Vec2 v)
	{
		x(v.x);
		y(v.y);

	}
	void x(double x)
	{
		x_m = x;
	}
	void y(double y)
	{
		y_m = y;
	}
};

struct Error
{
	int segmentationFault_m = 0;
	int nullptrAccess_m = 0;
	int outOfMemory_m = 0;
	int addressOutOfBounds_m = 0;
};

class Memory
{
public:
	typedef std::vector<bool> MarkingMap_t;
	typedef int Address_t;
	typedef std::vector<CircleObject> Memory_t;
	typedef std::vector<std::vector<bool>> RelationMatix_t;
private:
	Memory_t memory_m;
	RelationMatix_t relation_m;
	Error error_m;
private:
	void check(Address_t address)
	{
		if (hasExpired(address)) {
			++error_m.segmentationFault_m;
		}
		else if (address == 0) {
			++error_m.nullptrAccess_m;
		}
		else if (address <= -1 || size() <= address) {
			++error_m.addressOutOfBounds_m;
		}
	}
public:
	Memory(int size)
	{
		memory_m = std::move(std::vector<CircleObject>(size));
		memory_m[0].construct();
		relation_m = std::move(std::vector<std::vector<bool>>(size, std::vector<bool>(size, false)));
	}
	Address_t alloc()
	{
		for (Address_t address(1); address < size(); ++address) {
			if (!memory_m[address].exists()) {
				memory_m[address].construct();
				return address;
			}
		}
		++error_m.outOfMemory_m;
		return 0;
	}
	void free(Address_t address)
	{
		if (address != 0) {
			for (Address_t i(0); i < size(); ++i) {
				unlink(address, i);
			}
			memory_m[address].destruct();
		}
	}
	void link(Address_t from, Address_t to)
	{
		if (!hasExpired(from) && !hasExpired(to)) {
			relation_m[from][to] = true;
		}
	}
	void unlink(Address_t from, int to)
	{
		relation_m[from][to] = false;
	}
	MarkingMap_t mark()const
	{
		MarkingMap_t map(size());
		mark(0, map);
		return map;
	}
	void mark(Address_t address, MarkingMap_t &map)const
	{
		for (Address_t i(0); i < size(); ++i) {
			if (map[i] == false && relation_m[address][i]) {
				map[i] = true;
				mark(i, map);
			}
		}
	}
	void sweep(MarkingMap_t const &map)
	{
		for (Address_t address(1); address < size(); ++address) {
			if (map[address] == false) {
				free(address);
			}
		}
	}
	void gc()
	{
		sweep(mark());
	}
	bool hasExpired(Address_t address)
	{
		return !memory_m[address].exists();
	}
	CircleObject &operator[](Address_t address)
	{
		check(address);
		return memory_m[address];
	}
	MarkingMap_t getExistanceMap()const
	{
		return mark();
	}
	RelationMatix_t const &getRelationMatrix()const
	{
		return relation_m;
	}
	int size()const
	{
		return memory_m.size();
	}
	Error error()const
	{
		return error_m;
	}
	CircleObject &root()
	{
		return memory_m[0];
	}
};

class Controller
{
	GUI gui_m;
public:
	Controller() :gui_m(GUIStyle::Default)
	{
		gui_m.setCenter(Window::Center());
		gui_m.setTitle(L"Controler");

		gui_m.add(L"from", GUITextField::Create(none));
		gui_m.addln(L"from", GUIText::Create(L"from"));

		gui_m.add(L"to", GUITextField::Create(none));
		gui_m.addln(L"to  ", GUIText::Create(L"to"));

		gui_m.add(L"link", GUIButton::Create(L"link"));
		gui_m.addln(L"unlink", GUIButton::Create(L"unlink"));

		gui_m.add(L"alloc", GUIButton::Create(L"alloc"));
		gui_m.add(L"gc", GUIButton::Create(L"gc"));
		gui_m.addln(L"quit", GUIButton::Create(L"quit"));

		gui_m.addln(L"sf", GUIText::Create(ToString(0) + L" : segmentation faults"));
		gui_m.addln(L"na", GUIText::Create(ToString(0) + L" : nullptr accesses"));
		gui_m.addln(L"om", GUIText::Create(ToString(0) + L" : out of memory"));
		gui_m.addln(L"ob", GUIText::Create(ToString(0) + L" : address out of bounds"));
	}
	void update(Memory &memory)
	{
		if (gui_m.button(L"alloc").pushed) {
			memory[memory.alloc()].center(RandomVec2({ 200, 600 }, { 50, Window::Height()-50 }));
		}
		auto from = gui_m.textField(L"from").text;
		auto to = gui_m.textField(L"to").text;
		if (std::regex_match(from.str(), std::wregex(L"^[0-9]$|^[1-9][0-9]$|^100$")) &&
			std::regex_match(to.str(), std::wregex(L"^[0-9]$|^[1-9][0-9]$|^100$"))) {
			if (gui_m.button(L"link").pushed) {
				memory.link(FromString<int>(from), FromString<int>(to));
			}
			else if (gui_m.button(L"unlink").pushed) {
				memory.unlink(FromString<int>(from), FromString<int>(to));
			}
			
		}
		if (gui_m.button(L"gc").pushed) {
			memory.gc();
		}
		if (gui_m.button(L"quit").pushed) {
			System::Exit();
		}

		gui_m.text(L"sf").text = ToString(memory.error().segmentationFault_m) + L" : segmentation faults";
		gui_m.text(L"na").text = ToString(memory.error().nullptrAccess_m) + L" : nullptr accesses";
		gui_m.text(L"om").text = ToString(memory.error().outOfMemory_m) + L" : out of memory";
		gui_m.text(L"ob").text = ToString(memory.error().addressOutOfBounds_m) + L" : address out of bounds";
	}
};

void viewMemory(Memory &memory);

void Main()
{
	Font font;
	Memory m(100);
	Controller gui;
	Graphics::SetBackground(Color(160, 200, 100));
	Window::SetFullscreen(false, Point(1280, 720));
	Window::SetStyle(WindowStyle::NonFrame);
	Window::Centering();	
	m.root().center({ 150.0, Window::Height() / 2.0 });

	while (System::Update()) {
		gui.update(m);
		viewMemory(m);
	}
}


void viewMemory(Memory &memory)
{
	static Font font;
	font.drawCenter(L"Root", Circle(memory.root().center(), 50.0).draw(Palette::Aqua).center);
	for (int i(1); i < memory.size(); ++i) {
		double const r = 40.0;
		if (!memory.hasExpired(i)) {
			Circle c = Circle(memory[i].center(), r);
			if (c.leftClicked) {
				memory.free(i);
			}
			else if (c.mouseOver) {
				c.draw(Color(Palette::Pink, 128)).drawFrame(0.0, 3.0, Palette::Red);
			}
			else {
				c.draw(Color(Palette::Red, 128)).drawFrame(0.0, 3.0, Palette::Darkred);
			}
			font.drawCenter(ToString(i), c.center);
		}
	}

	auto rm = memory.getRelationMatrix();
	for (int j(1); j < memory.size(); ++j) {
		if (rm[0][j] == true) {
			Line(memory.root().center(), memory[j].center()).drawArrow(4, { 20, 50 }, Color(Palette::Yellow, 128));
		}
		else if (rm[j][0] == true) {
			Line(memory[j].center(), memory.root().center()).drawArrow(4, { 20, 50 }, Color(Palette::Yellow, 128));
		}
	}
	for (int i(1); i < memory.size(); ++i) {
		for (int j(1); j < memory.size(); ++j) {
			if (rm[i][j] == true) {
				if (i == j) {
					Circle(Vec2(40.0, -40.0).movedBy(memory[i].center()), 40.0).drawArc(-Math::Pi / 2.0, 3.0 / 2.0 * Math::Pi, 2.0, 2.0, Color(Palette::Yellow, 128));
					Line(Vec2(0.0, -40.0).movedBy(memory[i].center()), memory[i].center()).drawArrow(4, { 20, 50 }, Color(Palette::Yellow, 128));
					Line(Vec2(40.0, 0.0).movedBy(memory[i].center()), memory[i].center()).draw(4, Color(Palette::Yellow, 128));
				}
				else {
					Line(memory[i].center(), memory[j].center()).drawArrow(4, { 20, 50 }, Color(Palette::Yellow, 128));
				}
			}
		}
	}
}
