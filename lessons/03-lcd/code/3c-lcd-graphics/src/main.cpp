/*

  M5Stack Tab5 LCD + Canvas + Graphics Tutorial

  by Bryan A. "CrazyUncleBurton" Thompson

  Last Updated 3/15/2026

*/

#include <M5Unified.h>
#include <SPIFFS.h>
#include <cmath>
#include <algorithm>

M5GFX& display = M5.Display;
M5Canvas actorSprite(&display);
M5Canvas frameCanvas(&display);

enum DemoState {
  STATE_SHAPES = 0,
  STATE_WIDGETS,
  STATE_JPG,
  STATE_QR,
  STATE_SPRITES,
  STATE_ART,
  STATE_COUNT
};

DemoState currentState = STATE_SHAPES;
DemoState lastRenderedState = STATE_COUNT;
uint32_t frameCounter = 0;
uint32_t lastFrameMs = 0;
bool spiffsReady = false;
bool actorSpriteReady = false;
bool frameCanvasReady = false;
bool redrawStaticState = true;
uint32_t lastTouchPollMs = 0;
uint32_t lastButtonActionMs = 0;
bool turboMode = false;
String commandHint;

int spriteX = 120;
int spriteY = 120;
int spriteVx = 7;
int spriteVy = 6;

const int headerHeight = 36;
const int footerHeight = 112;
const int buttonHeight = 42;
const int buttonMargin = 16;
const uint32_t touchPollIntervalMs = 4;
const uint32_t buttonRepeatMs = 120;
const int turboButtonWidth = 180;
const int turboButtonHeight = 28;

const char* stateNames[STATE_COUNT] = {
  "1) Shapes",
  "2) Widgets",
  "3) JPG from SPIFFS",
  "4) QR Code",
  "5) Sprites",
  "6) Animated Art"
};

void nextState()
{
  currentState = static_cast<DemoState>((static_cast<int>(currentState) + 1) % STATE_COUNT);
}

void previousState()
{
  int next = static_cast<int>(currentState) - 1;
  if (next < 0) {
    next = STATE_COUNT - 1;
  }
  currentState = static_cast<DemoState>(next);
}

bool isAnimatedState(DemoState state)
{
  return (state == STATE_WIDGETS) || (state == STATE_SPRITES) || (state == STATE_ART);
}

uint32_t getFrameIntervalMs(DemoState state)
{
  switch (state) {
    case STATE_WIDGETS: return turboMode ? 8 : 12;
    case STATE_SPRITES: return turboMode ? 6 : 9;
    case STATE_ART: return turboMode ? 8 : 12;
    default: return 33;
  }
}

void ensureFrameCanvas()
{
  if (frameCanvasReady) {
    return;
  }

  frameCanvas.setColorDepth(16);
  frameCanvas.createSprite(display.width(), display.height());
  frameCanvasReady = true;
}

template <typename TGfx>
void drawButton(TGfx& gfx, int x, int y, int w, int h, const char* label, uint16_t color)
{
  gfx.fillRoundRect(x, y, w, h, 10, color);
  gfx.drawRoundRect(x, y, w, h, 10, TFT_WHITE);
  gfx.setTextColor(TFT_WHITE, color);
  gfx.setTextSize(2);
  gfx.drawCenterString(label, x + (w / 2), y + 10);
}

template <typename TGfx>
void drawChrome(TGfx& gfx)
{
  const int w = gfx.width();
  const int h = gfx.height();
  const int buttonTop = h - buttonHeight - 10;
  const int buttonWidth = (w - (buttonMargin * 3)) / 2;

  gfx.fillRect(0, 0, w, headerHeight, TFT_DARKGREEN);
  gfx.setTextSize(2);
  gfx.setTextColor(TFT_WHITE, TFT_DARKGREEN);
  gfx.drawString("M5GFX Demo State Machine", 8, 8);

  const int turboX = w - turboButtonWidth - 8;
  const int turboY = 4;
  const uint16_t turboColor = turboMode ? TFT_RED : TFT_DARKCYAN;
  gfx.fillRoundRect(turboX, turboY, turboButtonWidth, turboButtonHeight, 8, turboColor);
  gfx.drawRoundRect(turboX, turboY, turboButtonWidth, turboButtonHeight, 8, TFT_WHITE);
  gfx.setTextColor(TFT_WHITE, turboColor);
  gfx.setTextSize(1);
  gfx.drawCenterString(turboMode ? "Turbo: ON" : "Turbo: OFF", turboX + (turboButtonWidth / 2), turboY + 8);

  gfx.fillRect(0, h - footerHeight, w, footerHeight, TFT_DARKGREY);
  gfx.setTextColor(TFT_YELLOW, TFT_DARKGREY);
  gfx.setTextSize(2);
  gfx.drawString(stateNames[currentState], 8, h - footerHeight + 6);

  gfx.setTextColor(TFT_CYAN, TFT_DARKGREY);
  gfx.setTextSize(2);
  gfx.drawCenterString(commandHint, w / 2, h - footerHeight + 34);

  drawButton(gfx, buttonMargin, buttonTop, buttonWidth, buttonHeight, "Last", TFT_BLUE);
  drawButton(gfx, buttonMargin * 2 + buttonWidth, buttonTop, buttonWidth, buttonHeight, "Next", TFT_GREEN);
}

