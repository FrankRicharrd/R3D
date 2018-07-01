using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces.Geometry;
//using ViewerCommon.Geometry;
//using ViewerCommon.Model;

namespace Viewer.Interfaces
{
    public interface IRTransformNode : IRNode
    {
        List<IRNode> Children { get; set; }

        void AddChild(IRNode childNode);
        IVec3 GetTranslation();
        void SetTranslation(IVec3 vec);

        void RemoveChild(IRNode node);

    }
}
