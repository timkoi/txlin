#ifndef TXLIN_H
#define TXLIN_H
#include <SDL.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <string>

#define HWND Uint32
#define DWORD unsigned long
#define HDC SDL_Renderer*
#define HPEN SDL_Renderer*
#define RGBQUAD Uint32
#define HFONT void*
#define HBRUSH SDL_Renderer*

#define TXLIN_VERSION "TXLin [Ver: 1.74a, Rev: 106, Date: 2014-04-26 00:00:00]"
#define TXLIN_VERSIONNUM 0x174a0

// compatibility defines
#define _TX_VERSION TXLIN_VERSION
#define _TX_VER TXLIN_VERSIONNUM

// debug define
#ifdef TXLIN_DEBUG
#define DBGOUT std::cerr << "[TXLin/DEBUG/" << __LINE__ << "] "
#else
#define DBGOUT std::stringstream()
#endif
#define DBGINT(var) DBGOUT << #var << " = " << var << std::endl
#define TXLIN_WARNING(text) std::cerr << "[TXLin/WARNING] " << text << std::endl
#define TXLIN_WARNINGNOTIMPLEMENTED() TXLIN_WARNING(std::string(__PRETTY_FUNCTION__) + " is not implemented yet.")

// colors
#define TX_BLACK RGB(0, 0, 0)   
#define TX_BLUE RGB(0, 0, 128)
#define TX_GREEN RGB(0, 128, 0)   
#define TX_CYAN RGB(0, 128, 128)   
#define TX_RED RGB(128, 0, 0)   
#define TX_MAGENTA RGB(128, 0, 128)  
#define TX_BROWN RGB(128, 128, 0)
#define TX_ORANGE RGB(255, 128, 0)  
#define TX_GRAY RGB(160, 160, 160)   
#define TX_DARKGRAY RGB(128, 128, 128)
#define TX_LIGHTGRAY RGB(192, 192, 192)
#define TX_LIGHTBLUE RGB(0, 0, 255)   
#define TX_LIGHTGREEN RGB(0, 255, 128)   
#define TX_LIGHTCYAN RGB(0, 255, 255)   
#define TX_LIGHTRED RGB(255, 0, 128)
#define TX_LIGHTMAGENTA RGB(255, 0, 255)
#define TX_PINK RGB(255, 128, 255)
#define TX_YELLOW RGB(255, 255, 128)   
#define TX_WHITE RGB(255, 255, 255) 

#define TX_GREY TX_GRAY
#define TX_DARKGREY TX_DARKGRAY
#define TX_LIGHTGREY TX_LIGHTGRAY

#define TX_TRANSPARENT RGB(-1, -1, -1)

// dummy defines that are needed for API compatibility
#define FLOODFILLBORDER 0x0001
#define FLOODFILLSURFACE 0x0002
#define TA_LEFT 3
#define TA_RIGHT 4
#define TA_CENTER 5

#define TXLIN_TEXTSET_MAXWIDTH 6
#define TXLIN_TEXTSET_HALFWIDTH (TXLIN_TEXTSET_MAXWIDTH / 2)
#define TXLIN_TEXTSET_MAXHEIGHT 6
#define TXLIN_TEXTSET_HALFHEIGHT (TXLIN_TEXTSET_MAXHEIGHT / 2)

#define TXLIN_UNPORTABLEDEF_SQUARE(num) ((num) * (num))

struct POINT {
    double x;
    double y;   
};

struct COLORREF {
    int r;
    int g;
    int b;
};

static bool txLinUnportableHasInitializedTXLinInThisContext = false;
static HWND txLinUnportableRecentlyCreatedWindow = -1;

inline HWND txWindow() {
    DBGINT(txLinUnportableRecentlyCreatedWindow);
    return txLinUnportableRecentlyCreatedWindow;
}

