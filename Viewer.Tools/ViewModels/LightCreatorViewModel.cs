using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Media;
using Prism.Events;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Interfaces;
using System.Windows.Input;
using Viewer.Commands;
using Viewer.Infrastructure;

namespace Viewer.Tools.ViewModels
{
    public class LightCreatorViewModel : ToolViewModel
    {

        double _x;
        double _y;
        double _z;


        IRTransformNode _selectedNode;

        Color _selectedDiffuseColor;
        public ICommand CreateCommand { get; set; }

        public LightCreatorViewModel() : base("LightCreator")
        {
            CreateCommand = new RelayCommand(CreateCommandHandler);
        }

        private void CreateCommandHandler()
        {
            var light = _manager.CreateLight(SelectedNode, SelectedDiffuseColor, X, Y, Z);
        }

        IApplicationData _data; IEventAggregator _eventaggre;
        IRBuilderDocument _doc;
        IRLightManager _manager;
        public Color SelectedDiffuseColor
        {
            get
            {
                return _selectedDiffuseColor;
            }

            set
            {
                _selectedDiffuseColor = value;
                RaisePropertyChanged();
            }
        }

        public IRTransformNode SelectedNode
        {
            get
            {
                return _selectedNode;
            }

            set
            {
                _selectedNode = value;
                RaisePropertyChanged();
            }
        }

        public double X
        {
            get
            {
                return _x;
            }

            set
            {
                _x = value;
                RaisePropertyChanged();

            }
        }

        public double Y
        {
            get
            {
                return _y;
            }

            set
            {
                _y = value;
                RaisePropertyChanged();

            }
        }

        public double Z
        {
            get
            {
                return _z;
            }

            set
            {
                _z = value;
                RaisePropertyChanged();
            }
        }

        public override bool Connect(IApplicationData data, IEventAggregator eventaggre)
        {
            _data = data;
            _eventaggre = eventaggre;

            _doc = _data.ActiveDocument as IRBuilderDocument;
            _manager = _doc.GetLightsManager();

            eventaggre.GetEvent<OnTransformNodeSelectedOnScreen>().Subscribe(OnTransformNodeSelectedOnScreenHandler);

            return true;
        }

        private void OnTransformNodeSelectedOnScreenHandler(IRTransformNode obj)
        {
            SelectedNode = obj;
        }

        public override bool Disconnect()
        {
            return true;
        }
    }
}
