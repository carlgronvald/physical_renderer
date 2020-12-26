#pragma once

struct Color {
    float R, G, B;
    int Geom;

    Color(float red, float green, float blue, int geom=-1) {
        R = red;
        G = green;
        B = blue;
        Geom = geom;
    }

    Color operator+(const Color& rhs) {
        return Color(R + rhs.R, G + rhs.G, B + rhs.B, Geom);
    }

    Color operator*(const float rhs) {
        return Color(R * rhs, G * rhs, B * rhs, Geom);
    }

    Color operator*(const Color& rhs) {
        return Color(R * rhs.R, G * rhs.G, B * rhs.B, Geom);
    }

    Color operator%(const int rhs) {
        Color n(R, G, B, Geom);
        while (n.R > rhs) {
            n.R -= rhs;
        }
        while (n.G > rhs) {
            n.G -= rhs;
        }
        while (n.B > rhs) {
            n.B -= rhs;
        }
        return n;
    }
};