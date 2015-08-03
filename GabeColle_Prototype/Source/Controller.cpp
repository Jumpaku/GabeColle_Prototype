#include"Controller.h"
#include"gc\Memory.h"
#include"CircleObject.h"

Controller::Controller() :gui_m(GUIStyle::Default)
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

void Controller::update(gc::Memory<CircleObject> &memory)
{
	if (gui_m.button(L"alloc").pushed) {
		int p = memory.alloc();
		if (p != 0) {
			memory.access(p).center(RandomVec2({ 200, 600 }, { 50, Window::Height() - 50 }));
		}
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
