using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;

namespace Viewer.Commands
{
    public class ModelingCommands
    {

        IApplicationData _data;
        IRModelingManager _modeler;
        public ModelingCommands(IApplicationData data, IRModelingManager modeler)
        {
            _data = data;
            _modeler = modeler;
        }

        public void CreateSphere()
        {
            _modeler.CreateSphere(10, 25, 25);
        }


    }
}
