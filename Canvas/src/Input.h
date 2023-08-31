#include "Core.h"

#include <map>
#include <functional>

enum class Action
{
	Release = 0,
	Press,
	Repeat
};

enum class MouseButton
{
	Left = 0,
	Right = 1,
	Middle = 2,
	M4 = 3,
	M5 = 4,
	M6 = 5,
	M7 = 6,
	M8 = 7,
};

enum class Key
{
	/* Printable Keys */
	Space = 32,
	Apostrophe = 39,
	Comma = 44,
	Minus = 45,
	Period = 46,
	Slash = 47,
	Num0 = 48,
	Num1 = 49,
	Num2 = 50,
	Num3 = 51,
	Num4 = 52,
	Num5 = 53,
	Num6 = 54,
	Num7 = 55,
	Num8 = 56,
	Num9 = 57,
	Semicolon = 59,
	Equal = 61,
	A = 65,
	B = 66,
	C = 67,
	D = 68,
	E = 69,
	F = 70,
	G = 71,
	H = 72,
	I = 73,
	J = 74,
	K = 75,
	L = 76,
	M = 77,
	N = 78,
	O = 79,
	P = 80,
	Q = 81,
	R = 82,
	S = 83,
	T = 84,
	U = 85,
	V = 86,
	W = 87,
	X = 88,
	Y = 89,
	Z = 90,
	LeftBracket = 91,
	Backslash = 92,
	RightBracket = 93,
	GraveAccent = 96,

	/* Function Keys */
	Key_Keypad_Subtract = 333,
	Escape = 256,
	Enter = 257,
	Tab = 258,
	Backspace = 259,
	Insert = 260,
	Delete = 261,
	Right = 262,
	Left = 263,
	Down = 264,
	Up = 265,
	PageUp = 266,
	PageDown = 267,
	Home = 268,
	End = 269,
	CapsLock = 280,
	ScrollLock = 281,
	NumLock = 282,
	PrintScreen = 283,
	Pause = 284,
	F1 = 290,
	F2 = 291,
	F3 = 292,
	F4 = 293,
	F5 = 294,
	F6 = 295,
	F7 = 296,
	F8 = 297,
	F9 = 298,
	F10 = 299,
	F11 = 300,
	F12 = 301,
	F13 = 302,
	F14 = 303,
	F15 = 304,
	F16 = 305,
	F17 = 306,
	F18 = 307,
	F19 = 308,
	F20 = 309,
	F21 = 310,
	F22 = 311,
	F23 = 312,
	F24 = 313,
	F25 = 314,
	Keypad_0 = 320,
	Keypad_1 = 321,
	Keypad_2 = 322,
	Keypad_3 = 323,
	Keypad_4 = 324,
	Keypad_5 = 325,
	Keypad_6 = 326,
	Keypad_7 = 327,
	Keypad_8 = 328,
	Keypad_9 = 329,
	Keypad_Decimal = 330,
	Keypad_Divide = 331,
	Keypad_Multiply = 332,
	Keypad_Add = 334,
	Keypad_Enter = 335,
	Keypad_Equal = 336,
	LeftShift = 340,
	LeftControl = 341,
	LeftAlt = 342,
	LeftSuper = 343,
	RightShift = 344,
	RightControl = 345,
	RightAlt = 346,
	RightSuper = 347,
	Menu = 348
};

class CANVAS_API Input
{
public:
	static bool IsKeyPressed(Key key);
	static bool IsMouseButtonPressed(MouseButton button);
	static bool IsKeyReleased(Key key);
	static bool IsMouseButtonReleased(MouseButton button);
	static void SetKeyCallback(Key key, Action action, std::function<void()> func);
	static void SetMouseButtonCallback(MouseButton button, Action action, std::function<void()> func);
};