inline SDL_Point* txLinUnportablePointCapsToSDL(const POINT* points, int pointsCount) {
    SDL_Point* pointsFinal = (SDL_Point*)(calloc(pointsCount, sizeof(SDL_Point)));
    for (int i = 0; i < pointsCount; i++) {
        pointsFinal[i].x = (int)(points[i].x);
        pointsFinal[i].y = (int)(points[i].y);
    }
    return pointsFinal;
}

inline bool txLinUnportableInitSDL() {
    return (SDL_Init(SDL_INIT_VIDEO) == 0);
}

inline int txLinUnportableModule(int number) {
    if (number < 0)
        return ((number) * (-1));
    return number;
}

inline void txRedrawWindow() {
    SDL_UpdateWindowSurface(SDL_GetWindowFromID(txWindow()));
}

inline HWND txCreateWindow(double sizeX, double sizeY, bool centered = true) {
    if (txLinUnportableHasInitializedTXLinInThisContext && txLinUnportableInitSDL() == false)
        return -1;
    int x = SDL_WINDOWPOS_CENTERED;
    int y = SDL_WINDOWPOS_CENTERED;
    if (centered == false) {
        x = 2;
        y = 2;
    }
    SDL_Window* window = SDL_CreateWindow("TXLin", x, y, (int)(sizeX), (int)(sizeY), SDL_WINDOW_SHOWN);
    txLinUnportableRecentlyCreatedWindow = SDL_GetWindowID(window);
    return txLinUnportableRecentlyCreatedWindow;
}

inline HDC txDC();

inline bool txSetDefaults(HDC dc = txDC()) {
    DBGOUT << "called txSetDefaults" << std::endl;
    SDL_Renderer* rendererContext = dc;
    if (rendererContext == nullptr)
        return false;
    SDL_SetRenderDrawColor(rendererContext, 255, 255, 255, 0);
    //SDL_RenderClear(rendererContext);
    return true;
}

HDC txDC() {
    DBGOUT << "called txDC()" << std::endl;
    SDL_Window* window = SDL_GetWindowFromID(txWindow());
    if (window == nullptr)
        return nullptr;
    SDL_Renderer* resultRenderer = SDL_GetRenderer(window);
    if (resultRenderer == nullptr) {
        DBGOUT << "regenerating renderer" << std::endl;
        resultRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
        txSetDefaults(resultRenderer);
        txRedrawWindow();
        //txSetDefaults(resultRenderer);
    }
    return resultRenderer;
}

inline RGBQUAD* txVideoMemory(HDC dc = txDC()) {
    //if (dc == nullptr)
        return nullptr;
    //return (RGBQUAD*)(dc->pixels);
}


inline bool txOK() {
    return txLinUnportableHasInitializedTXLinInThisContext;
}


inline POINT txGetExtent (HDC dc = txDC()) {
    POINT result;
    result.x = 0.0;
    result.y = 0.0;
    int xSet = 0;
    int ySet = 0;
    if (SDL_GetRendererOutputSize(dc, &xSet, &ySet) != 0)
        return result;
    result.x = (double)(xSet);
    result.y = (double)(ySet);
    return result;
}


inline int txGetExtentX (HDC dc = txDC()) {
    return (int)(txGetExtent(dc).x);
}


inline int txGetExtentY (HDC dc = txDC()) {
     return (int)(txGetExtent(dc).y);
}


inline const char* txVersion() {
    return TXLIN_VERSION;
}

inline unsigned txVersionNumber() {
    return TXLIN_VERSIONNUM;
}

const char* txGetModuleFileName (bool fileNameOnly = true) {
    // currently does not work
    (void)(fileNameOnly);
    return nullptr;
}

COLORREF RGB (int red, int green, int blue) {
    COLORREF result = { red, green, blue };
    return result;
}

HPEN txSetColor (COLORREF color, double thickness = 1, HDC dc = txDC()) {
    (void)(thickness);
    SDL_Renderer* rendererContext = dc;
    if (rendererContext == nullptr)
        return nullptr;
    SDL_SetRenderDrawColor(rendererContext, color.r, color.g, color.b, 0);
    return rendererContext;
}

