#include <iostream>   // std::cout, std::cin
#include <string>     // std::string
#include <vector>     // std::vector
#include <cstdlib>    // std::rand(), std::srand()
#include <ctime>      // std::time()

// WIDTH and HEIGHT control the dimensions of the printed ASCII art.
const int WIDTH = 60;
const int HEIGHT = 18;

// generate_snow(count)
// Returns a std::vector (dynamic array) containing 'count' pairs of ints.
// Each std::pair<int,int> stores an (x, y) coordinate for a snowflake.
// 
// The syntax { x, y } uses uniform initialization to construct the pair.
// snow.reserve(count) lets the vector allocate enough memory once to
// avoid multiple resizes (an optimization).
std::vector<std::pair<int, int>> generate_snow(int count) {

    std::vector<std::pair<int, int>> snow;

    snow.reserve(count);

	// Generate 'count' random (x, y) coordinates
    for (int i = 0; i < count; ++i) {

        int x = std::rand() % WIDTH;   // '%' is modulo → produces range 0..WIDTH-1
		int y = std::rand() % HEIGHT;  // produces range 0..HEIGHT-1

        snow.push_back({ x, y });      // add a new pair into the vector
    }
    return snow;                       // return by value (C++ optimizes this heavily)
}

// print_greeting(...)
// Prints the boxed greeting card. Uses a lambda to center text.
//
// A lambda is an inline, anonymous function:
//   auto center = [](const string& text) { ... };
//
// static_cast<int>(text.size()) ensures size_t → int conversion is explicit.
void print_greeting(const std::string& recipient,
    const std::string& message,
    const std::string& sender,
    const std::string& year)
{
    using std::cout;
    using std::string;

    // Lambda to center and pad text to WIDTH
	// This ensures the box border is consistent, no matter the text length.
    auto center_pad = [](const string& text) {
		// Calculate left padding
        int padding = (WIDTH - static_cast<int>(text.size())) / 2;
		// Ensure padding is not negative
        if (padding < 0) padding = 0;
		// Create left padding
        string centered = string(padding, ' ') + text;
        // Pad right side to ensure total length is WIDTH
        centered += string(WIDTH - static_cast<int>(centered.size()), ' ');
        return centered;
        };

	// Apply lambda to print the greeting box
    cout << "\n";
    cout << "+" << std::string(WIDTH + 2, '-') << "+\n";
    cout << "| " << center_pad("MERRY CHRISTMAS " + year) << " |\n";
    cout << "| " << std::string(WIDTH, ' ') << " |\n";

    if (!recipient.empty())
        cout << "| " << center_pad("To: " + recipient) << " |\n";

    if (!message.empty())
        cout << "| " << center_pad(message) << " |\n";
    else
        cout << "| " << center_pad("Wishing you a warm, cozy Christmas.") << " |\n";

    if (!sender.empty())
        cout << "| " << center_pad("From: " + sender) << " |\n";

    cout << "| " << std::string(WIDTH, ' ') << " |\n";
    cout << "+" << std::string(WIDTH + 2, '-') << "+\n";
}


// draw_scene(...)
// Draws HEIGHT lines of ASCII art. Each line starts with:
//     std::string line(WIDTH, ' ');
//
// This constructs a string containing WIDTH copies of the space character.
// 
// line.replace(start, length, "text")
// overwrites a substring of 'line' starting at index 'start' and spanning
// 'length' characters, replacing it with the characters in the string literal.
//
// For example:
//    line.replace(4, 11, "  | [] |   ");
//
// The backslash in the ASCII art must be escaped as "\\", because "\" is
// normally used to introduce escape sequences inside strings.
void draw_scene(const std::vector<std::pair<int, int>>& snow) {
    using std::cout;
    using std::string;

    for (int row = 0; row < HEIGHT; row++) {

        // Creates a line like "                    " (60 spaces).
        std::string line(WIDTH, ' ');

        // SNOW LAYER
        // Each snowflake is drawn as a '.' IF that
        // position is empty (to avoid overwriting art).
        for (auto& flake : snow) {

            // flake.second is the y-coordinate
            if (flake.second == row) {

                // flake.first is the x-coordinate
                if (line[flake.first] == ' ') {
                    line[flake.first] = '.';
                }
            }
        }

        // CHURCH WITH STEEPLE
        // Every ASCII line is exactly 11 characters.
        // They are drawn starting at column 4.
        if (row == 6)  line.replace(4, 11, "    ++     "); // cross at top
        if (row == 7)  line.replace(4, 11, "    ||     "); // steeple shaft
        if (row == 8)  line.replace(4, 11, "   /  \\    "); // /\ using escaped backslash
        if (row == 9)  line.replace(4, 11, "  /____\\   "); // roof
        if (row == 10) line.replace(4, 11, "  | [] |   "); // windows
        if (row == 11) line.replace(4, 11, "  | [] |   "); // repeat row
        if (row == 12) line.replace(4, 11, "  | __ |   "); // door
        if (row == 13) line.replace(4, 11, "  |____|   "); // base

        // TREE ON THE RIGHT
        // Uses 10-character-wide replacement blocks.
        if (row == 8)  line.replace(40, 10, "    *     ");
        if (row == 9)  line.replace(40, 10, "   /_\\    ");   // escaped backslash
        if (row == 10) line.replace(40, 10, "  /_/_\\   ");
        if (row == 11) line.replace(40, 10, " /_/_/_\\  ");
        if (row == 12) line.replace(40, 10, "/_/_/_/_\\ ");
        if (row == 13) line.replace(40, 10, "   /_\\    ");
        if (row == 14) line.replace(40, 10, "   /_\\    ");

        cout << line << "\n";
    }
}

// main()
// Seeds the random number generator using current time.
// Reads each input line using std::getline (important because getline
//   can handle spaces, unlike operator>>).
// First prints the greeting card, then the scene.
int main() {

    // std::srand seeds the random engine. std::time(nullptr)
    // returns the current time as a number → good for randomness.
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // User inputs
    std::string recipient, sender, message, year = "2025";

    // getline reads an entire line up to '\n'. Unlike >>, it won't stop at spaces.
    std::cout << "Recipient name: ";
    std::getline(std::cin, recipient);

    std::cout << "Sender name: ";
    std::getline(std::cin, sender);

    std::cout << "Custom message: ";
    std::getline(std::cin, message);

    std::cout << "Year [" << year << "]: ";
    std::string y;
    std::getline(std::cin, y);

    if (!y.empty()) year = y;  // override default if user typed something

    // First print greeting card at the top
    print_greeting(recipient, message, sender, year);

    // Generate snowflake coordinates (85 chosen for good coverage)
    auto snow = generate_snow(85);

    // Draw landscape scene under greeting
    draw_scene(snow);

    return 0;  // return 0 indicates successful program exit
}