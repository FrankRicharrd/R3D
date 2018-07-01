using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces
{
    public interface IRNode
    {

        RNodeType NodeType { get; }

        string Name { get; set; }

        string GetName();
        void SetName(string name);
        void SetPolygonMode(int mode);
        object GetWrappedNativeObject();

        IRNode Parent { get; set; }

    }
}