COLORREF txColor (double red, double green, double blue) {
    COLORREF color = { (int)(red), (int)(green), (int)(blue) };
    txSetColor(color);
    return color;
}

COLORREF txGetColor (HDC dc = txDC()) {
    COLORREF result = { 255, 255, 255 };
    if (dc == nullptr)
        return result;
    Uint8 rBit, gBit, bBit;
    SDL_GetRenderDrawColor(dc, &rBit, &gBit, &bBit, nullptr);
    result.r = (int)(rBit);
    result.g = (int)(gBit);
    result.b = (int)(bBit);
    return result;
}

COLORREF txFillColor (double red, double green, double blue) {
    return txColor(red, green, blue);
} 

COLORREF txGetFillColor (HDC dc = txDC()) {
    return txGetColor(dc);
}

unsigned txExtractColor (COLORREF color, COLORREF component) {
    // does not work yet
    return 0;
}

COLORREF txRGB2HSL (COLORREF rgbColor) {
    // does not work yet
    return rgbColor;
}

COLORREF txHSL2RGB (COLORREF hslColor) {
    // does not work yet
    return hslColor;
}


bool txClear (HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    SDL_RenderClear(dc);
    return true;
}


inline bool txSetPixel (double x, double y, COLORREF color = txGetColor(), HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    txSetColor(color);
    SDL_RenderDrawPoint(dc, (int)(x), (int)(y));
    return true;
}

inline bool txPixel (double x, double y, double red, double green, double blue, HDC dc = txDC()) {
    return txSetPixel(x, y, RGB((int)(red), (int)(green), (int)(blue)), dc);
}

inline COLORREF txGetPixel (double x, double y, HDC dc = txDC()) {
    /*
    Uint32* arrayOfPixels = txVideoMemory(dc);
    Uint32 pixel = arrayOfPixels[((int)(y) * txGetExtentX(dc)) + (int)(x)];
    Uint8 rBit, gBit, bBit;
    SDL_GetRGB(pixel, nullptr, &rBit, &gBit, &bBit);
    COLORREF result = { (int)(rBit), (int)(gBit), (int)(bBit) };
    */
    COLORREF result = { 255, 255, 255 };
    return result;
}

inline bool txLine (double x0, double y0, double x1, double y1, HDC dc = txDC()) {
    if (dc == nullptr) {
        DBGOUT << "dc is nullptr, return false" << std::endl;
        return false;
    }
    DBGINT(x0);
    DBGINT(y0);
    DBGINT(x1);
    DBGINT(y1);
    DBGOUT << "SDL_RenderDrawLine" << std::endl;
    SDL_RenderDrawLine(dc, (int)(x0), (int)(y0), (int)(x1), (int)(y1));
    DBGOUT << "txRedrawWindow()" << std::endl;
    txRedrawWindow(); 
    DBGOUT << "return true;" << std::endl;
    return true;
}

inline bool txRectangle (double x0, double y0, double x1, double y1, HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    SDL_Rect rectangle;
    rectangle.x = (int)(x0);
    rectangle.y = (int)(y0);
    rectangle.w = txLinUnportableModule(x1 - x0);
    rectangle.h = txLinUnportableModule(y1 - y0);
    SDL_RenderDrawRect(dc, &rectangle);
    txRedrawWindow();
    return true;
}

inline bool txPolygon (const POINT* points, int numPoints, HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    SDL_Point* sdlPoints = txLinUnportablePointCapsToSDL(points, numPoints);
    SDL_RenderDrawPoints(dc, sdlPoints, numPoints);
    free(sdlPoints);
    sdlPoints = nullptr;
    txRedrawWindow();
    return true;
} 

