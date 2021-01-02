#pragma once
struct Complex {
public:
	float R, I;

	Complex(float R, float I) : R(R), I(I) {

	}

	float SqrAbs() {
		return R * R + I * I;
	}

	Complex operator+(const Complex& rhs) {
		return Complex(R + rhs.R, I + rhs.I);
	}

	Complex operator*(const Complex& rhs) {
		return Complex(R * rhs.R - I * rhs.I, R * rhs.I + I * rhs.R);
	}
};

class Test
{
public:
	virtual void run(int w, int h, char* data, int AA=1) = 0;
};

class TestEquations :
	public Test
{
	const static int depth = 100;
	Complex Recurse(Complex z, Complex c) {
		return z * z*Complex(1.0,im) + c;
	}

	int Recursions(Complex z, Complex c) {
		for (int i = 0; i < depth-1; i++) {
			z = Recurse(z, c);
			if (z.SqrAbs() > 1)
				return i;
		}
		return depth;
	}

public:
	float im = 1;

	void run(int w, int h, char* data, int AA=1) {
		for (int i = 0; i < w; i++) {
			for (int j = 0; j < h; j++) {
				float x = ((float)i / w)*2-1;
				float y = ((float)j / h)*2-1;

				float k=0;// = Recursions(Complex(0, 0), Complex(x, y));
				for (int s = 0; s < AA; s++) {
					for (int l = 0; l < AA; l++) {
						k += Recursions(Complex(0, 0), Complex(x + ((float)s) / (AA * w), y + ((float) l) / (AA * h)));
					}
				}
				k /= (AA * AA);

				data[(i + j * w) * 3] = k * 255 / depth*10;
				data[(i + j * w) * 3+1] = k * 255 / depth;
				data[(i + j * w) * 3+2] = k * 255 / depth;
			}
		}
	}
};

