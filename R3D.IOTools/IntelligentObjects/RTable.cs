using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using R3DIOInterop;
using Viewer.Interfaces;
using Viewer.Interfaces.IntelligentObject;

namespace R3D.IOTools.IntelligentObjects
{
    public class RTable : IRIntelligentObjectPlugin
    {
        List<RWoodPiece> _woodPieces = new List<RWoodPiece>();

        RIntelligentObjectManagerInterop _manager;

        RIntelligentObjectInterop _iObject;

        RIOTranslateDraggerInterop _rightDragger;
        RIOTranslateDraggerInterop _frontDragger;

        double _changedLength = 0;

        double Thickness = 1;
        double _length;
        IApplicationData _data;
        string _iconPath;
        public string IconPath
        {
            get
            {
                return _iconPath;
            }

            set
            {
                _iconPath = value;
            }
        }

        string _name;
        public string Name
        {
            get
            {
                return _name;
            }

            set
            {
                _name = value;
            }
        }

        public RTable()
        {

        }

        double _width;
        double _height;
        double _legoffset;

        RIntelliDrawableInstanceInterop _topPieceInstance;
        RIntelliDrawableInstanceInterop _l1;
        RIntelliDrawableInstanceInterop _l2;
        RIntelliDrawableInstanceInterop _l3;
        RIntelliDrawableInstanceInterop _l4;

        public void CreateTable(double length, double Width, double Height, double LegOffset = 0)
        {
            _length = length;
            _width = Width;
            _height = Height;
            _legoffset = LegOffset;
            _changedLength = length;
            _iObject = _manager.CreateIntelligentObject();

            RWoodPiece topPiece = new RWoodPiece(_iObject);
            topPiece.DefineWoodPiece(Thickness, length, Width);
            var topPieceInstance = topPiece.CreateInstance(0, Height, 0);
            _topPieceInstance = topPieceInstance;
            _woodPieces.Add(topPiece);
            // topPiece.Definition.SetDraggerVisibility(true);


            RWoodPiece Leg = new RWoodPiece(_iObject);
            Leg.DefineWoodPiece(Height, Thickness, Thickness);
            _woodPieces.Add(Leg);

            //Table legs...

            //l1 .......l4
            //l2.......l3
            //...


            var l1 = Leg.CreateInstance(LegOffset, 0, LegOffset);
            var l2 = Leg.CreateInstance(LegOffset, 0, (Width - Thickness) - LegOffset);
            var l3 = Leg.CreateInstance((length - Thickness) - LegOffset, 0, (Width - Thickness) - LegOffset);
            var l4 = Leg.CreateInstance((length - Thickness) - LegOffset, 0, LegOffset);

            _l1 = l1;
            _l2 = l2;
            _l3 = l3;
            _l4 = l4;

            _rightDragger = _iObject.CreateTranslateDragger(
                new RVertex3DInterop(1, 0, 0), new RVertex3DInterop(length, Height / 2, Width / 2),
                new List<RIntelliDrawableInstanceInterop>()
                {
                    l3, l4
                },
                new List<RIntelliDrawableInstanceInterop>()
                {
                    topPieceInstance
                }
                );

            _rightDragger.SubscribeBeforeUpdatingEvent(new BeforeUpdatingDelegate(OnRightDraggerChanged));

            _rightDragger.SubscribeOnReleaseEvent(new OnReleaseDelegate(OnRightDraggerRelease));


            //var primaryEntity = new KeyValuePair<RIntelliDrawableInstanceInterop, int>(topPieceInstance,topPiece.FaceIndexForLength);

            //var secondaryEntity = new KeyValuePair<RIntelliDrawableInstanceInterop, int>(l4, Leg.FaceIndexForLength);

            //var faceConstrain = _iObject.CreateFaceConstrain(primaryEntity, secondaryEntity);

            //faceConstrain.SetConstrainType(RIOConstrainType.NegativeDistance);
            //faceConstrain.SetFaceConstrainDistance(LegOffset);

            CreateConstrain(topPieceInstance, topPiece.FaceIndexForPositiveLength, l4, Leg.FaceIndexForPositiveLength, LegOffset, RIOConstrainType.NegativeDistance);
            CreateConstrain(topPieceInstance, topPiece.FaceIndexForPositiveLength, l3, Leg.FaceIndexForPositiveLength, LegOffset, RIOConstrainType.NegativeDistance);


            //frontDragger

            _frontDragger = _iObject.CreateTranslateDragger(
                new RVertex3DInterop(0, 0, 1), new RVertex3DInterop(length / 2, Height / 2, Width),
                new List<RIntelliDrawableInstanceInterop>()
                {
                    l2, l3
                },
                new List<RIntelliDrawableInstanceInterop>()
                {
                    topPieceInstance
                }
                );

            _frontDragger.SubscribeBeforeUpdatingEvent(new BeforeUpdatingDelegate(OnFrontDraggerChanged));

            _frontDragger.SubscribeOnReleaseEvent(new OnReleaseDelegate(OnFrontDraggerRelease));


            //var primaryEntityFront = new KeyValuePair<RIntelliDrawableInstanceInterop, int>(topPieceInstance, topPiece.FaceIndexForWidth);

            //var secondaryEntityFront = new KeyValuePair<RIntelliDrawableInstanceInterop, int>(l2, Leg.FaceIndexForWidth);

            //var faceConstrainFront = _iObject.CreateFaceConstrain(primaryEntityFront, secondaryEntityFront);

            //faceConstrainFront.SetConstrainType(RIOConstrainType.NegativeDistance);
            //faceConstrainFront.SetFaceConstrainDistance(LegOffset);


            CreateConstrain(topPieceInstance, topPiece.FaceIndexForPositiveWidth, l2, Leg.FaceIndexForPositiveWidth, LegOffset, RIOConstrainType.NegativeDistance);
            CreateConstrain(topPieceInstance, topPiece.FaceIndexForPositiveWidth, l3, Leg.FaceIndexForPositiveWidth, LegOffset, RIOConstrainType.NegativeDistance);



















            _iObject.Redraw(true);
            _iObject.SetIODraggerVisiblility(true);
        }

