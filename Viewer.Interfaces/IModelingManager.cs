using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces
{
    public interface IRModelingManager
    {
        IRTransformNode CreateSphere(double radius, int rings, int sector);
        void CreateCustomData(string fileName, object v);

        string GetNumberOfDrawables();
    }
}