void drawStateShapes()
{
  const int sceneTop = headerHeight;

  display.drawCircle(95, sceneTop + 88, 58, TFT_SKYBLUE);
  display.fillCircle(245, sceneTop + 88, 58, TFT_BLUE);
  display.drawRect(340, sceneTop + 28, 130, 118, TFT_WHITE);
  display.fillRect(490, sceneTop + 28, 130, 118, TFT_ORANGE);

  display.drawTriangle(90, sceneTop + 300, 170, sceneTop + 182, 250, sceneTop + 300, TFT_MAGENTA);
  display.fillTriangle(300, sceneTop + 300, 385, sceneTop + 182, 470, sceneTop + 300, TFT_RED);

  display.drawEllipse(560, sceneTop + 220, 65, 40, TFT_CYAN);
  display.fillEllipse(700, sceneTop + 220, 65, 40, TFT_DARKCYAN);

  for (int i = 0; i < 80; ++i) {
    display.drawPixel(30 + (i * 4), sceneTop + 342 + (i % 3), TFT_YELLOW);
  }
  display.drawLine(30, sceneTop + 372, 770, sceneTop + 372, TFT_GREEN);

  commandHint = "drawCircle(...) fillRect(...) drawLine(...)";
}

template <typename TGfx>
void drawStateWidgets(TGfx& gfx)
{
  const int sceneTop = headerHeight;
  const int w = gfx.width();
  const int barX = 50;
  const int barY = sceneTop + 70;
  const int barW = w - 100;
  const int barH = 36;
  const int phase = (frameCounter * (turboMode ? 6 : 4)) % 200;
  const int level = (phase <= 100) ? phase : (200 - phase);

  gfx.drawRoundRect(barX, barY, barW, barH, 12, TFT_WHITE);
  gfx.fillRoundRect(barX + 3, barY + 3, ((barW - 6) * level) / 100, barH - 6, 10, TFT_GREEN);
  gfx.setTextColor(TFT_WHITE, TFT_BLACK);
  gfx.setTextSize(2);
  gfx.drawString("Progress", barX, barY - 28);
  gfx.drawRightString(String(level) + "%", barX + barW - 8, barY + 9);

  const int meterCx = 160;
  const int meterCy = sceneTop + 190;
  const int meterR = 70;
  for (int i = 0; i <= 100; i += 5) {
    float angle = (225.0f - (270.0f * (static_cast<float>(i) / 100.0f))) * 0.0174533f;
    int x0 = meterCx + static_cast<int>(cosf(angle) * (meterR - 18));
    int y0 = meterCy - static_cast<int>(sinf(angle) * (meterR - 18));
    int x1 = meterCx + static_cast<int>(cosf(angle) * meterR);
    int y1 = meterCy - static_cast<int>(sinf(angle) * meterR);
    uint16_t c = (i <= level) ? TFT_ORANGE : TFT_DARKGREY;
    gfx.drawLine(x0, y0, x1, y1, c);
  }

  gfx.drawCircle(meterCx, meterCy, 10, TFT_WHITE);
  gfx.fillCircle(meterCx, meterCy, 8, TFT_NAVY);

  const int barsX = 360;
  const int barsBaseY = sceneTop + 230;
  for (int i = 0; i < 5; ++i) {
    int bh = 20 + ((level + i * 15) % 100);
    gfx.drawRect(barsX + (i * 34), barsBaseY - 100, 24, 100, TFT_LIGHTGREY);
    gfx.fillRect(barsX + (i * 34) + 2, barsBaseY - bh, 20, bh, TFT_CYAN);
  }

  commandHint = "fillRoundRect(...) + drawLine(...) + drawRect(...)";
}

