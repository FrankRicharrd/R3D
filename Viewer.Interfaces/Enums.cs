using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer
{
    public enum RNodeType
    {
        Transform,
        Geometry, 
        Light,
        Material
    }

    public enum DraggerType
    {
        TabBoxDragger = 0,
        TabPlaneDragger = 1,
        TabBoxTrackballDragger = 2,
        TrackballDragger = 3,
        Translate1DDragger = 4,
        Translate2DDragger = 5,
        TranslateAxisDragger = 6,
        TabPlaneTrackballDragger = 7,
        TranslatePlaneDragger = 8,
        Scale1DDragger = 9,
        Scale2DDragger = 10,
        RotateCylinderDragger = 11,
        RotateSphereDragger = 12,
    };


    public enum MouseButton
    {
        Left = 1,
        Middle = 2,
        Right = 3,
    }

    public enum FViewType
    {
        Top = 0,
        Bottom = 1,
        Front = 2,
        Back = 3,
        Left = 4,
        Right = 5,
        Perspective = 6
    };


    public enum ShaderType
    {
        [Description("Vertex")]
        VERTEX = 35633, //GL_VERTEX_SHADER,
        [Description("TESSCONTROL")]
        TESSCONTROL = 36488 ,//GL_TESS_CONTROL_SHADER,
        [Description("TESSEVALUATION")]
        TESSEVALUATION = 36487,//GL_TESS_EVALUATION_SHADER,
        [Description("GEOMETRY")]
        GEOMETRY = 36313 ,//GL_GEOMETRY_SHADER,
        [Description("FRAGMENT")]
        FRAGMENT = 35632, //GL_FRAGMENT_SHADER,
        //COMPUTE = GL_COMPUTE_SHADER,
        [Description("UNDEFINED")]
        UNDEFINED = -1
    };


    public enum SelectionType
    {
        TransformNode,
        GeometryNode,
        IntelligentObject
    }

}
