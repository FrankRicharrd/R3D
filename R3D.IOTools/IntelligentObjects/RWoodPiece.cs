using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using R3DIOInterop;
namespace R3D.IOTools.IntelligentObjects
{
    public class RWoodPiece
    {
        RIntelligentObjectInterop _iObject;

        RIntelliDrawableDefInterop _definition;

        List<RIntelliDrawableInstanceInterop> _instances = new List<RIntelliDrawableInstanceInterop>();

        int _faceIndexForPositiveLength;
        int _faceIndexForPositiveWidth;

        int _faceIndexForNegativeWidth;


        public RIntelliDrawableDefInterop Definition
        {
            get
            {
                return _definition;
            }

            set
            {
                _definition = value;
            }
        }

        public int FaceIndexForPositiveLength
        {
            get
            {
                return _faceIndexForPositiveLength;
            }

            set
            {
                _faceIndexForPositiveLength = value;
            }
        }

        public int FaceIndexForPositiveWidth
        {
            get
            {
                return _faceIndexForPositiveWidth;
            }

            set
            {
                _faceIndexForPositiveWidth = value;
            }
        }

        public int FaceIndexForNegativeWidth
        {
            get
            {
                return _faceIndexForNegativeWidth;
            }

            set
            {
                _faceIndexForNegativeWidth = value;
            }
        }

        public RWoodPiece(R3DIOInterop.RIntelligentObjectInterop iObject)
        {
            _iObject = iObject;
        }

        public void DefineWoodPiece(double thickness, double length, double width)
        {
            RVertex3DInterop v0 = new RVertex3DInterop(0, 0, 0);
            RVertex3DInterop v1 = new RVertex3DInterop(length, 0, 0);
            RVertex3DInterop v2 = new RVertex3DInterop(length, thickness, 0);
            RVertex3DInterop v3 = new RVertex3DInterop(0, thickness, 0);
            RVertex3DInterop v4 = new RVertex3DInterop(0, 0, width);
            RVertex3DInterop v5 = new RVertex3DInterop(length, 0, width);
            RVertex3DInterop v6 = new RVertex3DInterop(length, thickness, width);
            RVertex3DInterop v7 = new RVertex3DInterop(0, thickness, width);

            List<RVertex3DInterop> lstvertices = new List<RVertex3DInterop>();
            lstvertices.Add(v0);
            lstvertices.Add(v1);
            lstvertices.Add(v2);
            lstvertices.Add(v3);
            lstvertices.Add(v4);
            lstvertices.Add(v5);
            lstvertices.Add(v6);
            lstvertices.Add(v7);



            Definition = _iObject.CreateIntelliDrawableDefinition(lstvertices);

            int f1 = Definition.CreateFace(new List<int>() { 4, 5, 6, 7 });
            int f2 = Definition.CreateFace(new List<int>() { 5, 1, 2, 6 });
            int f3 = Definition.CreateFace(new List<int>() { 1, 0, 3, 2 });
            int f4 = Definition.CreateFace(new List<int>() { 0, 4, 7, 3 });
            int f5 = Definition.CreateFace(new List<int>() { 7, 6, 2, 3 });
            int f6 = Definition.CreateFace(new List<int>() { 0, 1, 5, 4 });

            Definition.AddFaceDragger(f1);
            Definition.AddFaceDragger(f2);
            Definition.AddFaceDragger(f3);
            Definition.AddFaceDragger(f4);
            Definition.AddFaceDragger(f5);
            Definition.AddFaceDragger(f6);

            FaceIndexForPositiveLength = f2;
            FaceIndexForPositiveWidth = f1;
            FaceIndexForNegativeWidth = f3;
        }


        public RIntelliDrawableInstanceInterop CreateDynamicInstance(double locationX, double locationY, double locationZ)
        {
            var instance = Definition.CreateDynamicInstance();

            instance.SetMatrix(new List<double>()
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                locationX, locationY, locationZ,1

            });

            _instances.Add(instance);

            return instance;
        }

        public RIntelliDrawableInstanceInterop CreateInstance(double locationX, double locationY, double locationZ )
        {

            var instance = Definition.CreateInstance();

            instance.SetMatrix(new List<double>()
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                locationX, locationY, locationZ,1

            });

            _instances.Add(instance);

            return instance;
        }

    }
}
