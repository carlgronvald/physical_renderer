#pragma once

struct Color {
    float R, G, B;

    Color(float red, float green, float blue) {
        R = red;
        G = green;
        B = blue;
    }

    Color operator+(const Color& rhs) {
        return Color(R + rhs.R, G + rhs.G, B + rhs.B);
    }

    Color operator*(const float rhs) {
        return Color(R * rhs, G * rhs, B * rhs);
    }

    Color operator*(const Color& rhs) {
        return Color(R * rhs.R, G * rhs.G, B * rhs.B);
    }
};