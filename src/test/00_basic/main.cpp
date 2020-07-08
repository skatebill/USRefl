#include <USRefl.h>

#include <iostream>

using namespace Ubpa::USRefl;
using namespace std;

struct [[size(8)]] Point {
	[[not_serialize]]
	float x;
	[[info("hello")]]
	float y;
	bool a{ true };
};

template<>
struct Type<Point> {
	static constexpr FieldList fields = {
		Field{"x", &Point::x, AttrList{ Attr{ "not_serialize", true } }},
		Field{"y", &Point::y, AttrList{ Attr{ "info", "hello" } }}
	};

	static constexpr AttrList attrs = {
		Attr{ "size", 8 }
	};
};

int main() {
	Point p{ 1,2 };

	Type<Point>::fields.ForEach([](auto&& field) {
		cout << field.name << endl;
		field.attrs.ForEach([](auto&& attr) {
			cout << "key   : " << attr.key << endl;
			if constexpr (!attr.is_value_empty)
				cout << "value : " << attr.value << endl;
		});
	});

	constexpr auto y_idx = Type<Point>::fields.Find("y");
	constexpr auto y_field = Type<Point>::fields.Get<y_idx>();
	static_assert(y_field.name == "y");

	constexpr bool containX = Type<Point>::fields.Contains("x");

	Type<Point>::attrs.ForEach([](auto&& attr) {
		cout << "key   : " << attr.key << endl;
		if constexpr (!attr.is_value_empty)
			cout << "value : " << attr.value << endl;
	});

	ForEachNonStaticFieldOf(p, [](auto&& field) {
		cout << field << endl;
	});
}