#include <string>
#include <list>
#include "test_runner.h"
using namespace std;

class Editor {
public:
	Editor() {
		pointer = editor.begin();
	}
	void Left() {
		if (pointer != editor.begin()) {
			--pointer;
		}
	}
	void Right() {
		if (pointer != editor.end()) {
			++pointer;
		}
	}
	void Insert(char token) {
		editor.insert(pointer, token);
	}
	void Cut(size_t tokens = 1) {
		buffer.clear();
		auto buffer_begin{ pointer }, buffer_end{ pointer };
		size_t length = 0;
		while (buffer_end != editor.end() && length != tokens ) {
			++length;
			++buffer_end;
			Right();
		}
		buffer.splice(buffer.begin(), editor, buffer_begin, buffer_end);
	}
	void Copy(size_t tokens = 1) {
		buffer.clear();
		auto buffer_end = pointer;
		size_t length = 0;
		while (buffer_end != editor.end() && length != tokens) {
			++length;
			++buffer_end;
		}
		if (pointer != buffer_end)
			buffer = { pointer, buffer_end };
	}
	void Paste() {
		for (auto i = buffer.begin(); i != buffer.end(); ++i) {
			Insert(*i);
		}
	}
	string GetText() const {
		return { editor.begin(), editor.end() };
	}
private:
	list<char>::iterator pointer;
	list<char> editor;
	list<char> buffer;
};

void TypeText(Editor& editor, const string& text) {
	for (char c : text) {
		editor.Insert(c);
	}
}

void TestEditing() {
	{
		Editor editor;

		const size_t text_len = 12;
		const size_t first_part_len = 7;
		TypeText(editor, "hello, world");
		for (size_t i = 0; i < text_len; ++i) {
			editor.Left();
		}
		editor.Cut(first_part_len);
		for (size_t i = 0; i < text_len - first_part_len; ++i) {
			editor.Right();
		}
		TypeText(editor, ", ");
		editor.Paste();
		editor.Left();
		editor.Left();
		editor.Cut(3);
		
		ASSERT_EQUAL(editor.GetText(), "world, hello");
	}
	{
		Editor editor;

		TypeText(editor, "misprnit");
		editor.Left();
		editor.Left();
		editor.Left();
		editor.Cut(1);
		editor.Right();
		editor.Paste();

		ASSERT_EQUAL(editor.GetText(), "misprint");
	}
}

void TestReverse() {
	Editor editor;

	const string text = "esreveR";
	for (char c : text) {
		editor.Insert(c);
		editor.Left();
	}

	ASSERT_EQUAL(editor.GetText(), "Reverse");
}

void TestNoText() {
	Editor editor;
	ASSERT_EQUAL(editor.GetText(), "");

	editor.Left();
	editor.Left();
	editor.Right();
	editor.Right();
	editor.Copy(0);
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "");
}

void TestEmptyBuffer() {
	Editor editor;

	editor.Paste();
	TypeText(editor, "example");
	editor.Left();
	editor.Left();
	editor.Paste();
	editor.Right();
	editor.Paste();
	editor.Copy(0);
	editor.Paste();
	editor.Left();
	editor.Cut(0);
	editor.Paste();

	ASSERT_EQUAL(editor.GetText(), "example");
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestEditing);
	RUN_TEST(tr, TestReverse);
	RUN_TEST(tr, TestNoText);
	RUN_TEST(tr, TestEmptyBuffer);
	return 0;
}