using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Events;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Interfaces;
using System.Collections.ObjectModel;
using Viewer.Infrastructure;

namespace Viewer.Tools.ViewModels
{

    public class UI_RNode : TreeViewItemViewModel
    {

        IRNode _thisItem;
        IApplicationData _data;
        public UI_RNode(IApplicationData data, IRNode currentItem, TreeViewItemViewModel parent) : base(parent, true)
        {
            _thisItem = currentItem;
            _data = data;
        }

        public IRNode ThisItem
        {
            get
            {
                return _thisItem;
            }

            set
            {
                _thisItem = value;
            }
        }

        protected override void LoadChildren()
        {
            var transNode = (IRTransformNode)ThisItem;

            foreach (var item in transNode.Children)
            {
                base.Children.Add(new UI_RNode(_data,item, this));
            }
        }

        public string IconPath { get { return _data.GetIconPath(_thisItem.NodeType + ".png"); } }

        public string RNodeName { get { return _thisItem.GetName(); } }
    }

    public class TreepanelViewModel : ToolViewModel
    {
        ObservableCollection<UI_RNode> _children;

        public TreepanelViewModel() : base("TreeView")
        {
            MinWidth = 250;
        }

        public ObservableCollection<UI_RNode> Children
        {
            get
            {
                return _children;
            }

            set
            {
                _children = value;
                RaisePropertyChanged();
            }
        }

        IApplicationData _data; IEventAggregator _eventagg;

        IRBuilderDocument _rDoc;

        public override bool Connect(IApplicationData data, IEventAggregator eventaggre)
        {
            _eventagg = eventaggre;
            _data = data;

            _rDoc = data.ActiveDocument as IRBuilderDocument;

            List<UI_RNode> lst = new List<UI_RNode>()
            {
                new UI_RNode(data, _rDoc.Tree.GeometryParentNode,null)
            };

            Children = new ObservableCollection<UI_RNode>(lst);

            return true;
        }
        public override bool Disconnect()
        {
            return true;
        }
        internal void TreeViewSelectionChanged(IRNode thisItem)
        {
            if (thisItem == null)
                return;

            _rDoc.SelectedNodes.Clear();
            _rDoc.SelectedNodes.Add(thisItem);
            _eventagg.GetEvent<NodeSelectionChangedEvent>().Publish(thisItem);
        }
    }
}
