using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces.IntelligentObject
{

    public class RIntelligentObjectAttribute : Attribute
    {


    }
    public interface IRIntelligentObjectPlugin
    {
        string IconPath { get; set; }
        string Name { get; set; }

        // this get triggered when plugin get loaded
        bool Connect(IApplicationData data, object IOManager);

        //Called when user picks a point in 3d space
        bool CreateObject();
    }
}
