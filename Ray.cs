using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathNet.Numerics.LinearAlgebra;

namespace RayTracer {
    class Ray {
        public Vector<float> Source { get; private set; }
        public Vector<float> Direction { get; private set; }

        public Ray(Vector<float> source, Vector<float> direction) {
            this.Source = source;
            this.Direction = direction;
        }
    }
}
