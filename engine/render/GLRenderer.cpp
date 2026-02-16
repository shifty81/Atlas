#include "GLRenderer.h"
#include <GL/gl.h>

namespace atlas::render {

void GLRenderer::SetViewport(int32_t width, int32_t height) {
    m_viewportWidth = width;
    m_viewportHeight = height;
}

void GLRenderer::BeginFrame() {
    glViewport(0, 0, m_viewportWidth, m_viewportHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, static_cast<double>(m_viewportWidth),
            static_cast<double>(m_viewportHeight), 0.0,
            -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.18f, 0.18f, 0.20f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void GLRenderer::EndFrame() {
    glFlush();
}

static void glSetColor(const atlas::ui::UIColor& color) {
    glColor4ub(color.r, color.g, color.b, color.a);
}

void GLRenderer::DrawRect(const ui::UIRect& rect, const ui::UIColor& color) {
    glSetColor(color);
    glBegin(GL_QUADS);
    glVertex2i(rect.x, rect.y);
    glVertex2i(rect.x + rect.w, rect.y);
    glVertex2i(rect.x + rect.w, rect.y + rect.h);
    glVertex2i(rect.x, rect.y + rect.h);
    glEnd();
}

void GLRenderer::DrawText(const ui::UIRect& rect, const std::string& text, const ui::UIColor& color) {
    // Render each character as a small filled rectangle placeholder.
    // A production font rasterizer would use glyph bitmaps; this provides
    // visible, readable-position text so the editor layout is usable.
    glSetColor(color);

    const int charW = 8;
    const int charH = 13;
    int x = rect.x + 4;
    int y = rect.y + charH + 2;

    for (size_t i = 0; i < text.size(); ++i) {
        char ch = text[i];
        if (ch == '\n') {
            x = rect.x + 4;
            y += charH + 2;
            if (y > rect.y + rect.h) break;
            continue;
        }
        if (x + charW > rect.x + rect.w) break;

        // Draw a small glyph placeholder block per character
        glBegin(GL_QUADS);
        glVertex2i(x, y - charH + 2);
        glVertex2i(x + charW - 2, y - charH + 2);
        glVertex2i(x + charW - 2, y);
        glVertex2i(x, y);
        glEnd();

        x += charW;
    }
}

void GLRenderer::DrawIcon(const ui::UIRect& rect, uint32_t /*iconId*/, const ui::UIColor& tint) {
    // Placeholder: draw a diamond shape as icon stand-in
    glSetColor(tint);
    int cx = rect.x + rect.w / 2;
    int cy = rect.y + rect.h / 2;
    glBegin(GL_QUADS);
    glVertex2i(cx, rect.y);
    glVertex2i(rect.x + rect.w, cy);
    glVertex2i(cx, rect.y + rect.h);
    glVertex2i(rect.x, cy);
    glEnd();
}

void GLRenderer::DrawBorder(const ui::UIRect& rect, int32_t thickness, const ui::UIColor& color) {
    // Top
    DrawRect({rect.x, rect.y, rect.w, thickness}, color);
    // Bottom
    DrawRect({rect.x, rect.y + rect.h - thickness, rect.w, thickness}, color);
    // Left
    DrawRect({rect.x, rect.y, thickness, rect.h}, color);
    // Right
    DrawRect({rect.x + rect.w - thickness, rect.y, thickness, rect.h}, color);
}

void GLRenderer::DrawImage(const ui::UIRect& rect, uint32_t /*textureId*/, const ui::UIColor& tint) {
    // Placeholder: draw a tinted rectangle with a cross pattern
    DrawRect(rect, tint);

    ui::UIColor dark = {
        static_cast<uint8_t>(tint.r / 2),
        static_cast<uint8_t>(tint.g / 2),
        static_cast<uint8_t>(tint.b / 2),
        tint.a
    };

    glSetColor(dark);
    glBegin(GL_LINES);
    glVertex2i(rect.x, rect.y);
    glVertex2i(rect.x + rect.w, rect.y + rect.h);
    glVertex2i(rect.x + rect.w, rect.y);
    glVertex2i(rect.x, rect.y + rect.h);
    glEnd();
}

} // namespace atlas::render