inline bool txCircle (double x, double y, double r) {
    int xtmp = (int)(r);
    int ytmp = 0;
    txSetPixel((int)(x) + xtmp, (int)(y) + ytmp);
    if ((int)(r) > 0) {
        txSetPixel((int)(x) + xtmp, (ytmp * (-1)) + (int)(y));
        txSetPixel((int)(x) + ytmp, xtmp + (int)(y));
        txSetPixel((int)(x) + (ytmp * (-1)), xtmp + (int)(y));
    }
    int P = 1 - (int)(r);
    while (xtmp > ytmp) {
        ytmp = ytmp + 1;
        if (P <= 0)
            P = P + (2 * ytmp) + 1;
        else {
            xtmp = xtmp - 1;
            P = P + (2 * ytmp) - (2 * xtmp) + 1;
        }
        if (xtmp < ytmp)
            break;
        txSetPixel((int)(x) + xtmp, ytmp + (int)(y));
        txSetPixel((int)(x) + (xtmp * (-1)), ytmp + (int)(y));
        txSetPixel((int)(x) + xtmp, (ytmp * (-1)) + (int)(y));
        txSetPixel((int)(x) + (xtmp * (-1)), (ytmp * (-1)) + (int)(y));
        if (xtmp != ytmp) {
            txSetPixel((int)(x) + ytmp, xtmp + (int)(y));
            txSetPixel((int)(x) + ((-1) * ytmp), xtmp + (int)(y));
            txSetPixel((int)(x) + ytmp, ((-1) * xtmp) + (int)(y));
            txSetPixel((int)(x) + ((-1) * ytmp), ((-1) * xtmp) + (int)(y));
        }
    }
    return true;
}

bool txFloodFill (double x, double y, COLORREF color, DWORD mode = FLOODFILLSURFACE, HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    TXLIN_WARNINGNOTIMPLEMENTED();
    return true;
}

/*


bool txArc (double x0, double y0, double x1, double y1, double startAngle, double totalAngle, HDC dc = txDC());


bool txPie (double x0, double y0, double x1, double y1, double startAngle, double totalAngle, HDC dc = txDC());


bool txChord (double x0, double y0, double x1, double y1, double startAngle, double totalAngle, HDC dc = txDC());


bool txFloodFill (double x, double y, COLORREF color = TX_TRANSPARENT, DWORD mode = FLOODFILLSURFACE, HDC dc = txDC());
*/

bool txTriangle (double x1, double y1, double x2, double y2, double x3, double y3) {
    if (txLine(x1, y1, x2, y2) && txLine(x2, y2, x3, y3) && txLine(x3, y3, x1, y1))
        return true;
    return false;
}


inline int txBegin() {
    return -1;
}

inline int txEnd() {
    return -1;
}

inline void txLinUnportableSDLEventLoop() {
    SDL_Delay(1000);
    bool stop = false;
    SDL_Event* eventHandler = (SDL_Event*)(malloc(sizeof(SDL_Event)));
    while (stop == false) {
        while (SDL_PollEvent(eventHandler) != 0) {
            if (eventHandler->type == SDL_QUIT) {
                stop = true;
                break;
            }
        }
    }
    return;
}