void drawStateJpg()
{
  const int sceneTop = headerHeight;
  const int sceneH = display.height() - headerHeight - footerHeight;
  display.fillRect(0, sceneTop, display.width(), sceneH, TFT_BLACK);

  if (spiffsReady && SPIFFS.exists("/demo.jpg")) {
    bool jpgOk = display.drawJpgFile("/spiffs/demo.jpg", 40, sceneTop + 20);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(2);
    if (jpgOk) {
      display.drawString("Loaded /demo.jpg", 40, sceneTop + sceneH - 44);
    } else {
      display.drawString("/demo.jpg decode failed", 40, sceneTop + 90);
      display.setTextSize(1);
      display.drawString("Try replacing with a baseline/progressive-compatible JPG", 40, sceneTop + 126);
    }
  } else {
    display.drawRect(40, sceneTop + 20, 520, 220, TFT_RED);
    display.setTextColor(TFT_WHITE, TFT_BLACK);
    display.setTextSize(2);
    display.drawString("No /demo.jpg in SPIFFS", 56, sceneTop + 90);
    display.setTextSize(1);
    display.drawString("Add file: 3c-lcd-graphics/data/demo.jpg then Upload Filesystem Image", 56, sceneTop + 126);
  }

  commandHint = "drawJpgFile(\"/spiffs/demo.jpg\", x, y)";
}

void drawStateQr()
{
  const int sceneW = display.width();
  const int sceneTop = headerHeight;
  const int sceneH = display.height() - headerHeight - footerHeight;
  const int qrSize = std::min(sceneW, sceneH) - 40;
  const int qrX = (sceneW - qrSize) / 2;
  const int qrY = sceneTop + ((sceneH - qrSize) / 2);

  display.fillRect(0, sceneTop, sceneW, sceneH, TFT_WHITE);
  display.qrcode("https://crazyuncleburton.com", qrX, qrY, qrSize, 7);
  display.setTextColor(TFT_BLACK, TFT_WHITE);
  display.setTextSize(1);
  display.drawCenterString("https://crazyuncleburton.com", sceneW / 2, sceneTop + sceneH - 18);

  commandHint = "qrcode(url, x, y, size, version)";
}

template <typename TGfx>
void drawStateSprites(TGfx& gfx)
{
  const int sceneTop = headerHeight;
  const int sceneH = gfx.height() - headerHeight - footerHeight;
  const int sceneBottom = sceneTop + sceneH;

  if (!actorSpriteReady) {
    actorSprite.setColorDepth(16);
    actorSprite.createSprite(72, 72);
    actorSpriteReady = true;
  }

  gfx.fillRect(0, sceneTop, gfx.width(), sceneH, TFT_NAVY);
  for (int i = 0; i < 20; ++i) {
    int sx = (i * 29 + (frameCounter * 2)) % gfx.width();
    int sy = sceneTop + ((i * 37) % sceneH);
    gfx.drawPixel(sx, sy, TFT_WHITE);
  }

  actorSprite.fillSprite(TFT_BLACK);
  actorSprite.fillCircle(36, 36, 32, TFT_GOLD);
  actorSprite.drawCircle(36, 36, 32, TFT_WHITE);
  actorSprite.fillCircle(24, 28, 5, TFT_BLACK);
  actorSprite.fillCircle(48, 28, 5, TFT_BLACK);
  actorSprite.drawFastHLine(18, 48, 36, TFT_BLACK);

  spriteX += spriteVx;
  spriteY += spriteVy;
  if (turboMode) {
    spriteX += spriteVx;
    spriteY += spriteVy;
  }
  if (spriteX < 0 || spriteX > gfx.width() - 72) {
    spriteVx = -spriteVx;
    spriteX += spriteVx;
  }
  if (spriteY < sceneTop || spriteY > sceneBottom - 72) {
    spriteVy = -spriteVy;
    spriteY += spriteVy;
  }

  actorSprite.pushSprite(&gfx, spriteX, spriteY, TFT_BLACK);
  commandHint = "sprite.pushSprite(x, y, transparentColor)";
}

