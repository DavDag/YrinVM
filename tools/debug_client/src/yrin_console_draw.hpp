#pragma once

#ifndef YRINVM_YRIN_CONSOLE_DRAW_HPP
#define YRINVM_YRIN_CONSOLE_DRAW_HPP

/*
Table showing how ASCII characters are drawn on the screen
0 |     32 |    64 | @  96 | `  128 | Ç 160 | á 192 | └ 224 | Ó
1 | ☺   33 | !  65 | A  97 | a  129 | ü 161 | í 193 | ┴ 225 | ß
2 | ☻   34 | "  66 | B  98 | b  130 | é 162 | ó 194 | ┬ 226 | Ô
3 | ♥   35 | #  67 | C  99 | c  131 | â 163 | ú 195 | ├ 227 | Ò
4 | ♦   36 | $  68 | D  100 | d 132 | ä 164 | ñ 196 | ─ 228 | õ
5 | ♣   37 | %  69 | E  101 | e 133 | à 165 | Ñ 197 | ┼ 229 | Õ
6 | ♠   38 | &  70 | F  102 | f 134 | å 166 | ª 198 | ã 230 | µ
7 | •   39 | '  71 | G  103 | g 135 | ç 167 | º 199 | Ã 231 | þ
8 | ◘   40 | (  72 | H  104 | h 136 | ê 168 | ¿ 200 | ╚ 232 | Þ
9 | ○   41 | )  73 | I  105 | i 137 | ë 169 | ® 201 | ╔ 233 | Ú
10 | ◙	42 | *  74 | J  106 | j 138 | è 170 | ¬ 202 | ╩ 234 | Û
11 | ♂  43 | +  75 | K  107 | k 139 | ï 171 | ½ 203 | ╦ 235 | Ù
12 | ♀  44 | ,  76 | L  108 | l 140 | î 172 | ¼ 204 | ╠ 236 | ý
13 | ♪	45 | -  77 | M  109 | m 141 | ì 173 | ¡ 205 | ═ 237 | Ý
14 | ♫  46 | .  78 | N  110 | n 142 | Ä 174 | « 206 | ╬ 238 | ¯
15 | ☼  47 | /  79 | O  111 | o 143 | Å 175 | » 207 | ¤ 239 | ´
16 | ►  48 | 0  80 | P  112 | p 144 | É 176 | ░ 208 | ð 240 | ­
17 | ◄  49 | 1  81 | Q  113 | q 145 | æ 177 | ▒ 209 | Ð 241 | ±
18 | ↕  50 | 2  82 | R  114 | r 146 | Æ 178 | ▓ 210 | Ê 242 | ‗
19 | ‼  51 | 3  83 | S  115 | s 147 | ô 179 | │ 211 | Ë 243 | ¾
20 | ¶  52 | 4  84 | T  116 | t 148 | ö 180 | ┤ 212 | È 244 | ¶
21 | §  53 | 5  85 | U  117 | u 149 | ò 181 | Á 213 | ı 245 | §
22 | ▬  54 | 6  86 | V  118 | v 150 | û 182 | Â 214 | Í 246 | ÷
23 | ↨  55 | 7  87 | W  119 | w 151 | ù 183 | À 215 | Î 247 | ¸
24 | ↑  56 | 8  88 | X  120 | x 152 | ÿ 184 | © 216 | Ï 248 | °
25 | ↓  57 | 9  89 | Y  121 | y 153 | Ö 185 | ╣ 217 | ┘ 249 | ¨
26 | →  58 | :  90 | Z  122 | z 154 | Ü 186 | ║ 218 | ┌ 250 | ·
27 | ←  59 | ;  91 | [  123 | { 155 | ø 187 | ╗ 219 | █ 251 | ¹
28 | ∟  60 | <  92 | \  124 | | 156 | £ 188 | ╝ 220 | ▄ 252 | ³
29 | ↔  61 | =  93 | ]  125 | } 157 | Ø 189 | ¢ 221 | ¦ 253 | ²
30 | ▲  62 | >  94 | ^  126 | ~ 158 | × 190 | ¥ 222 | Ì 254 | ■
31 | ▼  63 | ?  95 | _  127 | ⌂ 159 | ƒ 191 | ┐ 223 | ▀ 255 |
*/

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

/*
 * Reference
 * https://github.com/DavDag/CppConsoleGameLibrary
 */

namespace Yrin::Console {

