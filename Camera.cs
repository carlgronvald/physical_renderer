using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathNet.Numerics.LinearAlgebra;

namespace RayTracer {
    class Camera {        
        private Vector<float> Right { get; set; }
        private Vector<float> Up { get; set; }
        private Vector<float> ScreenCenter { get; set; }
        public Vector<float> Location {
            get; private set;
        }
        public Vector<float> Direction {
            get; private set;
        }
        public float MinZ {
            get; private set;
        }
        public float FOV {
            get; private set;
        }

        public Camera(Vector<float> location, Vector<float> direction, Vector<float> up, float minZ, float fov) {
            this.Location = location;
            this.Direction = direction;
            this.Up = up;
            this.Right = Program.CrossProduct(Direction, Up)*(float)Math.Tan(fov/180*Math.PI);// TODO: GET FOV TO WORK
            this.MinZ = minZ;
            this.FOV = fov;
            this.ScreenCenter = Location + Direction;
        }

        /// <summary>
        /// x and y are given in the [-1,1] box.
        /// </summary>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        public Ray CreateRay(float x, float y) {
            Vector<float> src = ScreenCenter + x * Right + y * Up;
            Vector<float> dir = src - Location;
            dir = dir.Normalize(2);

            return new Ray(src, dir);
        }
    }
}