template <typename TGfx>
void drawStateArt(TGfx& gfx)
{
  const int sceneTop = headerHeight;
  const int sceneH = gfx.height() - headerHeight - footerHeight;
  const int centerY = sceneTop + (sceneH / 2);
  const float t = frameCounter * (turboMode ? 0.22f : 0.17f);

  gfx.fillRect(0, sceneTop, gfx.width(), sceneH, TFT_BLACK);

  for (int x = 0; x < gfx.width(); x += 6) {
    float s1 = sinf((x * 0.035f) + t);
    float s2 = sinf((x * 0.077f) - (t * 1.4f));
    int y = centerY + static_cast<int>((s1 * 55.0f) + (s2 * 25.0f));
    uint8_t r = static_cast<uint8_t>(128 + (127 * sinf((x * 0.01f) + t)));
    uint8_t g = static_cast<uint8_t>(128 + (127 * sinf((x * 0.015f) + t + 2.0f)));
    uint8_t b = static_cast<uint8_t>(128 + (127 * sinf((x * 0.02f) + t + 4.0f)));
    gfx.drawLine(x, centerY, x, y, gfx.color565(r, g, b));
  }

  int pulse = 30 + static_cast<int>(20.0f * (sinf(t * 1.7f) + 1.0f));
  gfx.drawCircle(gfx.width() / 2, centerY, pulse, TFT_WHITE);
  gfx.fillCircle(gfx.width() / 2, centerY, pulse / 3, TFT_CYAN);

  commandHint = "drawLine(...) + drawCircle(...) + fillCircle(...)";
}

void renderStaticFrame()
{
  const int sceneTop = headerHeight;
  const int sceneH = display.height() - headerHeight - footerHeight;
  display.fillRect(0, sceneTop, display.width(), sceneH, TFT_BLACK);

  switch (currentState) {
    case STATE_SHAPES: drawStateShapes(); break;
    case STATE_JPG: drawStateJpg(); break;
    case STATE_QR: drawStateQr(); break;
    default: break;
  }

  drawChrome(display);
}

void renderAnimatedFrame()
{
  ensureFrameCanvas();
  const int sceneTop = headerHeight;
  const int sceneH = frameCanvas.height() - headerHeight - footerHeight;

  frameCanvas.fillScreen(TFT_BLACK);
  frameCanvas.fillRect(0, sceneTop, frameCanvas.width(), sceneH, TFT_BLACK);

  switch (currentState) {
    case STATE_WIDGETS: drawStateWidgets(frameCanvas); break;
    case STATE_SPRITES: drawStateSprites(frameCanvas); break;
    case STATE_ART: drawStateArt(frameCanvas); break;
    default: break;
  }

  drawChrome(frameCanvas);
  frameCanvas.pushSprite(0, 0);
}

void handleTouchNavigation(uint32_t now)
{
  if (now - lastTouchPollMs < touchPollIntervalMs) {
    return;
  }
  lastTouchPollMs = now;

  const int w = display.width();
  const int h = display.height();
  const int buttonTop = h - buttonHeight - 10;
  const int buttonWidth = (w - (buttonMargin * 3)) / 2;
  const int leftX = buttonMargin;
  const int rightX = buttonMargin * 2 + buttonWidth;
  const int turboX = w - turboButtonWidth - 8;
  const int turboY = 4;

  uint16_t x = 0;
  uint16_t y = 0;
  bool touchDown = display.getTouch(&x, &y);

  if (touchDown && (now - lastButtonActionMs >= buttonRepeatMs)) {
    if (x >= turboX && x <= (turboX + turboButtonWidth)
     && y >= turboY && y <= (turboY + turboButtonHeight)) {
      turboMode = !turboMode;
      lastFrameMs = 0;
      redrawStaticState = true;
      lastButtonActionMs = now;
      return;
    }

    if (y >= buttonTop && y <= (buttonTop + buttonHeight)) {
      if (x >= leftX && x <= (leftX + buttonWidth)) {
        previousState();
        lastButtonActionMs = now;
      } else if (x >= rightX && x <= (rightX + buttonWidth)) {
        nextState();
        lastButtonActionMs = now;
      }
    }
  }
}

void setup()
{
  auto cfg = M5.config();
  M5.begin(cfg);

  display.setRotation(1);
  display.fillScreen(TFT_BLACK);
  display.setTextFont(1);

  spiffsReady = SPIFFS.begin(false);
  commandHint = "drawCircle(x, y, r, color)";

  redrawStaticState = true;
}

void loop()
{
  const uint32_t now = millis();
  M5.update();
  handleTouchNavigation(now);

  if (currentState != lastRenderedState) {
    lastRenderedState = currentState;
    redrawStaticState = true;
  }

  const uint32_t frameIntervalMs = getFrameIntervalMs(currentState);

  if (isAnimatedState(currentState)) {
    if (now - lastFrameMs >= frameIntervalMs) {
      lastFrameMs = now;
      ++frameCounter;
      renderAnimatedFrame();
      redrawStaticState = true;
    }
  } else if (redrawStaticState) {
    lastFrameMs = now;
    renderStaticFrame();
    redrawStaticState = false;
  }
}
