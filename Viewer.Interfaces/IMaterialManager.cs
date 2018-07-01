using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces
{
    public interface IRMaterialManager
    {

        bool SetShaderToNode(string shaderprogram, ShaderType shaderType, IRNode node);

    }
}
