#ifndef __CONSOLECOLORS__
#define __CONSOLECOLORS__
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
using namespace std;
bool textcolorprotect=true;
void clearConsole(); 
enum concol
{
    black=0,
    dark_blue=1,
    dark_green=2,
    dark_aqua,dark_cyan=3,
    dark_red=4,
    dark_purple=5,dark_pink=5,dark_magenta=5,
    dark_yellow=6,
    dark_white=7,
    gray=8,
    blue=9,
    green=10,
    aqua=11,cyan=11,
    red=12,
    purple=13,pink=13,magenta=13,
    yellow=14,
    white=15
};
 
inline void setcolor(concol textcolor,concol backcolor);
inline void setcolor(int textcolor,int backcolor);
int textcolor();/*returns current text color*/
int backcolor();/*returns current background color*/
 
#define std_con_out GetStdHandle(STD_OUTPUT_HANDLE)
 
//-----------------------------------------------------------------------------
 
int textcolor()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(std_con_out,&csbi);
    int a=csbi.wAttributes;
    return a%16;
}
 
int backcolor()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(std_con_out,&csbi);
    int a=csbi.wAttributes;
    return (a/16)%16;
}
 
inline void setcolor(concol textcol,concol backcol)
{setcolor(int(textcol),int(backcol));}
 
inline void setcolor(int textcol,int backcol)
{
    if(textcolorprotect)
    {if((textcol%16)==(backcol%16))textcol++;}
    textcol%=16;backcol%=16;
    unsigned short wAttributes= ((unsigned)backcol<<4)|(unsigned)textcol;
    SetConsoleTextAttribute(std_con_out, wAttributes);
}
 
#if defined(_INC_OSTREAM)||defined(_IOSTREAM_)
ostream& operator<<(ostream& os,concol c)
{os.flush();setcolor(c,backcolor());return os;}
#endif
 
#if defined(_INC_ISTREAM)||defined(_IOSTREAM_)
istream& operator>>(istream& is,concol c)
{cout.flush();setcolor(c,backcolor());return is;}
#endif
 
//only call it at beggining of program
//only if you are using a GUI (windows, qt, opengl...)
//not if you are  using a console app. 
void SetStdOutToNewConsole()
{
    // allocate a console for this app
    AllocConsole();
 
    // redirect unbuffered STDOUT to the console
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	int fileDescriptor = _open_osfhandle((intptr_t)consoleHandle, _O_TEXT );
    FILE *fp = _fdopen( fileDescriptor, "w" );
    *stdout = *fp;
    setvbuf( stdout, NULL, _IONBF, 0 );
 
    // give the console window a nicer title
    SetConsoleTitle(L"Debug Output");
 
    // give the console window a bigger buffer size
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(consoleHandle, &csbi) )
    {
        COORD bufferSize;
        bufferSize.X = csbi.dwSize.X;
        bufferSize.Y = 9999;
        SetConsoleScreenBufferSize(consoleHandle, bufferSize);
    }
    clearConsole(); 
}
void clearConsole() {
    COORD topLeft  = { 0, 0 };
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;
 
    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console,  FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
     
}
 
void setConsoleXY(int x, int y)
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
     
}

//setColor(concol.red,concol.blue);
//setConsoleXY(10,10);
//cout << "Bodies:"; 
//setConsoleXY(10,20); 
//cout << "10";


#endif