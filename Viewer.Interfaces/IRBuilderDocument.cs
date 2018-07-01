using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces.IntelligentObject;

namespace Viewer.Interfaces
{
    public interface IRBuilderDocument : IRDocument
    {
        ITree Tree { get; set; }
        List<string> GetShaderPaths();
        void AddShaderPathToCurrentShaderLibrary(string path);

        List<IRNode> SelectedNodes { get; set; }

        IRModelingManager GetModelingManager();

        IRMaterialManager GetMaterialManager();

        void SetSelectiontype(SelectionType type);

        IRLightManager GetLightsManager();

    }
}
