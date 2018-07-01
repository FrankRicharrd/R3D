using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Infrastructure;
using Viewer.Interfaces;

namespace Viewer.Tools.ViewModels
{
    public class PropertiesViewModel : ToolViewModel
    {

        IEventAggregator _eventAggregator;
        private IRNode selectedItem;

        public IRNode SelectedItem
        {
            get
            {
                return selectedItem;
            }

            set
            {
                selectedItem = value;
                RaisePropertyChanged();
            }
        }

        IApplicationData _data;

        public PropertiesViewModel() : base("Properties")
        {

            MinWidth = 250;
        }

        public override bool Disconnect()
        {
            return true;
        }


        string _contenttext;

        public string Contenttext
        {
            get
            {
                return _contenttext;
            }

            set
            {
                _contenttext = value;
                RaisePropertyChanged();
            }
        }

       

        private void OnSelectedNodeChanged(IRNode obj)
        {
            if (obj == null)
            {
                SelectedItem = null;
                return;
            }

            SelectedItem = obj;

            //if(obj.NodeType == RNodeType.Transform)
            //{
            //    SelectedItem = obj as RTransformNode;
            //}
            //else if( obj.NodeType == RNodeType.Geometry)
            //{
            //    SelectedItem = obj as RGeometryNode;
            //}
        }

        public override bool Connect(IApplicationData data, IEventAggregator eventaggre)
        {

            _data = data;
            _eventAggregator = eventaggre;
            _eventAggregator.GetEvent<NodeSelectionChangedEvent>().Subscribe(OnSelectedNodeChanged);
            return true;
        }
    }
}
