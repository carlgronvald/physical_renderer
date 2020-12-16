using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.LinearAlgebra.Double;



namespace RayTracer {
    class Program {

        public static string Vecstr(Vector<float> v) {
            return "<" + v[0] + "," + v[1] + "," + v[2] + ">";
        }

        public static Vector<float> Vec3(float v1, float v2, float v3) {
            return Vector<float>.Build.DenseOfArray(new float[] { v1, v2, v3 });
        }

        public static Vector<float> CrossProduct(Vector<float> vec1, Vector<float> vec2) {
            return Vec3(vec1[1] * vec2[2] - vec2[1] * vec1[2], vec1[2] * vec2[0] - vec2[2] * vec1[0], vec1[0] * vec2[1] - vec1[1] * vec2[0]);
        }
        
        private Camera cam;

        private void Run() {
            cam = new Camera(Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0,1,0), 1, 53.5f);

            int width = 11, height = 11;
            float baseX = -1 + 1 / (float)width;
            float baseY = -1 + 1 / (float)height;
            float xmod = 2 / (float)width;
            float ymod = 2 / (float)height;

            int[] colors = new int[width*height];

            Vector<float> circleCenter = Vec3(5, 0, 0);
            float radius = 3;

            for(int i=0;i<width;i++) {
                for(int j=0;j<height;j++) {
                    Ray r = cam.CreateRay(baseX + i*xmod, baseY+j*ymod);

                    Vector<float> Delta = r.Source - circleCenter;
                    Vector<float> D = r.Direction;
                    float delta = (D.DotProduct(Delta) * D.DotProduct(Delta)) - D.DotProduct(D) * (Delta.DotProduct(Delta) - radius * radius);
                    if (delta >= 0) {
                        float t = -(float)(D.DotProduct(Delta) + Math.Sqrt((D.DotProduct(Delta) * D.DotProduct(Delta)) - D.DotProduct(D) * (Delta.DotProduct(Delta) - radius * radius))) / (D.DotProduct(D));
                        Console.WriteLine(i + "," + j + " intersects with t="+t + " at " + Vecstr(r.Source+t*D));
                        colors[i * width + j] = 1;
                    } else {
                        colors[i * width + j] = 0;
                    }
                }
            }
            for(int i=0;i<width*height;i++) {
                Console.Write(colors[i] == 1 ? "x" : "0");
                if (i % width == width-1) Console.WriteLine(); 
            }
        }


        static void Main(string[] args) {
            new Program().Run();
            

            Matrix<double> A = DenseMatrix.OfArray(new double[,] {
                {1,2,3 },
                {4,5,6 },
                {7,8,9 }
            });

            Console.ReadLine();
        }
    }
}