        private void CreateConstrain(RIntelliDrawableInstanceInterop primaryInstance, int primaryFaceid, RIntelliDrawableInstanceInterop secondaryInstance, int secondaryFaceId, double distance, RIOConstrainType constrainType)
        {
            var primaryEntity = new KeyValuePair<RIntelliDrawableInstanceInterop, int>(primaryInstance, primaryFaceid);

            var secondaryEntity = new KeyValuePair<RIntelliDrawableInstanceInterop, int>(secondaryInstance, secondaryFaceId);

            var faceConstrain = _iObject.CreateFaceConstrain(primaryEntity, secondaryEntity);

            faceConstrain.SetConstrainType(constrainType);
            faceConstrain.SetFaceConstrainDistance(distance);
        }

        private bool OnFrontDraggerRelease()
        {
            return true;
        }

        private bool OnFrontDraggerChanged(double A_0, double A_1, double A_2)
        {
            return true;
        }

        List<RIntelliDrawableInstanceInterop> _rightDraggerInstances = new List<RIntelliDrawableInstanceInterop>();
        private bool OnRightDraggerRelease()
        {

            if (_rightDraggerInstances.Count == 0)
            {
                _rightDraggerInstances.Add(_woodPieces[1].CreateDynamicInstance((_changedLength / 2), 0, (_width - Thickness) - _legoffset));
                CreateConstrain(_topPieceInstance, _woodPieces[0].FaceIndexForPositiveWidth, _rightDraggerInstances[0], _woodPieces[1].FaceIndexForPositiveWidth, _legoffset, RIOConstrainType.NegativeDistance);

                _rightDraggerInstances.Add(_woodPieces[1].CreateDynamicInstance((_changedLength / 2), 0, _legoffset));
                CreateConstrain(_topPieceInstance, _woodPieces[0].FaceIndexForNegativeWidth, _rightDraggerInstances[1], _woodPieces[1].FaceIndexForNegativeWidth, _legoffset, RIOConstrainType.PositiveDistance);

            }
            else
            {
                _rightDraggerInstances[0].SetMatrix(GetTranslatedMatrix((_changedLength / 2), 0, (_width - Thickness) - _legoffset));

                _rightDraggerInstances[1].SetMatrix(GetTranslatedMatrix((_changedLength / 2), 0,  _legoffset));

            }

            return true;
        }

        private static List<double> GetTranslatedMatrix(double x, double y, double z)
        {
            return (new List<double>()
            {
                1,0,0,0,
                0,1,0,0,
                0,0,1,0,
                x, y, z,1
            });


        }

        public bool OnRightDraggerChanged(double x, double y, double z)
        {
            _changedLength = _changedLength + x;

            return true;
        }


        public bool Connect(IApplicationData data, object IOManager)
        {
            _manager = (R3DIOInterop.RIntelligentObjectManagerInterop)IOManager;
            _data = data;
            Name = "SampleTable";
            IconPath = data.GetIconPath("sampletable.png");
            return true;
        }

        public bool CreateObject()
        {
            CreateTable(10, 5, 5, 0.25);
            return true;
        }
    }
}