    enum FrameType : unsigned short int { DOUBLE = 0, SINGLE = 1 };

    class ConsoleTool {
    private:
        HANDLE consoleOutput;
        COORD bufferSize{}, bufferCoord{};
        SMALL_RECT region{};
        CHAR_INFO buffer{}
        [SCREEN_HEIGHT][SCREEN_WIDTH];

    public:
        ConsoleTool() {
            consoleOutput = (HANDLE) GetStdHandle(STD_OUTPUT_HANDLE);
            bufferSize = {SCREEN_WIDTH, SCREEN_HEIGHT};
            bufferCoord = {0, 0};
            region = {0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1};

            ReadConsoleOutputA(consoleOutput, (CHAR_INFO *) buffer, bufferSize, bufferCoord, &region);
        }

        void FillCell(SHORT x, SHORT y, CHAR c, WORD attr) {
            buffer[y][x].Attributes = attr;
            buffer[y][x].Char.AsciiChar = c;
        }

        void DrawBuffer() {
            WriteConsoleOutputA(consoleOutput, (CHAR_INFO *) buffer, bufferSize, bufferCoord, &region);
        }

        static void DrawBufferRegion(SHORT x, SHORT y, SHORT columns, SHORT rows) {
            SMALL_RECT region = {x, y, x + columns - 1, y + rows - 1};
            WriteConsoleOutputA(consoleOutput, (CHAR_INFO *) buffer, bufferSize, {x, y}, &region);
        }

        static void FillBuffer(CHAR c, WORD attr) {
            CHAR_INFO ci;
            ci.Attributes = attr;
            ci.Char.AsciiChar = c;

            for (unsigned short y = 0; y < SCREEN_HEIGHT; ++y)
                for (unsigned short x = 0; x < SCREEN_WIDTH; ++x)
                    buffer[y][x] = ci;
        }

        void DrawFrame(int x, int y, int w, int h, FrameType type, unsigned char attr) {
            --w;
            --h;
            static const char c[2][6] = {{(char) 201, (char) 187, (char) 200, (char) 188, (char) 205, (char) 186},
                                         {(char) 218, (char) 191, (char) 192, (char) 217, (char) 196, (char) 179}};

            // Corners
            FillCell(x, y, c[type][0], attr); // '╔' | '┌'
            FillCell(x + w, y, c[type][1], attr); // '╗' | '┐'
            FillCell(x, y + h, c[type][2], attr); // '╚' | '└'
            FillCell(x + w, y + h, c[type][3], attr); // '╝' | '┘'
            for (int i = x + 1; i < x + w; ++i) {
                FillCell(i, y, c[type][4], attr); // '═' | '─'
                FillCell(i, y + h, c[type][4], attr); // '═' | '─'
            }
            for (int i = y + 1; i < y + h; ++i) {
                FillCell(x, i, c[type][5], attr); // '║' | '│'
                FillCell(x + w, i, c[type][5], attr); // '║' | '│'
            }
        }

        void DrawWords(int x, int y, const char *text, size_t n, unsigned char attr) {
            if (n == NULL) n = strlen(text);

            for (int i = 0; i < n; ++i)
                FillCell(x + i, y, text[i], attr);
        }

        static void SetTitle(const char *title) {
            SetConsoleTitle(title);
        }

        void SetWindowSize(SHORT width, SHORT height, bool adjustBuffer) {
            SMALL_RECT rect = {0, 0, width, height};
            if (adjustBuffer) SetConsoleScreenBufferSize(consoleOutput, {width, height});
            SetConsoleWindowInfo(consoleOutput, TRUE, &rect);
        }

        void FillCell(SHORT x, SHORT y, CHAR c, WORD attr) {
            buffer[y][x].Attributes = attr;
            buffer[y][x].Char.AsciiChar = c;
        }

        void ShowConsoleCursor(BOOL visible) {
            CONSOLE_CURSOR_INFO cursorInfo;
            GetConsoleCursorInfo(consoleOutput, &cursorInfo);
            cursorInfo.bVisible = visible;
            SetConsoleCursorInfo(consoleOutput, &cursorInfo);
        }

        void Gotoxy(SHORT x, SHORT y) {
            SetConsoleCursorPosition(consoleOutput, {x, y});
        }
    };

} // Yrin::ConsoleDraw

#endif //YRINVM_YRIN_CONSOLE_DRAW_HPP