inline void txLinUnportableMonolithicCharacterSet(int x, int y, const char character) {
    char upperChar = (char)(toupper(character));
    if (upperChar == ' ')
        DBGOUT << "space" << std::endl;
    else if (upperChar == '.')
        txSetPixel((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    else if (upperChar == '!') {
        txLinUnportableMonolithicCharacterSet(x, y, '.');
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT - 2);
    }
    else if (upperChar == '+') {
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == ',')
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT - 2, (int)(x) - 1, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT + 2);
    else if (upperChar == 'A') {
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y));
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == 'B' || upperChar == '6' || upperChar == '9') {
        if (upperChar == '9')
            txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        else
            txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        //txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        if (upperChar != '6')
            txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'C') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'D') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'E' || upperChar == 'F' || upperChar == '3') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        if (upperChar != 'F')
            txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        if (upperChar != '3')
            txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        else
            txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'G') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == 'H') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXHEIGHT, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == 'I')
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    else if (upperChar == 'J') {
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'K') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == 'L') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'M') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXHEIGHT, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == 'N') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXHEIGHT, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXHEIGHT, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'P' || upperChar == 'R') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT + 2);
        if (upperChar == 'R')
            txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'Q') {
        txCircle((int)(x), (int)(y), (int)(TXLIN_TEXTSET_HALFHEIGHT));
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'S' || upperChar == '5') {
        if (upperChar == 'S')
            txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        else {
            txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + 1);
            txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + 1, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        }
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'T') {
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == 'U') {
         txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
         txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
         txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXHEIGHT, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == 'V') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == 'W') {
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXHEIGHT, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == 'X') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == 'Y') {
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == 'Z') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == '1') {
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_HALFWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
    }
    else if (upperChar == '2') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == '4') {
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x), (int)(y), (int)(x), (int)(y) + TXLIN_TEXTSET_HALFHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else if (upperChar == '7') {
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
    }
    else if (upperChar == '8') {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
    }
    else {
        txLine((int)(x), (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y));
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y), (int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x) + TXLIN_TEXTSET_MAXWIDTH, (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT);
        txLine((int)(x), (int)(y) + TXLIN_TEXTSET_MAXHEIGHT, (int)(x), (int)(y));
    }
}

inline bool txTextOut(double x, double y, const char* text, HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    int posx = (int)(x);
    for (int i = 0; i < strlen(text); i++) {
        txLinUnportableMonolithicCharacterSet(posx, (int)(y), text[i]);
        posx = posx + TXLIN_TEXTSET_MAXWIDTH + 3;
    }
    return true;
}

inline HFONT txSelectFont (const char* name, double sizeY, HDC dc = txDC()) {
    (void)(dc);
    (void)(sizeY);
    (void)(name);
    TXLIN_WARNING(std::string("txSelectFont(const char* name, double sizeY, HDC dc) was added to TXLin for source compatibility purposes. It actually does nothing, because TXLin uses its own monolithic font engine rather than FontConfig. Thus, the font \"" + std::string(name) + "\" is not available."));
    return nullptr;
}

unsigned txSetTextAlign (unsigned align = TA_LEFT, HDC dc = txDC()) {
    if (dc == nullptr)
        return 0;
    (void)(align);
    TXLIN_WARNING("txSetTextAlign(unsigned align, HDC dc) was added to TXLin for source compatibility purposes. It actually does nothing, because TXLib does not support text alignment at the moment.");
    return TA_LEFT;
}

inline void txTextCursor(bool value = true) {
    // does not work yet
    (void)(value);
    TXLIN_WARNING("txTextCursor(bool value) was added to TXLin for source compatibility. It actually does nothing, because there is no easy and cross-platform way to turn off cursor blinking on both Linux and Mac OS X.");
}

inline HBRUSH txSetFillColor(COLORREF color, HDC dc = txDC()) {
    if (dc == nullptr)
        return nullptr;
    TXLIN_WARNING("txSetFillColor(COLORREF color, HDC dc) and txSetColor(COLORREF color, double thickness, HDC dc) do the same thing. In TXLib, there is no difference between the fill color and the foreground color.");
    return txSetColor(color, 1, dc);
}

inline bool txEllipse(double x0, double y0, double x1, double y1, HDC dc = txDC()) {
    if (dc == nullptr)
        return false;
    int width = txLinUnportableModule((int)(x1 - x0));
    int height = txLinUnportableModule((int)(y1 - y0));
    for (int y = (height * (-1)); y < (height + 1); y++) {
        for (int x = (width * (-1)); x < (width + 1); x++) {
            if ((TXLIN_UNPORTABLEDEF_SQUARE(x) * (TXLIN_UNPORTABLEDEF_SQUARE(height)) <= (TXLIN_UNPORTABLEDEF_SQUARE(width) * TXLIN_UNPORTABLEDEF_SQUARE(height))))
                txSetPixel((int)(x0) + x, (int)(y0) + y);
        }
    }
    return true;
}

#endif