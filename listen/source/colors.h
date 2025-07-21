#pragma once
#include <array>
#include <ostream>

#include <fmt/format.h>
#include <fmt/ostream.h>

// Define some useful colors.
// Based on https://jonasjacek.github.io/colors/
// I only exported the ones w/ unique names.
namespace Colors {

// Wrapper so we can overload ostream operator.
struct Color {
  constexpr explicit Color(int code) : code(code) {}
  const int code;
};

static constexpr Color None{-1};
static constexpr Color Black{0};
static constexpr Color Maroon{1};
static constexpr Color Green{2};
static constexpr Color Olive{3};
static constexpr Color Navy{4};
static constexpr Color Purple{5};
static constexpr Color Teal{6};
static constexpr Color Silver{7};
static constexpr Color Grey{8};
static constexpr Color Red{9};
static constexpr Color Lime{10};
static constexpr Color Yellow{11};
static constexpr Color Blue{12};
static constexpr Color Fuchsia{13};
static constexpr Color Aqua{14};
static constexpr Color White{15};
static constexpr Color Grey0{16};
static constexpr Color NavyBlue{17};
static constexpr Color DarkBlue{18};
static constexpr Color Blue3{19};
static constexpr Color Blue1{21};
static constexpr Color DarkGreen{22};
static constexpr Color DeepSkyBlue4{23};
static constexpr Color DodgerBlue3{26};
static constexpr Color DodgerBlue2{27};
static constexpr Color Green4{28};
static constexpr Color SpringGreen4{29};
static constexpr Color Turquoise4{30};
static constexpr Color DeepSkyBlue3{31};
static constexpr Color DodgerBlue1{33};
static constexpr Color Green3{34};
static constexpr Color SpringGreen3{35};
static constexpr Color DarkCyan{36};
static constexpr Color LightSeaGreen{37};
static constexpr Color DeepSkyBlue2{38};
static constexpr Color DeepSkyBlue1{39};
static constexpr Color SpringGreen2{42};
static constexpr Color Cyan3{43};
static constexpr Color DarkTurquoise{44};
static constexpr Color Turquoise2{45};
static constexpr Color Green1{46};
static constexpr Color SpringGreen1{48};
static constexpr Color MediumSpringGreen{49};
static constexpr Color Cyan2{50};
static constexpr Color Cyan1{51};
static constexpr Color DarkRed{52};
static constexpr Color DeepPink4{53};
static constexpr Color Purple4{54};
static constexpr Color Purple3{56};
static constexpr Color BlueViolet{57};
static constexpr Color Orange4{58};
static constexpr Color Grey37{59};
static constexpr Color MediumPurple4{60};
static constexpr Color SlateBlue3{61};
static constexpr Color RoyalBlue1{63};
static constexpr Color Chartreuse4{64};
static constexpr Color DarkSeaGreen4{65};
static constexpr Color PaleTurquoise4{66};
static constexpr Color SteelBlue{67};
static constexpr Color SteelBlue3{68};
static constexpr Color CornflowerBlue{69};
static constexpr Color Chartreuse3{70};
static constexpr Color CadetBlue{72};
static constexpr Color SkyBlue3{74};
static constexpr Color SteelBlue1{75};
static constexpr Color PaleGreen3{77};
static constexpr Color SeaGreen3{78};
static constexpr Color Aquamarine3{79};
static constexpr Color MediumTurquoise{80};
static constexpr Color Chartreuse2{82};
static constexpr Color SeaGreen2{83};
static constexpr Color SeaGreen1{84};
static constexpr Color Aquamarine1{86};
static constexpr Color DarkSlateGray2{87};
static constexpr Color DarkMagenta{90};
static constexpr Color DarkViolet{92};
static constexpr Color LightPink4{95};
static constexpr Color Plum4{96};
static constexpr Color MediumPurple3{97};
static constexpr Color SlateBlue1{99};
static constexpr Color Yellow4{100};
static constexpr Color Wheat4{101};
static constexpr Color Grey53{102};
static constexpr Color LightSlateGrey{103};
static constexpr Color MediumPurple{104};
static constexpr Color LightSlateBlue{105};
static constexpr Color DarkOliveGreen3{107};
static constexpr Color DarkSeaGreen{108};
static constexpr Color LightSkyBlue3{109};
static constexpr Color SkyBlue2{111};
static constexpr Color DarkSeaGreen3{115};
static constexpr Color DarkSlateGray3{116};
static constexpr Color SkyBlue1{117};
static constexpr Color Chartreuse1{118};
static constexpr Color LightGreen{119};
static constexpr Color PaleGreen1{121};
static constexpr Color DarkSlateGray1{123};
static constexpr Color Red3{124};
static constexpr Color MediumVioletRed{126};
static constexpr Color Magenta3{127};
static constexpr Color DarkOrange3{130};
static constexpr Color IndianRed{131};
static constexpr Color HotPink3{132};
static constexpr Color MediumOrchid3{133};
static constexpr Color MediumOrchid{134};
static constexpr Color MediumPurple2{135};
static constexpr Color DarkGoldenrod{136};
static constexpr Color LightSalmon3{137};
static constexpr Color RosyBrown{138};
static constexpr Color Grey63{139};
static constexpr Color MediumPurple1{141};
static constexpr Color Gold3{142};
static constexpr Color DarkKhaki{143};
static constexpr Color NavajoWhite3{144};
static constexpr Color Grey69{145};
static constexpr Color LightSteelBlue3{146};
static constexpr Color LightSteelBlue{147};
static constexpr Color Yellow3{148};
static constexpr Color DarkSeaGreen2{151};
static constexpr Color LightCyan3{152};
static constexpr Color LightSkyBlue1{153};
static constexpr Color GreenYellow{154};
static constexpr Color DarkOliveGreen2{155};
static constexpr Color DarkSeaGreen1{158};
static constexpr Color PaleTurquoise1{159};
static constexpr Color DeepPink3{161};
static constexpr Color Magenta2{165};
static constexpr Color HotPink2{169};
static constexpr Color Orchid{170};
static constexpr Color MediumOrchid1{171};
static constexpr Color Orange3{172};
static constexpr Color LightPink3{174};
static constexpr Color Pink3{175};
static constexpr Color Plum3{176};
static constexpr Color Violet{177};
static constexpr Color LightGoldenrod3{179};
static constexpr Color Tan{180};
static constexpr Color MistyRose3{181};
static constexpr Color Thistle3{182};
static constexpr Color Plum2{183};
static constexpr Color Khaki3{185};
static constexpr Color LightGoldenrod2{186};
static constexpr Color LightYellow3{187};
static constexpr Color Grey84{188};
static constexpr Color LightSteelBlue1{189};
static constexpr Color Yellow2{190};
static constexpr Color DarkOliveGreen1{191};
static constexpr Color Honeydew2{194};
static constexpr Color LightCyan1{195};
static constexpr Color Red1{196};
static constexpr Color DeepPink2{197};
static constexpr Color DeepPink1{198};
static constexpr Color Magenta1{201};
static constexpr Color OrangeRed1{202};
static constexpr Color IndianRed1{203};
static constexpr Color HotPink{205};
static constexpr Color DarkOrange{208};
static constexpr Color Salmon1{209};
static constexpr Color LightCoral{210};
static constexpr Color PaleVioletRed1{211};
static constexpr Color Orchid2{212};
static constexpr Color Orchid1{213};
static constexpr Color Orange1{214};
static constexpr Color SandyBrown{215};
static constexpr Color LightSalmon1{216};
static constexpr Color LightPink1{217};
static constexpr Color Pink1{218};
static constexpr Color Plum1{219};
static constexpr Color Gold1{220};
static constexpr Color NavajoWhite1{223};
static constexpr Color MistyRose1{224};
static constexpr Color Thistle1{225};
static constexpr Color Yellow1{226};
static constexpr Color LightGoldenrod1{227};
static constexpr Color Khaki1{228};
static constexpr Color Wheat1{229};
static constexpr Color Cornsilk1{230};
static constexpr Color Grey100{231};
static constexpr Color Grey3{232};
static constexpr Color Grey7{233};
static constexpr Color Grey11{234};
static constexpr Color Grey15{235};
static constexpr Color Grey19{236};
static constexpr Color Grey23{237};
static constexpr Color Grey27{238};
static constexpr Color Grey30{239};
static constexpr Color Grey35{240};
static constexpr Color Grey39{241};
static constexpr Color Grey42{242};
static constexpr Color Grey46{243};
static constexpr Color Grey50{244};
static constexpr Color Grey54{245};
static constexpr Color Grey58{246};
static constexpr Color Grey62{247};
static constexpr Color Grey66{248};
static constexpr Color Grey70{249};
static constexpr Color Grey74{250};
static constexpr Color Grey78{251};
static constexpr Color Grey82{252};
static constexpr Color Grey85{253};
static constexpr Color Grey89{254};
static constexpr Color Grey93{255};

// Pair together human-readable name w/ color struct.
using NameAndColor = std::pair<const char *, Color>;

// Colors w/ their respective names as strings, ordered alphabetically.
static constexpr std::array<NameAndColor, 202> AllColors = {{
    {"Aqua", Aqua},
    {"Aquamarine1", Aquamarine1},
    {"Aquamarine3", Aquamarine3},
    {"Black", Black},
    {"Blue", Blue},
    {"Blue1", Blue1},
    {"Blue3", Blue3},
    {"BlueViolet", BlueViolet},
    {"CadetBlue", CadetBlue},
    {"Chartreuse1", Chartreuse1},
    {"Chartreuse2", Chartreuse2},
    {"Chartreuse3", Chartreuse3},
    {"Chartreuse4", Chartreuse4},
    {"CornflowerBlue", CornflowerBlue},
    {"Cornsilk1", Cornsilk1},
    {"Cyan1", Cyan1},
    {"Cyan2", Cyan2},
    {"Cyan3", Cyan3},
    {"DarkBlue", DarkBlue},
    {"DarkCyan", DarkCyan},
    {"DarkGoldenrod", DarkGoldenrod},
    {"DarkGreen", DarkGreen},
    {"DarkKhaki", DarkKhaki},
    {"DarkMagenta", DarkMagenta},
    {"DarkOliveGreen1", DarkOliveGreen1},
    {"DarkOliveGreen2", DarkOliveGreen2},
    {"DarkOliveGreen3", DarkOliveGreen3},
    {"DarkOrange", DarkOrange},
    {"DarkOrange3", DarkOrange3},
    {"DarkRed", DarkRed},
    {"DarkSeaGreen", DarkSeaGreen},
    {"DarkSeaGreen1", DarkSeaGreen1},
    {"DarkSeaGreen2", DarkSeaGreen2},
    {"DarkSeaGreen3", DarkSeaGreen3},
    {"DarkSeaGreen4", DarkSeaGreen4},
    {"DarkSlateGray1", DarkSlateGray1},
    {"DarkSlateGray2", DarkSlateGray2},
    {"DarkSlateGray3", DarkSlateGray3},
    {"DarkTurquoise", DarkTurquoise},
    {"DarkViolet", DarkViolet},
    {"DeepPink1", DeepPink1},
    {"DeepPink2", DeepPink2},
    {"DeepPink3", DeepPink3},
    {"DeepPink4", DeepPink4},
    {"DeepSkyBlue1", DeepSkyBlue1},
    {"DeepSkyBlue2", DeepSkyBlue2},
    {"DeepSkyBlue3", DeepSkyBlue3},
    {"DeepSkyBlue4", DeepSkyBlue4},
    {"DodgerBlue1", DodgerBlue1},
    {"DodgerBlue2", DodgerBlue2},
    {"DodgerBlue3", DodgerBlue3},
    {"Fuchsia", Fuchsia},
    {"Gold1", Gold1},
    {"Gold3", Gold3},
    {"Green", Green},
    {"Green1", Green1},
    {"Green3", Green3},
    {"Green4", Green4},
    {"GreenYellow", GreenYellow},
    {"Grey", Grey},
    {"Grey0", Grey0},
    {"Grey100", Grey100},
    {"Grey11", Grey11},
    {"Grey15", Grey15},
    {"Grey19", Grey19},
    {"Grey23", Grey23},
    {"Grey27", Grey27},
    {"Grey3", Grey3},
    {"Grey30", Grey30},
    {"Grey35", Grey35},
    {"Grey37", Grey37},
    {"Grey39", Grey39},
    {"Grey42", Grey42},
    {"Grey46", Grey46},
    {"Grey50", Grey50},
    {"Grey53", Grey53},
    {"Grey54", Grey54},
    {"Grey58", Grey58},
    {"Grey62", Grey62},
    {"Grey63", Grey63},
    {"Grey66", Grey66},
    {"Grey69", Grey69},
    {"Grey7", Grey7},
    {"Grey70", Grey70},
    {"Grey74", Grey74},
    {"Grey78", Grey78},
    {"Grey82", Grey82},
    {"Grey84", Grey84},
    {"Grey85", Grey85},
    {"Grey89", Grey89},
    {"Grey93", Grey93},
    {"Honeydew2", Honeydew2},
    {"HotPink", HotPink},
    {"HotPink2", HotPink2},
    {"HotPink3", HotPink3},
    {"IndianRed", IndianRed},
    {"IndianRed1", IndianRed1},
    {"Khaki1", Khaki1},
    {"Khaki3", Khaki3},
    {"LightCoral", LightCoral},
    {"LightCyan1", LightCyan1},
    {"LightCyan3", LightCyan3},
    {"LightGoldenrod1", LightGoldenrod1},
    {"LightGoldenrod2", LightGoldenrod2},
    {"LightGoldenrod3", LightGoldenrod3},
    {"LightGreen", LightGreen},
    {"LightPink1", LightPink1},
    {"LightPink3", LightPink3},
    {"LightPink4", LightPink4},
    {"LightSalmon1", LightSalmon1},
    {"LightSalmon3", LightSalmon3},
    {"LightSeaGreen", LightSeaGreen},
    {"LightSkyBlue1", LightSkyBlue1},
    {"LightSkyBlue3", LightSkyBlue3},
    {"LightSlateBlue", LightSlateBlue},
    {"LightSlateGrey", LightSlateGrey},
    {"LightSteelBlue", LightSteelBlue},
    {"LightSteelBlue1", LightSteelBlue1},
    {"LightSteelBlue3", LightSteelBlue3},
    {"LightYellow3", LightYellow3},
    {"Lime", Lime},
    {"Magenta1", Magenta1},
    {"Magenta2", Magenta2},
    {"Magenta3", Magenta3},
    {"Maroon", Maroon},
    {"MediumOrchid", MediumOrchid},
    {"MediumOrchid1", MediumOrchid1},
    {"MediumOrchid3", MediumOrchid3},
    {"MediumPurple", MediumPurple},
    {"MediumPurple1", MediumPurple1},
    {"MediumPurple2", MediumPurple2},
    {"MediumPurple3", MediumPurple3},
    {"MediumPurple4", MediumPurple4},
    {"MediumSpringGreen", MediumSpringGreen},
    {"MediumTurquoise", MediumTurquoise},
    {"MediumVioletRed", MediumVioletRed},
    {"MistyRose1", MistyRose1},
    {"MistyRose3", MistyRose3},
    {"NavajoWhite1", NavajoWhite1},
    {"NavajoWhite3", NavajoWhite3},
    {"Navy", Navy},
    {"NavyBlue", NavyBlue},
    {"Olive", Olive},
    {"Orange1", Orange1},
    {"Orange3", Orange3},
    {"Orange4", Orange4},
    {"OrangeRed1", OrangeRed1},
    {"Orchid", Orchid},
    {"Orchid1", Orchid1},
    {"Orchid2", Orchid2},
    {"PaleGreen1", PaleGreen1},
    {"PaleGreen3", PaleGreen3},
    {"PaleTurquoise1", PaleTurquoise1},
    {"PaleTurquoise4", PaleTurquoise4},
    {"PaleVioletRed1", PaleVioletRed1},
    {"Pink1", Pink1},
    {"Pink3", Pink3},
    {"Plum1", Plum1},
    {"Plum2", Plum2},
    {"Plum3", Plum3},
    {"Plum4", Plum4},
    {"Purple", Purple},
    {"Purple3", Purple3},
    {"Purple4", Purple4},
    {"Red", Red},
    {"Red1", Red1},
    {"Red3", Red3},
    {"RosyBrown", RosyBrown},
    {"RoyalBlue1", RoyalBlue1},
    {"Salmon1", Salmon1},
    {"SandyBrown", SandyBrown},
    {"SeaGreen1", SeaGreen1},
    {"SeaGreen2", SeaGreen2},
    {"SeaGreen3", SeaGreen3},
    {"Silver", Silver},
    {"SkyBlue1", SkyBlue1},
    {"SkyBlue2", SkyBlue2},
    {"SkyBlue3", SkyBlue3},
    {"SlateBlue1", SlateBlue1},
    {"SlateBlue3", SlateBlue3},
    {"SpringGreen1", SpringGreen1},
    {"SpringGreen2", SpringGreen2},
    {"SpringGreen3", SpringGreen3},
    {"SpringGreen4", SpringGreen4},
    {"SteelBlue", SteelBlue},
    {"SteelBlue1", SteelBlue1},
    {"SteelBlue3", SteelBlue3},
    {"Tan", Tan},
    {"Teal", Teal},
    {"Thistle1", Thistle1},
    {"Thistle3", Thistle3},
    {"Turquoise2", Turquoise2},
    {"Turquoise4", Turquoise4},
    {"Violet", Violet},
    {"Wheat1", Wheat1},
    {"Wheat4", Wheat4},
    {"White", White},
    {"Yellow", Yellow},
    {"Yellow1", Yellow1},
    {"Yellow2", Yellow2},
    {"Yellow3", Yellow3},
    {"Yellow4", Yellow4},
}};

// Define ostream so we can print it.
inline std::ostream &operator<<(std::ostream &stream, const Color &c) {
  if (c.code >= 0) {
    stream << "\u001b[38;5;";
    stream << c.code;
    stream << "m";
  } else {
    // terminate color:
    stream << "\u001b[0m";
  }
  return stream;
}

} // namespace Colors

// Allow formatting of Colors
template <> struct fmt::formatter<Colors::Color> {
  constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Colors::Color c, FormatContext &ctx) const
      -> decltype(ctx.out()) {
    return fmt::format_to(ctx.out(), "{}", fmt::streamed(c));
  }
};
